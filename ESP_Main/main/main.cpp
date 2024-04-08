#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../include/config.h"
#include "../lib/Robot/Robot.h"
#include "../lib/Status_LED/status_leds.h"

// #include <rmw_microros/rmw_microros.h>
#include <uros_network_interfaces.h>

StatusLED statusLED = StatusLED(NEOPIXEL_PIN);
Robot robot = Robot(ROBOT_NUM);

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting up");
    statusLED.SetWarning();
    delay(1000);

    bool init_successful = robot.init();
    if (!init_successful)
        statusLED.SetError();
    else
        statusLED.SetOK();

    ESP_ERROR_CHECK(uros_network_interface_initialize());
}

void loop() {}
