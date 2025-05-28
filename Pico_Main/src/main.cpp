#include <Arduino.h>
#include <EEPROM.h>
#include "../include/config.h"
#include <pico/stdlib.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "motion_controller.h"
#include "status_LED.h"
#include "i2c_control.h"

#include "sensors_config.h"

MotionController motionController = MotionController();
StatusLED statusLED = StatusLED();

void setup() {

    Serial.begin(115200);
#ifdef USE_SERIAL_USB
    while (!Serial)
        ;
#endif
    const int interval = 10;
    for (int i = 0; i < 100; ++i) {
        Serial.printf("Launching in %i ms\n", (100 - i) * interval);
        sleep_ms(interval);
    }
    EEPROM.begin(EEPROM_SIZE);

    motionController.initMotionController();
    statusLED.init(NEOPIXEL_PIN);
    Serial.println("Status LED initialized");

    statusLED.SetError();
    delay(1000);

//    Serial.printf("I2C from ESP\n");
    register_i2c_function(reinterpret_cast<i2c_response_t>(&motionCallback), PICO_MOTOR_COMMAND_REGISTER,
                          MOTOR_COMMAND_SIZE);
    register_i2c_function(reinterpret_cast<i2c_response_t>(&testCallback), TEST_CONNECTION_REGISTER,
                          TEST_CONNECTION_SIZE);

//    initPicoPeriph();
//    initPicoController();

//    config_icm42688();
//    config_lis3mdl();
    Serial.printf("Testing?\n");
    statusLED.SetOK();


}

void loop() {

//    read_sensors();
//    delay(50);
    Serial.printf("Looping\n");
//    bus_scan();
//    delay(1000);
//    unsigned long currTime = millis();
    int top = 50;
    int looper = top;
    float theta = 0.01;
    MotionController::setSpeed(theta, PI/2, 0);
    while (theta < 0.1) {
        if (looper == 0) {
            MotionController::setSpeed(theta, PI/2, 0);
            theta += 0.01;
            looper = top;
        }

        MotionController::runPIDUpdate();
        delay(50);
        looper--;
    }
//    Serial.printf("Time taken: %lu\n", millis() - currTime);

}