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
//    encoder_pin_B = encoderPinB;
//
//    this->pwmA = new RP2040_PWM(pwm_pin_A, PWM_FREQ, 0);
//    this->pwmB = new RP2040_PWM(pwm_pin_B, PWM_FREQ, 0);
//    uint16_t PWM_TOP_A = this->pwmA->get_TOP();
//    uint16_t PWM_DIV_A = this->pwmA->get_DIV();
//    uint16_t PWM_TOP_B = this->pwmB->get_TOP();
//    uint16_t PWM_DIV_B = this->pwmB->get_DIV();
//    uint16_t PWM_Level = 0;
//
//    // setPWM_manual(uint8_t pin, uint16_t top, uint8_t div, uint16_t level, bool phaseCorrect = false)
//    this->pwmA->setPWM_manual(pwmPinA, PWM_TOP_A, PWM_DIV_A, PWM_Level, true);
//    this->pwmB->setPWM_manual(pwmPinB, PWM_TOP_B, PWM_DIV_B, PWM_Level, true);
}

void Motor::initIRQ(){
    gpio_init(encoder_pin_A);
    gpio_init(encoder_pin_B);
    gpio_set_dir(encoder_pin_A, GPIO_IN);
    gpio_set_dir(encoder_pin_B, GPIO_IN);
    gpio_set_irq_enabled(encoder_pin_A, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(encoder_pin_B, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);


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

float DC_zero = 0;

void Motor::setSpeed(int speed) {
    auto duty_cycle_percent = (float) abs(speed);
    // If the speed is 0, stop the motor
    if (speed == 0) {
        pwmA->setPWM_DCPercentage_manual(pwm_pin_A, DC_zero);
        pwmB->setPWM_DCPercentage_manual(pwm_pin_B, DC_zero);
    }
        // If the speed is negative, set the direction to reverse
    else if (speed < 0) {
        pwmA->setPWM_DCPercentage_manual(pwm_pin_A, DC_zero);
        pwmB->setPWM_DCPercentage_manual(pwm_pin_B, duty_cycle_percent);
#ifdef ENCODER_DEBUG
        Serial.println("Reversing\n");
#endif
    }
        // If the speed is positive, set the direction to forward
    else {
        pwmA->setPWM_DCPercentage_manual(pwm_pin_A, duty_cycle_percent);
        pwmB->setPWM_DCPercentage_manual(pwm_pin_B, DC_zero);
#ifdef ENCODER_DEBUG
        Serial.println("Forward\n");
#endif
    }
#ifdef SPEED_DEBUG
    Serial.printf("Duty cycle set: %i\n", speed);
#endif
}

void Motor::brake() {
    float brake = 100;
    pwmA->setPWM_DCPercentage_manual(pwm_pin_A, brake);
    pwmB->setPWM_DCPercentage_manual(pwm_pin_B, brake);
#ifdef DEBUG
    Serial.println("Braking\n");
#endif
}

// PID Control function
void Motor::updateSpeed() {
    int error, dError, output;
    if (target_speed == 0 || isNewSpeed) {
        return;
    }
    // Calculate the error
    // Also adapt the error to be in the timer interval
    error = target_speed - encoderSpeed * 1000 / (TIMER_INTERVAL_MS);
    // Calculate the integral
    sumError += error;
    constrain(sumError, -maxError, maxError);
    // Calculate the derivative
    dError = error - lastError;
    // Calculate the output
    output = constrain((int)(kp * error + ki * sumError + kd * dError), -100, 100);
    // Set the PWM
    setSpeed(output);

#ifndef PID_DEBUG
#define PID_DEBUG 1
#endif

#if defined(PID_DEBUG)
    Serial.printf("Motor on pins: %d %d\n", pwm_pin_A, pwm_pin_B);
    Serial.printf("PID Speed: %i\n", output);
    Serial.printf("Raw Error: %i;Sum %i; dError %i\n", error, sumError, dError);
    Serial.printf("PID Error: %f;Sum %f; dError %f\n\n", error * kp, sumError * ki, dError * kd);
    //    Serial.printf("Current encoder counts: %i\n\n", curr_movemen
    //    t_encoder_count);
#endif
// Update the last error
    lastError = error;
}

int Motor::getCurrEncoderCount() const {
    return curr_movement_encoder_count;
}

int Motor::getEncoderSpeed() const {
    return encoderSpeed;
}

int Motor::getTargetSpeed() const {
    return target_speed;
}
