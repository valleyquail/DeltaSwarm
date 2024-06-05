//
// Created by nikesh on 5/28/24.
//

#include "i2c_control.h"
#include "Arduino.h"
#include "motion_controller.h"


void motionCallback(int8_t packet_index) {

#ifdef DEBUG
    Serial.printf("Motor speeds: %s\n", data_packets[packet_index].buffer);
#endif
    motionController.setSpeedFromI2C(data_packets[packet_index].buffer);
}