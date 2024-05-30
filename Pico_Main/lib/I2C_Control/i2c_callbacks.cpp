//
// Created by nikesh on 5/28/24.
//

#include "i2c_control.h"
#include "Arduino.h"
#include "motion_controller.h"

void motionCallback() {
#ifdef DEBUG
    Serial.printf("Motor speeds: %s\n", data_packets[MOTOR_SPEEDS].buffer);
#endif
    motionController.setSpeedFromI2C(data_packets[MOTOR_SPEEDS].buffer);
}