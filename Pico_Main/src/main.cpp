#include <Arduino.h>
#include "../include/config.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "../lib/Motion/motion_controller.h"
#include "status_LED.h"
#include <pico/stdlib.h>
#include "../lib/I2C_Control/i2c_control.h"

#include "../lib/Testing/serial_debugger.h"
#include "../lib/Odometry/sensors_config.h"

MotionController motionController = MotionController();
StatusLED statusLED = StatusLED(NEOPIXEL_PIN);

void setup() {

    Serial.begin(115200);

    const int interval = 10;
//    for (int i = 0; i < 100; ++i) {
//        Serial.printf("Launching in %i ms\n", (100 - i) * interval);
//        sleep_ms(interval);
//    }
    statusLED.SetError();

    Serial.printf("I2C from ESP\n");
    register_i2c_function(reinterpret_cast<i2c_response_t>(&motionCallback), PICO_MOTOR_COMMAND_REGISTER,
                          MOTOR_COMMAND_SIZE);
    register_i2c_function(reinterpret_cast<i2c_response_t>(&testCallback), TEST_CONNECTION_REGISTER,
                          TEST_CONNECTION_SIZE);

    initPicoPeriph();

    statusLED.SetWarning();


    initPicoController();

//    config_icm42688();
//    config_lis3mdl();
    Serial.printf("Testing?\n");

    statusLED.SetOK();

    delay(1000);
//    for (int i = 20000; i < 200000; i+=100) {
//        motor1.setTargetSpeed(i);
//        delay(10);
//        int currSpeed = motor1.getEncoderSpeed();
//        if (currSpeed > 0){
//            Serial.printf("Deadband ended at %i\n", i);
//            break;
//        }
//        Serial.println(i);
//    }

//MotionController::setSpeed(1, 1, 0);
//    motionController.runPIDUpdate();
//    motionController.debugMotorSpeeds();


}

void loop() {

//    read_sensors();
    delay(1000);
    Serial.printf("Looping\n");
//    bus_scan();
//    delay(1000);
//    unsigned long currTime = millis();
//    for (int i = 0; i < 100; i++) {
//        motionController.runPIDUpdate();
//        motionController.debugMotorSpeeds();
//        delay(10);
//    }
//    Serial.printf("Time taken: %lu\n", millis() - currTime);

}
