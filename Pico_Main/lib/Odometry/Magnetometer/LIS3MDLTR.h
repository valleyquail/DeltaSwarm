//
// Created by nikesh on 5/24/24.
//

#ifndef PICO_MAIN_LIS3MDLTR_H
#define PICO_MAIN_LIS3MDLTR_H
#include "pico/stdlib.h"

class LIS3MDLTR {
private:

public:
    const float noise = 0;

    int16_t x,     ///< The last read X mag in raw units
    y,         ///< The last read Y mag in raw units
    z;         ///< The last read Z mag in raw units
    float x_gauss, ///< The last read X mag in 'gauss'
    y_gauss,   ///< The last read Y mag in 'gauss'
    z_gauss;   ///< The last read Z mag in 'gauss'

    void init();


};


#endif //PICO_MAIN_LIS3MDLTR_H
