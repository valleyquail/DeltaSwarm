#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include <Arduino.h>
#include "../lib/Status_LED/status_leds.h"

void init_esp();
StatusLED statusLED(2);

extern "C" void app_main(void)
{
    initArduino();
    statusLED.SetOK();
}

void init_esp()
{
    return;
}