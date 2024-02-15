#include "motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

Motor::Motor(uint8_t pwmPinA, uint8_t pwmPinB, uint8_t encoderPinA, uint8_t encoderPinB)
{
    // Pin definitions
    pwm_pin_A = pwmPinA;
    pwm_pin_B = pwmPinB;
    encoder_pin_A = encoderPinA;
    encoder_pin_B = encoderPinB;
    gpio_set_irq_enabled(encoder_pin_A, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(encoder_pin_B, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    this->pwmA = new RP2040_PWM(pwm_pin_A, PWM_FREQ, 0);
    this->pwmB = new RP2040_PWM(pwm_pin_B, PWM_FREQ, 0);
#ifdef DEBUG
    printf("Motor created\n");
#endif
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
    // If the speed is 0, stop the motor
    if (speed == 0)
    {
        pwmA->setPWM(pwm_pin_A, PWM_FREQ, 0);
        pwmB->setPWM(pwm_pin_B, PWM_FREQ, 0);
        return;
    }

    curr_movement_encoder_count = 0;
    controlMode mode = SET_NEW_SPEED; // Set the mode to set new speed
}

// TODO Implement this function and fix the copilot garbage
void Motor::brake()
{
    pwmA->setPWM(pwm_pin_A, PWM_FREQ, 100);
    pwmB->setPWM(pwm_pin_B, PWM_FREQ, 100);
#ifdef DEBUG
    printf("Braking\n");
#endif
}

// PID Control function
void Motor::updateSpeed()
{
    // Calculate the error
    float error = target_speed - encoderSpeed;
    // Calculate the integral
    sumError += error;
    // Calculate the derivative
    float dError = error - lastError;
    // Calculate the output
    float output = kp * error + ki * sumError + kd * dError;
    // Set the PWM
    setPWM(output);
#ifdef DEBUG
    printf("Speed: %d\n", encoderSpeed);
    printf("Error: %f\n", error);
#endif
    // Update the last error
    lastError = error;
}