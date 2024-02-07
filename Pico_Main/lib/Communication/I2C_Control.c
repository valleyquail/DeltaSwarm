//
// Created by nikesh on 5/29/23.
//

#include "I2C_Control.h"
#include <math.h>
#include <pico/printf.h>

/**
 * Initializes the pico i2c connections using the default i2c
 */
void init_i2c()
{
    // set up the I2C communication
    i2c_init(i2c_default, UPDATE_FREQ);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);
}

// Read byte(s) from specified register. If nbytes > 1, read from consecutive
// registers.
int reg_read(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf,
             const uint8_t nbytes)
{

    int num_bytes_read = 0;

    // Check to make sure caller is asking for 1 or more bytes
    if (nbytes < 1)
    {
        return 0;
    }
    // Read data from register(s) over I2C
    i2c_write_blocking(i2c, addr, &reg, 1, true);
    num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);

    return num_bytes_read;
}

// Write 1 byte to the specified register
int reg_write(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf,
              const uint8_t nbytes)
{

    int num_bytes_read = 0;
    uint8_t msg[nbytes + 1];

    // Check to make sure caller is sending 1 or more bytes
    if (nbytes < 1)
    {
        return 0;
    }

    // Append register address to front of data packet
    msg[0] = reg;
    for (int i = 0; i < nbytes; i++)
    {
        msg[i + 1] = buf[i];
    }

    // Write data to register(s) over I2C
    i2c_write_blocking(i2c, addr, msg, (nbytes + 1), false);

    return num_bytes_read;
}