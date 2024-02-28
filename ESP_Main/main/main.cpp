#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"

#include "../lib/Status_LED/status_leds.h"
#include "../lib/Communication_Protocols/I2C.h"
#include "../include/config.h"
StatusLED statusLED = StatusLED(NEOPIXEL_PIN);
Pico_I2C pico_i2c = Pico_I2C();

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting up");
    statusLED.SetWarning();
    delay(1000);
    pico_i2c.init();
}

void loop()
{
}