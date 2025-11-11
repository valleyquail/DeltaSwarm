#ifndef _GLOBAL_CONSTANTS_H_
#define _GLOBAL_CONSTANTS_H_

#define PI_AS_HOST 0

#define PICO_BAUD_RATE 115200
// #define PICO_USE_USB_SERIAL 0

#define PICO_ADDRESS 0x40
#define ICM_20948_ADDRESS 0x68
#define ESP_32_I2C_ADDRESS 0x03

#define USE_I2C_FROM_ESP 1

// Fast mode
#define PICO_ESP_FREQ 100000
// Defines for data packet size and data "registers" for the pico
// Essentially this is what configures the pico as the client device, and this
// is what the ESP is going to be reading and writing from

#define NUM_PICO_REGISTERS 4
// Motor command size and address at the pico
// This is information that is sent to the pico
#define MOTOR_COMMAND_SIZE 13
#define PICO_MOTOR_COMMAND_REGISTER 0x00
#define TEST_CONNECTION_REGISTER 0x03
#define TEST_CONNECTION_SIZE 2
#define PERFORM_CALIBRATION_REGISTER 0x04

// This is information that will be sent from the pico
#define ODOMETRY_COMMAND_SIZE 16
#define PICO_ODOMETRY_COMMAND_REGISTER 0x01

// TODO: Implement encoder count dependent upon the number of additional motors

#define ENCODER_COMMAND_SIZE 16
#define PICO_ENCODER_COUNT_COMMAND_REGISTER 0x02

// I2C frequency for the pico when in master mode
#define PICO_CONTROLLER_I2C_FREQ 400000
#define ICM42688_ADDRESS 0x68
#define LIS3MDL_ADDRESS 0x1E

#endif
