#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include <Arduino.h>

void init_esp();

extern "C" void app_main(void)
{
    initArduino();
}

void init_esp()
{
    return;
}