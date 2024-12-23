#include "../../include/config.h"
#include "motor.h"
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "quad_substep.h"

constexpr int DEADBAND_END = 40;
#ifdef USE_ENCODER_INTERRUPTS
Motor::Motor(uint8_t pwmPinA, uint8_t pwmPinB, uint8_t encoderPinA, uint8_t encoderPinB) {
    // Pin definitions
    pwm_pin_A = pwmPinA;
    pwm_pin_B = pwmPinB;
    encoder_pin_A = encoderPinA;
    encoder_pin_B = encoderPinB;

    this->pwmA = new RP2040_PWM(pwm_pin_A, PWM_FREQ, 0);
    this->pwmB = new RP2040_PWM(pwm_pin_B, PWM_FREQ, 0);
    uint16_t PWM_TOP_A = this->pwmA->get_TOP();
    uint16_t PWM_DIV_A = this->pwmA->get_DIV();
    uint16_t PWM_TOP_B = this->pwmB->get_TOP();
    uint16_t PWM_DIV_B = this->pwmB->get_DIV();
    uint16_t PWM_Level = 0;

    // setPWM_manual(uint8_t pin, uint16_t top, uint8_t div, uint16_t level, bool phaseCorrect = false)
    this->pwmA->setPWM_manual(pwmPinA, PWM_TOP_A, PWM_DIV_A, PWM_Level, true);
    this->pwmB->setPWM_manual(pwmPinB, PWM_TOP_B, PWM_DIV_B, PWM_Level, true);
}

void Motor::initIRQ(){
    gpio_init(encoder_pin_A);
    gpio_init(encoder_pin_B);
    gpio_set_dir(encoder_pin_A, GPIO_IN);
    gpio_set_dir(encoder_pin_B, GPIO_IN);
    gpio_set_irq_enabled(encoder_pin_A, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(encoder_pin_B, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
}
#else

constexpr int substeps_per_rev = 256;

Motor::Motor() {
    // Default constructor for the motor to initialize the motor without the encoder interrupts
}

void Motor::initMotor(uint8_t pwmPinA, uint8_t pwmPinB, uint8_t encoderPinA, uint8_t encoderPinB,
                      substep_state_t *encoder_state,
                      const int *calibration_array) {
    pwm_pin_A = pwmPinA;
    pwm_pin_B = pwmPinB;
    encoder_pin_A = encoderPinA;
    encoder_pin_B = encoderPinB;
    pwmA = new RP2040_PWM(pwm_pin_A, PWM_FREQ, 0);
    pwmB = new RP2040_PWM(pwm_pin_B, PWM_FREQ, 0);
    uint16_t PWM_TOP_A = pwmA->get_TOP();
    uint16_t PWM_DIV_A = pwmA->get_DIV();
    uint16_t PWM_TOP_B = pwmB->get_TOP();
    uint16_t PWM_DIV_B = pwmB->get_DIV();
    uint16_t PWM_Level = 0;

    // setPWM_manual(uint8_t pin, uint16_t top, uint8_t div, uint16_t level, bool phaseCorrect = false)
    pwmA->setPWM_manual(pwmPinA, PWM_TOP_A, PWM_DIV_A, PWM_Level, true);
    pwmB->setPWM_manual(pwmPinB, PWM_TOP_B, PWM_DIV_B, PWM_Level, true);

    state = encoder_state;
    substep_init_state(state->pio, state->sm, encoder_pin_A, state);
    substep_set_calibration_data(state, calibration_array[0], calibration_array[1], calibration_array[2]);
}

#endif

void Motor::setPIDVals(float kp, float ki, float kd) {
#ifndef USE_ENCODER_INTERRUPTS
//divide by the substeps per step since the encoder speed is much larger now
    kp /= substeps_per_rev;
    ki /= substeps_per_rev;
    kd /= substeps_per_rev;
#endif
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void Motor::setTargetSpeed(int speed) {
#ifndef USE_ENCODER_INTERRUPTS
    // Convert the encoder speed to now be in terms of substeps per timer interval
    speed *= substeps_per_rev;
    prev_count = (int) state->position;
#else
    prev_count = 0;
#endif
    // If there is a new speed sent, update how the encoder values are being kept
    isNewSpeed = true;
    total_encoder_count += curr_movement_encoder_count;

    curr_movement_encoder_count = 0;
    sum_error = 0;
    last_error = 0;
    target_speed = speed;
    setSpeed((float)speed);
#ifdef ENCODER_DEBUG
    Serial.printf("New speed set: %i\n", speed);
    Serial.printf("Total encoder count: %ld\n", total_encoder_count);
#endif
}

float DC_zero = 0;

void Motor::setSpeed(float speed) {
    //TODO: Approximate the duty cycle percentage based on the encoder speed

    float duty_cycle_percent =  abs(speed);
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
    Serial.printf("Duty cycle set: %f\n", speed);
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

constexpr float alpha2 = 0.7;
// PID Control function
void Motor::updateSpeed() {
    int error, dError;
    float output;
    // If the target speed is 0, don't run the PID control
    if (target_speed == 0) {
        Serial.printf("zero\n");
        return;
    }
#ifdef USE_ENCODER_INTERRUPTS
    error = target_speed - encoder_speed * 1000 / (TIMER_INTERVAL_MS);
#else
    // Get the encoder speed in terms of substeps per timer interval
    // PIO program substep counts are reversed
    encoder_speed = -state->speed;
    error = target_speed - encoder_speed;
#endif
    // Calculate the error
    // Also adapt the error to be in the timer interval

    // Calculate the integral
    sum_error += error;
    if (target_speed > 0)
        sum_error = constrain(sum_error, 0, max_error);
    else
        sum_error = constrain(sum_error, -max_error, 0);
    // Calculate the derivative
    dError = error - last_error;
    dError = dError * (1.f - 0.1f) + prev_dError * 0.1f;  // Exponential smoothing
//    prev_dError = dError;
    // Calculate the output
    output = kp * error + ki * sum_error + kd * dError;
    if (target_speed > 0)
        output = constrain(output, DEADBAND_END, 100);
    else
        output = constrain(output, -100, -DEADBAND_END);
    // Set the PWM
    output = (alpha2 * last_pid_output) + ((1-alpha2) * output);
    setSpeed(output);
    last_pid_output = output;
    // Update the last error
    last_error = error;

#ifdef PID_DEBUG
    Serial.printf("Motor on pin: %d\n", pwm_pin_A);
    Serial.printf("Speed: %i, Target: %i\n", encoder_speed, target_speed);
    Serial.printf("Raw Error: %i;Sum %i; dError %i\n", error, sum_error, dError);
    Serial.printf("PID Output: %f\n", output);
#endif

}

int Motor::getCurrEncoderCount() const {
    return curr_movement_encoder_count;
}

int Motor::getEncoderSpeed() const {
#ifdef USE_ENCODER_INTERRUPTS
    return encoder_speed;
#else
    return state->speed;
#endif
}

int Motor::getTargetSpeed() const {
    return target_speed;
}
