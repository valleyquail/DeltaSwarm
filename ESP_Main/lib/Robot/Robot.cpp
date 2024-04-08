//
// Created by nikesh on 3/27/24.
//

#include "Robot.h"
#include "../Communication_Protocols/I2C.h"


Robot::Robot(int robot_number) { this->robot_number = robot_number; }

bool Robot::init()
{
    // Initialize the Pico I2C
    bool pico_init_successful = pico_i2c_init();
    if (!pico_init_successful) return false;
    return true;
}
