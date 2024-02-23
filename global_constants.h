#define PI_AS_HOST 0

#define PICO_BAUD_RATE 115200
#define PICO_USE_USB_SERIAL 0

#define PICO_ADDRESS 0x40
#define ICM_20948_ADDRESS 0x68
#define ESP_32_I2C_ADDRESS 0x03

#define USE_I2C_FROM_ESP 1

// Fast mode
#define PICO_ESP_FREQ 400000
// Defines for data packet size and data "registers" for the pico
// Essentially this is what configures the pico as the client device, and this
// is what the ESP is going to be reading and writing from

#define NUM_PICO_REGISTERS 3
// Motor command size and address at the pico
// This is information that is sent to the pico
#define MOTOR_COMMAND_SIZE 16
#define PICO_MOTOR_COMMAND_REGISTER 0x00

// This is information that will be sent from the pico
#define ENCODER_COMMAND_SIZE 16
#define PICO_ENCODER_COMMAND_REGISTER 0x01

#define IMU_COMMAND_SIZE 16
#define PICO_IMU_COMMAND_REGISTER 0x02

// Normal rate
#define PICO_I2C_FREQ 100000
