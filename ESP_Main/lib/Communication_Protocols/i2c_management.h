#include <stdio.h>
#include <stdlib.h>
#include "../../include/config.h"
#include "driver/i2c.h"


//TODO: For future extensibility, make an abstract class for i2c devices and implement it for each device

//Controller functions
//______________________________________________________________________________________________________________________
bool pico_i2c_init();

//Command functions
void picoSendMovement(float speed, float theta, float omega, bool orientation);

//Request functions
//______________________________________________________________________________________________________________________

float *requestOdometry();

//______________________________________________________________________________________________________________________

//Peripheral functions
bool init_esp_client(uint8_t address);