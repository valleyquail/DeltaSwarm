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
StatusLED statusLED = StatusLED();

void setup() {

    Serial.begin(115200);

    const int interval = 10;
    for (int i = 0; i < 100; ++i) {
        Serial.printf("Launching in %i ms\n", (100 - i) * interval);
        sleep_ms(interval);
    }
    /*substep_state_t state;

    // base pin to connect the A phase of the encoder. the B phase must be
    // connected to the next pin
    uint PIN_A = 20;


    Serial.printf("Hello from quadrature encoder substep\n");

    PIO pio = pio0;
    uint sm = 0;

    pio_add_program(pio, &quadrature_encoder_substep_program);
    substep_init_state(pio, sm, MOTOR1_A_ENC, &state);



    // replace this with the output of the calibration function
    substep_set_calibration_data(&state, 64, 128, 192);

    uint last_position = 0;
    int last_speed = 0;
    uint last_raw_step = 0;
    while (1) {

        // read the PIO and update the state data
        substep_update(&state);

        if (last_position != state.position || last_speed != state.speed || last_raw_step != state.raw_step) {
            // print out the result
            Serial.printf("pos: %-10d  speed: %-10d  raw_steps: %-10d\n", state.position, state.speed, state.raw_step);
            last_position = state.position;
            last_speed = state.speed;
            last_raw_step = state.raw_step;
        }
        // run at roughly 100Hz
        sleep_ms(10);
    }
*/

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