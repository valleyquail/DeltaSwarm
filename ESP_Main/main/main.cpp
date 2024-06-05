#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include <uros_network_interfaces.h>
#include "../include/config.h"
#include "../lib/Robot/Robot.h"
#include "../lib/Status_LED/status_leds.h"
#include "esp_err.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

StatusLED statusLED = StatusLED(NEOPIXEL_PIN);
Robot robot = Robot(ROBOT_NUM);
uint8_t hardcoded_mac_address[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() {
    Serial.begin(115200);
    printf("Starting up\n");
    statusLED.SetWarning();
    delay(1000);
    esp_base_mac_addr_get(hardcoded_mac_address);
    printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", hardcoded_mac_address[0], hardcoded_mac_address[1],
           hardcoded_mac_address[2], hardcoded_mac_address[3], hardcoded_mac_address[4],
           hardcoded_mac_address[5]);
    bool init_successful = robot.init();
    if (!init_successful)
        statusLED.SetError();
    else
        statusLED.SetOK();
    delay(1000);
    ESP_ERROR_CHECK(uros_network_interface_initialize());

}

void loop() {
    Serial.println("alive");
    delay(5000);
}
