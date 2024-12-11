#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "pico/stdlib.h"
#include "RP2040_PWM.h"
#include "quad_substep.h"

#ifndef __MOTOR_H__
#define __MOTOR_H__

// Deadband for the motor PWM
#define DEADBAND_END 60


// TODO: Maybe make a child class for drive motors vs actuator motors so that
// there can be functionality to change the PWM frequency of the motor for
// different speed control responses?
//TODO: Need to add a lot of logic to handle the pulses for short movements since there is a large amount of
// static friction within the motor that makes it difficult to move small amounts
class Motor {
private:
#ifndef USE_ENCODER_INTERRUPTS
    //PIO variables: DO NOT TOUCH
    substep_state_t *enc_state;
    uint block_num;
#endif
protected:
    // 20kHz PWM frequency
    const float PWM_FREQ = 20000.;

    // True if it is a new target speed
    volatile bool isNewSpeed = true;
    // PWM pin A and B, and encoder pin A and B for controlling the motor and
    // getting encoder counts
    uint8_t pwm_pin_A;
    uint8_t pwm_pin_B;
    uint8_t encoder_pin_A;
    uint8_t encoder_pin_B;
    RP2040_PWM *pwmA;
    RP2040_PWM *pwmB;

    // Stores the encoder count since the program started
    int total_encoder_count = 0;
    // Stores the encoder count since the last movement
    // Resets everytime there is a new movement

    // keeps track of encoder changes
    volatile int prevCount = 0;
    volatile int curr_movement_encoder_count = 0;

    // PID control variables
    float kp;
    float ki;
    float kd;

    // Encoder count within the timer interval
    // TODO Determine the timer interval
    volatile int encoderSpeed = 0;

    // Keep track of target speed in terms of encoder ticks per timer interval
    int target_speed = 0;
    // Integral control
    int sumError = 0;
    // Derivative control
    int lastError = 0;
    // Max error for integral control to prevent windup
    const int maxError = 10000;

    void setSpeed(int speed);

public:
#ifdef USE_ENCODER_INTERRUPTS
    Motor(uint8_t pwm_in_A, uint8_t pwm_in_B, uint8_t encoder_pin_A, uint8_t encoder_pin_B);

    void initIRQ();
#else
    Motor();
    Motor(uint8_t pwm_in_A, uint8_t pwm_in_B, uint8_t encoder_pin_A, uint8_t encoder_pin_B, substep_state_t *state, uint block);
#endif
    void setPIDVals(float kp, float ki, float kd);

    void updateSpeed();

    /**
     * @brief Sets a new speed for the motor as compared to using PID to match the
     * desired speed
     *
     * @param speed
     */
    void setTargetSpeed(int speed);

    /**
     * @brief Sets both PWM channels high so that the motors have some holding force
     */
    void brake();

    [[nodiscard]] int getCurrEncoderCount() const;

    /**
     * @brief Get the Encoder Speed object
     * @return the encoder delta in the timer interval
     */
    [[nodiscard]] int getEncoderSpeed() const;
    /**
     * @brief Calculate the change in encoder count since the last measurement
     * Used to calculate the speed of the motor in terms of encoder counts. It
     * has to be defined strangely because of the C++ linkage errors with C code.
     *
     */
    friend void calcEncoderDelta(void *motor_instance);

    /**
     * @brief This is the ISR for the encoder. It has to be defined strangely
     * because of the C++ linkage errors with C code.
     *
     */
    friend void encoderInterruptA(void *motor_instance);

    friend void encoderInterruptB(void *motor_instance);

    [[nodiscard]] int getTargetSpeed() const;

};

extern Motor motor1;
extern Motor motor2;
extern Motor motor3;

#endif