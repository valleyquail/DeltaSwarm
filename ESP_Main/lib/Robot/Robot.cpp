//
// Created by nikesh on 3/27/24.
//

#include "Robot.h"
#include "../Communication_Protocols/i2c_management.h"


Robot::Robot(int robot_number) { this->robot_number = robot_number; }

bool Robot::init() {
    // Initialize the Pico I2C

    if (!pico_i2c_init()) return false;

    return true;
}
