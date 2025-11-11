

#ifndef I2C_CONTROL_H
#define I2C_CONTROL_H

#include <pico/stdio.h>



/**
 * @brief This is a an API that allows users to register new I2C actions without having to modify some functions directly.
 * This involves some function pointer wizardry that will compile into a function that accounts for adding a I2C response to an
 * incoming instruction
 *
 * NOTE: All I2C function registrations much be called before the initPicoPeriph() function is called!!!
 */

typedef void (*i2c_response_t)(int8_t packet_index);

void register_i2c_function(i2c_response_t callback, int8_t packet_address, int8_t data_len);

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
    uint8_t data_len;
    union {
        bool message_sent;
        bool not_receiving_message;
    } status;
};

extern DataPacket data_packets[];


void initPicoPeriph();
void initPicoController();

void bus_scan();


//I2C callbacks
//______________________________________________________________________________________________________________________
void testCallback(int8_t packet_index);
void motionCallback(int8_t packet_index);


#endif //I2C_CONTROL_H
