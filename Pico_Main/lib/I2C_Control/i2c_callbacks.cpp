//
// Created by nikesh on 5/28/24.
//

#include "i2c_control.h"
#include "Arduino.h"
#include "motion_controller.h"


void motionCallback(int8_t packet_index) {

#ifdef DEBUG
    Serial.printf("Motor speeds:\n");
    for (int i = 0; i < 12; i++) {
        Serial.printf("%d: %02x, ", i, data_packets[packet_index].buffer[i]);
    }
    printf("\n");
#endif
    MotionController::setSpeedFromI2C(data_packets[packet_index].buffer);
}