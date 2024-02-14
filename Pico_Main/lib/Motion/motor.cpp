#include "motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

Motor::Motor(uint8_t pwmPinA, uint8_t pwmPinB, uint8_t encoderPinA, uint8_t encoderPinB)
{
    // Pin definitions
    pwm_in_A = pwmPinA;
    pwm_in_B = pwmPinB;
    encoder_pin_A = encoderPinA;
    encoder_pin_B = encoderPinB;
}

void Motor::setPIDVals(float kp, float ki, float kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

// TODO Implement this function and fix the copilot garbage
void Motor::setSpeed(float speed)
{
    curr_movement_encoder_count = 0;
    controlMode mode = SET_NEW_SPEED; // Set the mode to set new speed
    if (speed > 0)
    {
        gpio_put(this->pwm_in_A, 1);
        gpio_put(this->pwm_in_B, 0);
    }
    else
    {
        gpio_put(this->pwm_in_A, 0);
        gpio_put(this->pwm_in_B, 1);
    }
    pwm_set_gpio_level(this->pwm_in_A, abs(speed));
}

// TODO Implement this function and fix the copilot garbage
void Motor::brake()
{
    gpio_put(this->pwm_in_A, 1);
    gpio_put(this->pwm_in_B, 1);
    pwm_set_gpio_level(this->pwm_in_A, 0);
}