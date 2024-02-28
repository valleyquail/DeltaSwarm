#include "I2C.h"
#include "../../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"

#define I2C_MASTER_TIMEOUT_MS 10
// Configuration for the I2C bus
extern "C" i2c_config_t i2c_config = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = PICO_SDA1,
    .scl_io_num = PICO_SCL1,
    // Pullups do not need to be enabled since they are pulled up by the pico
    .sda_pullup_en = GPIO_PULLUP_DISABLE,
    .scl_pullup_en = GPIO_PULLUP_DISABLE,
};

i2c_port_t i2c_master_port = I2C_NUM_1;

Pico_I2C::Pico_I2C()
{
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
    if (i2c_driver_install_ret != ESP_OK)
    {
        printf("I2C driver install failed\n");
        return;
    }
    printf("I2C driver installed\n");
#endif
}

bool Pico_I2C::init()
{
    // Initialize the motor command buffer
    // Address
    this->motorCommandBuffer[0] = PICO_MOTOR_COMMAND_REGISTER & 0xF0 >> 8;
    this->motorCommandBuffer[1] = PICO_MOTOR_COMMAND_REGISTER & 0xF;
    // Flags
    this->motorCommandBuffer[2] = 'T';
    this->motorCommandBuffer[7] = 'F';
    this->encoderCountBuffer[12] = 'T';

    // Check that the ok is received:
    uint8_t check_ok[2];
    // Write to the Pico's motor command address and then check that it receives
    // the 'ok' buffer to make sure that it is communicating properly
    i2c_master_write_read_device(i2c_master_port, PICO_ADDRESS, motorCommandBuffer, 2, check_ok, 2, 1000 / configTICK_RATE_HZ);
    if ((char)check_ok[0] == 'o' && (char)check_ok[1] == 'k')
    {
        printf("connection to the pico is working\n");
        return true;
    }
    return false;
}

void Pico_I2C::sendMovement(float speed, float theta, float omega, bool orientation)
{
    // Send the motor command
    printf("Sending motor command\n");
    memcpy(&this->motorCommandBuffer[3], (uint8_t *)&speed, 4);
    memcpy(&this->motorCommandBuffer[9], (uint8_t *)&theta, 4);
    memcpy(&this->motorCommandBuffer[13], (uint8_t *)&omega, 4);
#ifdef DEBUG
    printf("Motor command: %f, %f, %f\n", speed, theta, omega);
#endif
    if (orientation)
    {
        this->motorCommandBuffer[15] = 'K';
    }
    else
    {
        this->motorCommandBuffer[15] = 'k';
    }
    i2c_master_write_to_device(i2c_master_port, PICO_ADDRESS, motorCommandBuffer, MOTOR_COMMAND_SIZE, I2C_MASTER_TIMEOUT_MS / configTICK_RATE_HZ);
}