//
// Created by nikesh on 5/29/23.
//

#ifndef DELTARHO_CONTROLLER_I2C_CONTROL_H
#define DELTARHO_CONTROLLER_I2C_CONTROL_H

#if __cplusplus
extern "C"
{
#endif

#include "hardware/i2c.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

// Values that might want to be changed
//_____________________________________________________________________________
//  The signal frequency
#define UPDATE_FREQ (400 * 1000) // 400kbits/sec send rate
#define MIN_OUTPUT_FREQ 50       // PCA will output at this as the minimum (Hz)

// SDA and SCL pins on the Pico_W
#define SDA 20
#define SCL 21

// User-defined via setting the jumpers on the board itself
#define PCA9685_ADDRESS 0x40

    // Datasheet for the TB6612FNG at:
    // https://www.sparkfun.com/datasheets/Robotics/TB6612FNG.pdf

    //_________________________________________________________________________
    // Set up the constants for the TB6612FNG

    //_________________________________________________________________________

    void init_i2c(void);

    extern void init_PCA(void);
    // void __reset__();
    int reg_read(i2c_inst_t *i2c, uint addr, uint8_t reg, uint8_t *buf,
                 uint8_t nbytes);

    int reg_write(i2c_inst_t *i2c, uint addr, uint8_t reg, uint8_t *buf,
                  uint8_t nbytes);

#if __cplusplus
}
#endif

#endif // DELTARHO_CONTROLLER_I2C_CONTROL_H