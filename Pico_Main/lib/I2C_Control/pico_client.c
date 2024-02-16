#include "i2c_control.h"
#include "../../include/config.h"
#include "pico/stdlib.h"
#include "../../include/pin_definitions.h"
#include "hardware/i2c.h"

void espRequestHandler(i2c_inst_t *i2c, i2c_slave_event_t event);

void initPicoClient()
{
}

static void setup_slave()
{
    gpio_init(SDA_PIN_1);
    gpio_set_function(SDA_PIN_1, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN_1);

    gpio_init(SCL_PIN_1);
    gpio_set_function(SCL_PIN_1, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN_1);

    i2c_init(i2c0, PICO_ESP_FREQ);
    // configure I2C0 for slave mode
    i2c_slave_init(i2c0, PICO_ADDRESS, &espRequestHandler);
}

void espRequestHandler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
}