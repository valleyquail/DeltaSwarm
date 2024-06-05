#include "../../include/config.h"
#include "motor.h"
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

Motor::Motor(uint8_t pwmPinA, uint8_t pwmPinB, uint8_t encoderPinA, uint8_t encoderPinB) {
    // Pin definitions
    pwm_pin_A = pwmPinA;
    pwm_pin_B = pwmPinB;
    encoder_pin_A = encoderPinA;
    encoder_pin_B = encoderPinB;
    prev_state = 0;
    gpio_init(encoder_pin_A);
    gpio_init(encoder_pin_B);
    gpio_set_dir(encoder_pin_A, GPIO_IN);
    gpio_set_dir(encoder_pin_B, GPIO_IN);
    gpio_set_irq_enabled(encoder_pin_A, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(encoder_pin_B, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    this->pwmA = new RP2040_PWM(pwm_pin_A, PWM_FREQ, 0);
    this->pwmB = new RP2040_PWM(pwm_pin_B, PWM_FREQ, 0);
}

void Motor::setPIDVals(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void Motor::setTargetSpeed(int speed) {
    // If there is a new speed sent, update how the encoder values are being kept
    isNewSpeed = true;
    total_encoder_count += curr_movement_encoder_count;
    curr_movement_encoder_count = 0;
    sumError = 0;
    lastError = 0;
    target_speed = speed;
    setSpeed(speed);
#ifdef ENCODER_DEBUG
    Serial.printf("New speed set: %i\n", speed);
    Serial.printf("Total encoder count: %ld\n", total_encoder_count);
#endif
    isNewSpeed = false;
}

void Motor::setSpeed(int speed) {
    //TODO: Get rid of this since this scaling will be taken care of by the PID controller
    speed *= 75;
    // If the speed is 0, stop the motor
    if (speed == 0) {
        pwmA->setPWM_Int(pwm_pin_A, PWM_FREQ, 0);
        pwmB->setPWM_Int(pwm_pin_B, PWM_FREQ, 0);
    }
        // If the speed is negative, set the direction to reverse
    else if (speed < 0) {
        pwmA->setPWM_Int(pwm_pin_A, PWM_FREQ, 0);
        pwmB->setPWM_Int(pwm_pin_B, PWM_FREQ, -speed);
#ifdef ENCODER_DEBUG
        Serial.println("Reversing\n");
#endif
    }
        // If the speed is positive, set the direction to forward
    else {
        pwmA->setPWM_Int(pwm_pin_A, PWM_FREQ, speed);
        pwmB->setPWM_Int(pwm_pin_B, PWM_FREQ, 0);
#ifdef ENCODER_DEBUG
        Serial.println("Forward\n");
#endif
    }
#ifdef ENCODER_DEBUG
    Serial.printf("Speed set: %i\n", speed);
#endif
}

void Motor::brake() {
    pwmA->setPWM(pwm_pin_A, PWM_FREQ, 100);
    pwmB->setPWM(pwm_pin_B, PWM_FREQ, 100);
#ifdef DEBUG
    Serial.println("Braking\n");
#endif
}

// PID Control function
void Motor::updateSpeed() {
#ifdef ENCODER_DEBUG
    if (isNewSpeed) {
        Serial.printf("Mode: %i\n", isNewSpeed);
        Serial.printf("locations of isnewspeed: %p\n", &isNewSpeed);
        return;
    }
#endif

    // Calculate the error
    float error = target_speed - encoderSpeed * 1000 / (TIMER_INTERVAL_MS);
    // Calculate the integral
    if (sumError + error < maxError && sumError + error > minError) {
        sumError += error;
    }
    // Calculate the derivative
    float dError = error - lastError;
    // Calculate the output
    float output = kp * error + ki * sumError + kd * dError;
    // Set the PWM
    setSpeed((int) output);
#if defined(ENCODER_DEBUG) || defined(PICO_USE_USB_SERIAL)
    Serial.printf("Motor on pins: %d %d\n", pwm_pin_A, pwm_pin_B);
    Serial.printf("PID Speed: %d\n", encoderSpeed);
    Serial.printf("Error: %f\n", error);
    Serial.printf("Current encoder counts: %i\n", curr_movement_encoder_count);
#endif
    // Update the last error
    lastError = error;
}