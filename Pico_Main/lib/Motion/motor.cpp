#include "../../include/config.h"
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
}

void Motor::setPIDVals(float kp, float ki, float kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void Motor::setTargetSpeed(int speed)
{
    // If there is a new speed sent, update how the encoder values are being kept
    isNewSpeed = true;
    total_encoder_count += curr_movement_encoder_count;
    curr_movement_encoder_count = 0;
    sumError = 0;
    lastError = 0;
    target_speed = speed;
#ifdef DEBUG
    Serial.printf("New speed set: %i\n", speed);
    Serial.printf("Total encoder count: %i\n", total_encoder_count);
#endif
    isNewSpeed = false;
    Serial.printf("target function locations of isnewspeed: %p\n", &isNewSpeed);
}

void Motor::setSpeed(int speed)
{
    // If the speed is 0, stop the motor
    if (speed == 0)
    {
        pwmA->setPWM(pwm_pin_A, PWM_FREQ, 0);
        pwmB->setPWM(pwm_pin_B, PWM_FREQ, 0);
        return;
    }
    // If the speed is negative, set the direction to reverse
    if (speed < 0)
    {
        pwmA->setPWM(pwm_pin_A, PWM_FREQ, 0);
        pwmB->setPWM(pwm_pin_B, PWM_FREQ, abs(speed));
    }
    // If the speed is positive, set the direction to forward
    else
    {
        pwmA->setPWM(pwm_pin_A, PWM_FREQ, abs(speed));
        pwmB->setPWM(pwm_pin_B, PWM_FREQ, 0);
    }
#ifdef DEBUG
    Serial.printf("Speed set: %i\n", speed);
#endif
}

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
    if (isNewSpeed)
    {
        Serial.printf("Mode: %i\n", isNewSpeed);
        Serial.printf("locations of isnewspeed: %p\n", &isNewSpeed);
        return;
    }

    // Calculate the error
    float error = target_speed - encoderSpeed / (TIMER_INTERVAL_MS * 1000);
    // Calculate the integral
    if (sumError + error < maxError && sumError + error > minError)
    {
        sumError += error;
    }
    // Calculate the derivative
    float dError = error - lastError;
    // Calculate the output
    float output = kp * error + ki * sumError + kd * dError;
    // Set the PWM
    setSpeed(output);
#if defined(DEBUG) || defined(PICO_USE_USB_SERIAL)
    Serial.printf("Motor on pins: %d\t%d\n", pwm_pin_A, pwm_pin_B);
    Serial.printf("PID Speed: %d\n", encoderSpeed);
    Serial.printf("Error: %f\n", error);
    Serial.printf("Current encoder counts: %i\n\n", curr_movement_encoder_count);

#endif
    // Update the last error
    lastError = error;
}