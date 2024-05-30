//
// Created by nikesh on 5/24/24.
//
#include <Arduino.h>
#include "../../include/config.h"
#include "i2c_control.h"
#include "hardware/i2c.h"

void initPicoController()
{
    gpio_init(SDA_PIN_0);
    gpio_set_function(SDA_PIN_0, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN_0);

    gpio_init(SCL_PIN_0);
    gpio_set_function(SCL_PIN_0, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN_0);

    i2c_init(i2c1, PICO_I2C_FREQ);
    Serial.printf("initialized the pico as a master\n");
}