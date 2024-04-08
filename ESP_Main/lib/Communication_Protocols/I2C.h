#include <stdio.h>
#include <stdlib.h>
#include "../../include/config.h"
#include "driver/i2c.h"


bool pico_i2c_init();
void sendMovement(float speed, float theta, float omega, bool orientation);
void requestEncoderCounts();
// TODO: Break out the different types of IMU data
void requestIMUData();
