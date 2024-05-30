
#ifdef __cplusplus
extern "C"
{
#endif

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include "../../include/config.h"

/**
 * @brief This is a an API that allows users to register new I2C actions without having to modify some functions directly.
 * This involves some function pointer wizardry that will compile into a function that accounts for adding a I2C response to an
 * incoming instruction
 */

typedef void (*i2c_response_t)();

void register_i2c_function(i2c_response_t callback, int packet_address);

// This is used as a general structure to maintain data handling throughout the
// program. It has a few uses:
// 1. It can be used as the buffer to read values from the ESP and then signal
//    that the motor speeds should change based on new data
// 2. It can be used to send information to the ESP from the interrupt sequence

// The union inside is a little bit goofy, but it prevents the need for
// creating multiple structs for different purposes.

// new_motor_speed: This is used to signal that the motor speeds should be
// changed based on the data in the buffer

// message_sent: This is used to signal that the message has been sent to the
// ESP

// is_receiving_message: This is used to signal that the Pico is currently
struct DataPacket {
    uint8_t buffer[32];
    uint8_t index;
    union {
        bool message_sent;
        bool not_receiving_message;
    } status;
};

extern struct DataPacket data_packets[];

// Represents the "registers" the ESP can read from
// In reality, these are indices into the data_packets array that stores the values
enum I2C_COMMANDS {
    MOTOR_SPEEDS = PICO_MOTOR_COMMAND_REGISTER,
    ODOMETRY = PICO_ODOMETRY_COMMAND_REGISTER,
    ENCODER_COUNT = PICO_ENCODER_COUNT_COMMAND_REGISTER,

    NONE = 127,
};

void initPicoPeriph();
void initPicoController();

//I2C callbacks

void motionCallback(int packet_address);


#ifdef __cplusplus
}
#endif
