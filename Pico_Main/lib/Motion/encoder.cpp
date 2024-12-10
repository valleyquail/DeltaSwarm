#include "motor.h"
#include <Arduino.h>
#include "../../include/config.h"
#include "pico/stdlib.h"

// Motors are defined in the motion_controller.cpp file for the drive motors
// the other motors will be defined later in what will likely be an actuator
// class. Actuator motors will likely be a child class of the Motor class since
// they will have additional functions



void gpio_callback(uint gpio, uint32_t events) {
    (void) events;
    // Put the GPIO event(s) that just happened into event_str,
    // so we can print it
#ifdef ENCODER_DEBUG
    Serial.printf("GPIO %d had an event\n", gpio);
#endif
    switch (gpio) {
        case MOTOR1_A_ENC:
            encoderInterruptA(&motor1);
            break;
        case MOTOR1_B_ENC:
            encoderInterruptB(&motor1);
            break;
        case MOTOR2_A_ENC:
            encoderInterruptA(&motor2);
            break;
        case MOTOR2_B_ENC:
            encoderInterruptB(&motor2);
            break;
        case MOTOR3_A_ENC:
            encoderInterruptA(&motor3);
            break;
        case MOTOR3_B_ENC:
            encoderInterruptB(&motor3);
            break;
        case MOTOR4_A_ENC:
            /* code */
            break;
        case MOTOR4_B_ENC:
            /* code */
            break;
        case MOTOR5_A_ENC:
            /* code */
            break;
        case MOTOR5_B_ENC:
            /* code */
            break;
        case MOTOR6_A_ENC:
            /* code */
            break;
        case MOTOR6_B_ENC:
            /* code */
            break;
        default:
            break;
    }
}

void calcEncoderDelta(void *motor_instance) {
    auto *motor = static_cast<Motor *>(motor_instance);
    motor->encoderSpeed = (motor->curr_movement_encoder_count - motor->prevCount);
    motor->prevCount = motor->curr_movement_encoder_count;
    if (motor->target_speed < 0)
        motor->encoderSpeed *= -1;
}

void encoderInterruptA(void *motor_instance) {
    auto *motor = static_cast<Motor *>(motor_instance);
    bool pinA = gpio_get(motor->encoder_pin_A);
    bool pinB = gpio_get(motor->encoder_pin_B);
    if (pinA == pinB) {
        motor->curr_movement_encoder_count++;
    } else {
        motor->curr_movement_encoder_count--;
    }

#ifdef ENCODER_DEBUG_2
    Serial.printf("Encoder count A for %i: %d\n", motor->encoder_pin_A, motor->curr_movement_encoder_count);
#endif
}

void encoderInterruptB(void *motor_instance) {
    auto *motor = static_cast<Motor *>(motor_instance);
    bool pinA = gpio_get(motor->encoder_pin_A);
    bool pinB = gpio_get(motor->encoder_pin_B);
    if (pinA == pinB) {
        motor->curr_movement_encoder_count--;
    } else {
        motor->curr_movement_encoder_count++;
    }
#ifdef ENCODER_DEBUG_2
    Serial.printf("Encoder count B for %i: %d\n", motor->encoder_pin_B, motor->curr_movement_encoder_count);
#endif
}