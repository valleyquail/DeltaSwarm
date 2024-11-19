
#include <stdio.h>
#include <stdlib.h>

#include <uros_network_interfaces.h>
#include "../../lib/Communication_Protocols/i2c_management.h"
#include "../include/config.h"
#include "../lib/ROS_stuff/ROS_config.h"
#include "../lib/Robot/Robot.h"
#include "../lib/Status_LED/status_leds.h"
#include "esp_err.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>

uint8_t hardcoded_mac_address[6] = {0x48, 0xCA, 0x43, 0x09, 0x59, 0x6C};

Robot robot(ROBOT_NUM);

extern "C" void app_main(void) {
    for (int i = 0; i < 100; ++i) {
        printf("Launching in %i ms\n", (100 - i) * 20);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    printf("Starting up\n");
    initStatusLED(NEOPIXEL_PIN);

    statusLEDSetWarning();
    SetBatteryLEDColor(30);
    //    esp_base_mac_addr_get(hardcoded_mac_address);
    //    printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", hardcoded_mac_address[0], hardcoded_mac_address[1],
    //           hardcoded_mac_address[2], hardcoded_mac_address[3], hardcoded_mac_address[4],
    //           hardcoded_mac_address[5]);

    // Wait for the pico to boot properly
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // #if defined(CONFIG_MICRO_ROS_ESP_NETIF_WLAN) || defined(CONFIG_MICRO_ROS_ESP_NETIF_ENET)
    //     ESP_ERROR_CHECK(uros_network_interface_initialize());
    // #endif

    //    xLaunchROSNode(nullptr);
    statusLEDSetError();
    while (!pico_i2c_init()) {

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    printf("I2C initialized\n");
    while (!test_pico_connection()) {
        statusLEDSetError();
        vTaskDelay(500 / portTICK_PERIOD_MS);
        SetStatusLEDOff();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    statusLEDSetOK();
    printf("I2C confirmed\n");
    //    ESP_ERROR_CHECK(uros_network_interface_initialize());

    bool flip_speed = true;
    for (;;) {
        printf("Still alive\n");

        if (flip_speed) {
            statusLEDSetOK();
            picoSendMovement(0.5, M_PI_2, 0, true);

            printf("Sent movement flipped\n");
        } else {
            statusLEDSetWarning();
            picoSendMovement(-0.5, M_PI_2, 0, true);
            printf("Sent movement unflipped\n");
        }
        flip_speed = !flip_speed;
        vTaskDelay(5000 / portTICK_PERIOD_MS);

    };
}
