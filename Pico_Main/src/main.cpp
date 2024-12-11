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
#include "../lib/Motion/quad_substep.h"
#include "../lib/Motion/quadrature_substep_pio.pio.h"


MotionController motionController = MotionController();
StatusLED *statusLED;

void setup() {

    Serial.begin(115200);

    const int interval = 10;
    for (int i = 0; i < 100; ++i) {
        Serial.printf("Launching in %i ms\n", (100 - i) * interval);
        sleep_ms(interval);
    }
    int PIN_A = MOTOR2_A_ENC;
    delay(1000);

    substep_state_t state;
    PIO pio = pio0;
    int sm = 0;
    //Set all of pio0 state machines to be enables

    pio_add_program(pio, &quadrature_encoder_substep_program);
    substep_init_state(pio, sm, PIN_A, &state);
    Serial.printf("here\n");
    pio_claim_sm_mask(pio, 0b1111);
    Serial.printf("State machines on pio0: %i\n", pio_claim_unused_sm(pio, false));
    statusLED = (new StatusLED(NEOPIXEL_PIN));
    Serial.println("Status LED initialized");

    statusLED->SetError();
    delay(1000);

//    Serial.printf("I2C from ESP\n");
//    register_i2c_function(reinterpret_cast<i2c_response_t>(&motionCallback), PICO_MOTOR_COMMAND_REGISTER,
//                          MOTOR_COMMAND_SIZE);
//    register_i2c_function(reinterpret_cast<i2c_response_t>(&testCallback), TEST_CONNECTION_REGISTER,
//                          TEST_CONNECTION_SIZE);

//    initPicoPeriph();
//    initPicoController();

//    config_icm42688();
//    config_lis3mdl();
    Serial.printf("Testing?\n");
    statusLED->SetOK();

    Serial.printf("Hello from quadrature encoder substep\n");
    init_pwm();
    Serial.println("PWM initialized");
    set_pwm(0.9);
    // - wait for the motor to reach a reasonably stable speed
    sleep_ms(2000);
    // - run the phase size calibration code
    Serial.printf("Calibrating\n");
    substep_calibrate_phases(pio, sm);
    Serial.printf("Calibrated\n");
    // - stop the motor
    set_pwm(0);

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