#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"

class Motor
{
protected:
    // Used to control the motors:
    enum controlMode
    {
        SET_NEW_SPEED,
        UPDATE_SPEED
    };

    volatile controlMode mode = SET_NEW_SPEED;
    // PWM pin A and B, and encoder pin A and B for controlling the motor and
    // getting encoder counts
    uint8_t pwm_in_A;
    uint8_t pwm_in_B;
    uint8_t encoder_pin_A;
    uint8_t encoder_pin_B;

    volatile int16_t lastA = 0;
    volatile int16_t lastB = 0;
    // Stores the enbcoder count since the program started
    int total_encoder_count = 0;
    // Stores the encoder count since the last movement
    // Resets everytime there is a new movement
    int curr_movement_encoder_count = 0;
    // keeps track of encoder changes
    volatile int16_t prevCount = 0;

    // PID control variables
    float kp;
    float ki;
    float kd;

    // Encoder count within the timer interval
    // TODO Determine the timer interval
    volatile uint16_t encoderSpeed = 0;
    // Keep track of target speed
    float target_speed = 0;
    // Integral control
    int32_t sumError = 0;

    void updateSpeed();
    void setPWM(float speed);

public:
    /**
     * @brief Construct a new Motor object
     */
    Motor(uint8_t pwm_in_A, uint8_t pwm_in_B, uint8_t encoder_pin_A, uint8_t encoder_pin_B);

    /**
     * @brief Set the PID values for the motor
     */
    void setPIDVals(float kp, float ki, float kd);

    /**
     * @brief Sets a new speed for the motor as compared to using PID to match the
     * desired speed
     *
     * @param speed
     */
    void setSpeed(float speed);
    /**
     * @brief Sets both PWM channels high so that the motors have some holding force
     *
     */
    void brake();
    /**
     * @brief Calculate the change in encoder count since the last measurement
     * Used to calculate the speed of the motor in terms of encoder counts
     *
     */
    void calcEncoderDelta();

    int getEncoderCount();
};