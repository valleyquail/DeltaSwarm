#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/config.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "i2c_management.h"
#include "sdkconfig.h"

#define I2C_MASTER_TIMEOUT_MS 10 /*!< I2C timeout in milliseconds */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */

// Configuration for the I2C bus

i2c_port_t i2c_master_port = (i2c_port_t)PICO_I2C_PORT;


// Command buffers
//______________________________________________________________________________________________________________________
uint8_t motorCommandBuffer[MOTOR_COMMAND_SIZE + 1];

// Addresses for reading
//______________________________________________________________________________________________________________________
uint8_t odometryWrite = PICO_ODOMETRY_COMMAND_REGISTER;
uint8_t encoderCountWrite = PICO_ENCODER_COUNT_COMMAND_REGISTER;


// Read data buffers
//______________________________________________________________________________________________________________________
//  x, y, theta
uint8_t odometryBuffer[16];
// needed for doing inverse kinematics on the arm
uint8_t encoderCountBuffer[16];


/***
 * Initializes the I2C bus for communication with the Pico
 * @return true if the I2C bus was successfully initialized, false otherwise

 */

bool pico_i2c_init()
{
    // Configure I2C
#ifdef DEBUG
    printf("Configuring I2C\n");
#endif

    // ______________________________________________________________________________________________________________________
    // Configure I2C
    i2c_config_t i2c_config;
    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = PICO_SDA1;
    i2c_config.scl_io_num = PICO_SCL1;
    // Pullups do not need to be enabled since they are pulled up by the pico
    i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
    i2c_config.scl_pullup_en = GPIO_PULLDOWN_DISABLE;
    i2c_config.master.clk_speed = PICO_ESP_FREQ;
    i2c_config.clk_flags = 0;
    esp_err_t i2c_param_config_err = i2c_param_config(i2c_master_port, &i2c_config);
    if (i2c_param_config_err != ESP_OK)
    {
        printf("I2C parameter config failed\n");
        return false;
    }
    //_______________________________________________________________________________________________________________________

    // Install the I2C driver
    esp_err_t i2c_driver_install_ret =
        i2c_driver_install(i2c_master_port, i2c_config.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
#ifdef DEBUG
    if (i2c_driver_install_ret != ESP_OK)
    {
        printf("I2C driver install failed\n");
        return false;
    }
    printf("I2C driver installed\n");
#endif
    return true;
}

bool test_pico_connection()
{
    // Check that the ok is received:
    uint8_t set_data[] = {TEST_CONNECTION_REGISTER, 'O', 'K'};
    uint8_t check_ok[] = {'x', 'x', 'x'};
    //Load data into the rp2040 to check that it is getting the values sent from the ESP
    // Write to the rp2040's motor command address and then check that it receives
    // the 'OK' buffer to make sure that it is communicating properly
    i2c_master_write_read_device(i2c_master_port, PICO_ADDRESS, set_data, 3, check_ok, TEST_CONNECTION_SIZE,
                                 I2C_MASTER_TIMEOUT_MS / configTICK_RATE_HZ);
    for (int i = 0 ; i < 3; i++)
    {
        printf("%i ", check_ok[i]);
    }
    printf("\n");
    if (check_ok[0] == 'O' && check_ok[1] == 'K')
    {
        printf("connection to the pico is working\n");
        return true;
    }
    printf("connection to the pico is not working: %c %c\n", check_ok[0], check_ok[1]);
    return false;
}

inline void flipEndian(float value, uint8_t *buffer){
    buffer[0] = (uint8_t) ((buffer[0]) & 0xff);
    buffer[1] = (uint8_t) ((buffer[1]) & 0xff);
    buffer[2] = (uint8_t) (buffer[2] & 0xff);
    buffer[3] = (uint8_t) (buffer[3] & 0xff);
}


/***
 * Sends a movement command to the Pico
 * @param speed the speed of the robot
 * @param theta the angle of the robot
 * @param omega the angular velocity of the robot
 * @param orientation whether or not to keep the orientation of the robot during a move
 */

void picoSendMovement(float speed, float theta, float omega, bool orientation)
{
    // Send the motor command
#ifdef DEBUG
    printf("Sending motor command\n");
#endif
    // Converts the floats raw bytes and then copies the information into the buffer to send to the pico for calling
    //  sending over the desired motion
    //Need to flip the bits, out of order
    motorCommandBuffer[0] = PICO_MOTOR_COMMAND_REGISTER;
    memcpy(motorCommandBuffer + 1, (uint32_t *)&speed, 4);
    memcpy(motorCommandBuffer + 5, (uint32_t *)&theta, 4);
    memcpy(motorCommandBuffer + 9, (uint32_t *)&omega, 4);
    flipEndian(speed, motorCommandBuffer + 1);
    flipEndian(theta, motorCommandBuffer + 5);
    flipEndian(omega, motorCommandBuffer + 9);
#ifdef DEBUG
    printf("Motor command: %f, %f, %f\n", speed, theta, omega);
    for (int i = 0; i < MOTOR_COMMAND_SIZE; i++)
    {
        printf("%d: %02x, ", i, motorCommandBuffer[i]);
    }
    printf("\n");
#endif
    if (orientation)
    {
        motorCommandBuffer[14] = 'K';
    }
    else
    {
        motorCommandBuffer[14] = 'k';
    }
    i2c_master_write_to_device(i2c_master_port, PICO_ADDRESS, motorCommandBuffer, MOTOR_COMMAND_SIZE,
                               I2C_MASTER_TIMEOUT_MS / configTICK_RATE_HZ);
}

// X, Y, Theta
float robotOdometry[3];

/***
 * Requests odomtery information from the Pico
 * Stored in the following format: x, y, theta
 * @return the odometry from the Pico
 */
float *requestOdometry()
{
#ifdef DEBUG
    printf("Requesting odometry\n");
#endif
    // Request odometry information from the pico to report and/or use for motion planning
    i2c_master_write_read_device(i2c_master_port, PICO_ADDRESS, &odometryWrite, 2, odometryBuffer,
                                 ODOMETRY_COMMAND_SIZE, I2C_MASTER_TIMEOUT_MS / configTICK_RATE_HZ);
    // Since the information is stored as floats but sent over as bytes, this converts the raw bytes into floats by
    // directly copying over the bytes into an array of floats that contains the requred information
    // TODO (nikesh): Check if the odometry is being sent correctly due to Endianness
    memcpy(robotOdometry, odometryBuffer, 4);
    memcpy(robotOdometry + 1, odometryBuffer + 4, 4);
    memcpy(robotOdometry + 2, odometryBuffer + 8, 4);
    return robotOdometry;
}
