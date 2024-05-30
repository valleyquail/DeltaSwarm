//
// Created by nikesh on 5/24/24.
//

#ifndef PICO_MAIN_ICM_42688_H
#define PICO_MAIN_ICM_42688_H

#include <hardware/i2c.h>
#include "../../I2C_Control/i2c_sensor.h"

class ICM_42688 : public I2C_Sensor{
private:

    float update_freq;
    const float noise = 0;
    const float bias = 0;
    const float scale = 0;


public:

    ICM_42688(float update_freq);





};


#endif //PICO_MAIN_ICM_42688_H
