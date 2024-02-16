#include "motor.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include "RPi_Pico_TimerInterrupt.h"
#include "../../include/config.h"
#include "../../include/pin_definitions.h"
#include "pico/stdlib.h"

// Motors are defined in the motion_controller.cpp file for the drive motors
// the other motors will be defined later in what will likely be an actuator
// class. Actuator motors will likely be a child class of the Motor class since
// they will have additional functions

extern Motor motor1;
extern Motor motor2;
extern Motor motor3;

void encoderInterruptA(void *motor_instance);
void encoderInterruptB(void *motor_instance);

void gpio_callback(uint gpio, uint32_t events)
{
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
    switch (gpio)
    {
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
#ifdef DEBUG
    printf("GPIO %d had an event\n", gpio);
#endif
}

void calcEncoderDelta(void *motor_instance)
{
    auto *motor = static_cast<Motor *>(motor_instance);
    motor->encoderSpeed = (motor->curr_movement_encoder_count - motor->prevCount);
    motor->prevCount = motor->curr_movement_encoder_count;
}

void encoderInterruptA(void *motor_instance)
{
    auto *motor = static_cast<Motor *>(motor_instance);
    bool pinA = gpio_is_pulled_up(motor->encoder_pin_A);
    bool pinB = gpio_is_pulled_up(motor->encoder_pin_B);
    if (pinA == pinB)
    {
        motor->curr_movement_encoder_count++;
    }
    else
    {
        motor->curr_movement_encoder_count--;
    }
}

void encoderInterruptB(void *motor_instance)
{
    auto *motor = static_cast<Motor *>(motor_instance);
    bool pinA = gpio_is_pulled_up(motor->encoder_pin_A);
    bool pinB = gpio_is_pulled_up(motor->encoder_pin_B);
    if (pinA == pinB)
    {
        motor->curr_movement_encoder_count--;
    }
    else
    {
        motor->curr_movement_encoder_count++;
    }
}