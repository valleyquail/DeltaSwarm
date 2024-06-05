#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/config.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "i2c_management.h"

#define I2C_MASTER_TIMEOUT_MS 1000

// Configuration for the I2C bus

i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = PICO_SDA1,
        .scl_io_num = PICO_SCL1,
        // Pullups do not need to be enabled since they are pulled up by the pico
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master = {.clk_speed = PICO_I2C_FREQ},
};


i2c_port_t i2c_master_port = I2C_NUM_1;


// Command buffers
//______________________________________________________________________________________________________________________
uint8_t motorCommandBuffer[MOTOR_COMMAND_SIZE + 1];

// Addresses for reading
//______________________________________________________________________________________________________________________
uint8_t odometryWrite = PICO_ODOMETRY_COMMAND_REGISTER;
uint8_t encoderCountWrite = PICO_ENCODER_COUNT_COMMAND_REGISTER;
uint8_t statusWrite = PICO_STATUS_REGISTER;

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

bool pico_i2c_init() {
    // Configure I2C
#ifdef DEBUG
    printf("Configuring I2C\n");
#endif
    // Set the clock speed of the I2C bus
    // C++ doesn't allow for direct assignment of the i2c_config struct
    i2c_config.master.clk_speed = PICO_I2C_FREQ;
    i2c_param_config(i2c_master_port, &i2c_config);
    // Install the I2C driver
    esp_err_t i2c_driver_install_ret = i2c_driver_install(i2c_master_port, i2c_config.mode, 0, 0, 0);
#ifdef DEBUG
    if (i2c_driver_install_ret != ESP_OK) {
        printf("I2C driver install failed\n");
        return false;
    }
    printf("I2C driver installed\n");
#endif

    // Initialize the motor command buffer
    // Address
    uint8_t reg_addr = PICO_MOTOR_COMMAND_REGISTER;
    // Check that the ok is received:
    uint8_t check_ok[] = {'x', 'x'};
    // Write to the Pico's motor command address and then check that it receives
    // the 'ok' buffer to make sure that it is communicating properly
    ESP_ERROR_CHECK(i2c_master_write_read_device(i2c_master_port, PICO_ADDRESS, &reg_addr, 1, check_ok, 2,
                                                 I2C_MASTER_TIMEOUT_MS / configTICK_RATE_HZ));
    if ((char) check_ok[0] == 'O' && (char) check_ok[1] == 'K') {
        printf("connection to the pico is working\n");
        return true;
    }

    printf("connection to the pico is not working: %s\n", check_ok);

    return false;
}

/***
 * Sends a movement command to the Pico
 * @param speed the speed of the robot
 * @param theta the angle of the robot
 * @param omega the angular velocity of the robot
 * @param orientation whether or not to keep the orientation of the robot during a move
 */

void picoSendMovement(float speed, float theta, float omega, bool orientation) {
    // Send the motor command
#ifdef DEBUG
    printf("Sending motor command\n");
#endif
    // Converts the floats raw bytes and then copies the information into the buffer to send to the pico for calling
    //  sending over the desired motion
    motorCommandBuffer[0] = PICO_MOTOR_COMMAND_REGISTER;
    memcpy(&motorCommandBuffer[1], (uint8_t * ) & speed, 4);
    memcpy(&motorCommandBuffer[5], (uint8_t * ) & theta, 4);
    memcpy(&motorCommandBuffer[9], (uint8_t * ) & omega, 4);
#ifdef DEBUG
    printf("Motor command: %f, %f, %f\n", speed, theta, omega);
#endif
    if (orientation) {
        motorCommandBuffer[14] = 'K';
    } else {
        motorCommandBuffer[14] = 'k';
    }
    i2c_master_write_to_device(i2c_master_port, PICO_ADDRESS, motorCommandBuffer, MOTOR_COMMAND_SIZE,
                               I2C_MASTER_TIMEOUT_MS / configTICK_RATE_HZ);
}


STATUS requestStatus() {
#ifdef DEBUG
    printf("Requesting status\n");
#endif
    uint8_t statusBuffer[2];
    // Request the status of the pico to check if it is functioning properly
    i2c_master_write_read_device(i2c_master_port, PICO_ADDRESS, &statusWrite, 2, statusBuffer, 2,
                                 I2C_MASTER_TIMEOUT_MS / configTICK_RATE_HZ);
    return (STATUS) statusBuffer[0];
}

// X, Y, Theta
float robotOdometry[3];

/***
 * Requests odomtery information from the Pico
 * Stored in the following format: x, y, theta
 * @return the odometry from the Pico
 */
float *requestOdometry() {
#ifdef DEBUG
    printf("Requesting odometry\n");
#endif
    // Request odometry information from the pico to report and/or use for motion planning
    i2c_master_write_read_device(i2c_master_port, PICO_ADDRESS, &odometryWrite, 2, odometryBuffer,
                                 ODOMETRY_COMMAND_SIZE,
                                 I2C_MASTER_TIMEOUT_MS / configTICK_RATE_HZ);
    // Since the information is stored as floats but sent over as bytes, this converts the raw bytes into floats by
    // directly copying over the bytes into an array of floats that contains the requred information
    // TODO (nikesh): Check if the odometry is being sent correctly due to Endianness
    memcpy(&robotOdometry[0], &odometryBuffer[0], 4);
    memcpy(&robotOdometry[1], &odometryBuffer[4], 4);
    memcpy(&robotOdometry[2], &odometryBuffer[8], 4);
    return robotOdometry;
}
