#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"

#include "../lib/Status_LED/status_leds.h"

#include "../include/config.h"
StatusLED statusLED = StatusLED(NEOPIXEL_PIN);

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting up");

    statusLED.SetWarning();
}

void loop()
{
}