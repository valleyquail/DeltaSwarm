#ifndef MOTOR_H
#define MOTOR_H


#include <cstdio>
#include <cstdlib>
#include <Arduino.h>
#include "pico/stdlib.h"
#include "RP2040_PWM.h"
#include "quad_substep.h"
#include "motion_controller.h"
// Deadband for the motor PWM



// TODO: Maybe make a child class for drive motors vs actuator motors so that
// there can be functionality to change the PWM frequency of the motor for
// different speed control responses?
//TODO: Need to add a lot of logic to handle the pulses for short movements since there is a large amount of
// static friction within the motor that makes it difficult to move small amounts
class Motor {
    friend class MotionController;

private:
#ifndef USE_ENCODER_INTERRUPTS

//PIO variables: DO NOT TOUCH
    substep_state_t *state;
#endif

protected:
    // 20kHz PWM frequency
    const float PWM_FREQ = 20000.;

    // True if it is a new target speed
    bool isNewSpeed = true;
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
    int prev_count = 0;
    int curr_movement_encoder_count = 0;

    // PID control variables
    float kp;
    float ki;
    float kd;

    // Encoder count within the timer interval
    volatile int encoder_speed = 0;

    // Keep track of target speed in terms of encoder ticks per timer interval
    int target_speed = 0;
    // Integral control
    int sum_error = 0;
    // Derivative control
    int last_error = 0;
    int prev_dError = 0;
    float last_pid_output = 0;
    // Max error for integral control to prevent windup
    const int max_error = 40000;

    void setSpeed(float speed);

public:
#ifdef USE_ENCODER_INTERRUPTS
    Motor(uint8_t pwm_in_A, uint8_t pwm_in_B, uint8_t encoder_pin_A, uint8_t encoder_pin_B);
    void initIRQ();
#else

    Motor();

    void initMotor(uint8_t pwm_A, uint8_t pwm_B, uint8_t encoder_A, uint8_t encoder_B, substep_state_t *state,
                   const int *calibration_array);

#endif

    void setPIDVals(float kp, float ki, float kd);

    void updateSpeed();

    /**
     * @brief Sets a new speed for the motor as compared to using PID to match the
     * desired speed
     *
     * @param speed the speed of the motor in encoder counts per timer interval
     */
    void setTargetSpeed(int speed);

    /**
     * @brief Sets both PWM channels high so that the motors have some holding force
     */
    void brake();

    [[nodiscard]] int getCurrEncoderCount() const;

    /**
     * @brief Get the recored encoder speed of the motor if using encoder interrupts. Otherwise, it uses the substep
     * speed calculated by the PIO program
     * @return the encoder delta in the timer interval
     */
    [[nodiscard]] int getEncoderSpeed() const;

    /**
     * @brief Get the Target Speed object
     * @return the target speed of the motor in encoder counts per timer interval
     */
    [[nodiscard]] int getTargetSpeed() const;

#ifdef USE_ENCODER_INTERRUPTS
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
#else
    friend void setPrevSubsteps(void *motor_instance);
#endif
};

extern Motor motor1;
extern Motor motor2;
extern Motor motor3;

#endif