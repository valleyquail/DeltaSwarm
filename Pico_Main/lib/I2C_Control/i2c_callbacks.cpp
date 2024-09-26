//
// Created by nikesh on 5/28/24.
//

#include "i2c_control.h"
#include "Arduino.h"
#include "motion_controller.h"

// Callback function for testing the I2C communication
// Needs to exist purely to facilitate assigning a data length
void testCallback(int8_t packet_index) {
}

void motionCallback(int8_t packet_index) {

#ifdef DEBUG
    Serial.printf("Motor speeds: ");
    for (int i = 0; i < 12; i++) {
        Serial.printf("%i ", data_packets[packet_index].buffer[i+1]);
    }
#endif
    // Add one to the data packet buffer to skip the register byte
    MotionController::setSpeedFromI2C(data_packets[packet_index].buffer + 1);
}