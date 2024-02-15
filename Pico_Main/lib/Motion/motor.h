#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "pico/stdlib.h"
#include "RP2040_PWM.h"

// TODO: Maybe make a child class for drive motors vs actuator motors so that
// there can be functionality to change the PWM frequency of the motor for
// differnt speed control responses?
class Motor
{
protected:
    // 20kHz PWM frequency
    const int PWM_FREQ = 20000;
    // Used to control the motors:
    enum controlMode
    {
        SET_NEW_SPEED,
        UPDATE_SPEED
    };

    volatile controlMode mode = SET_NEW_SPEED;
    // PWM pin A and B, and encoder pin A and B for controlling the motor and
    // getting encoder counts
    uint8_t pwm_pin_A;
    uint8_t pwm_pin_B;
    uint8_t encoder_pin_A;
    uint8_t encoder_pin_B;
    RP2040_PWM *pwmA;
    RP2040_PWM *pwmB;

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
    // Derivative control
    int32_t lastError = 0;
    // Max error for integral control since the duty cycle is limited to 100
    const int maxError = 100;

    void setPWM(float speed);
    void resetEncoderCount();

public:
    Motor(uint8_t pwm_in_A, uint8_t pwm_in_B, uint8_t encoder_pin_A, uint8_t encoder_pin_B);
    void setPIDVals(float kp, float ki, float kd);
    void updateSpeed();
    /**
     * @brief Sets a new speed for the motor as compared to using PID to match the
     * desired speed
     *
     * @param speed
     */
    void setSpeed(float speed);

    /**
     * @brief Sets both PWM channels high so that the motors have some holding force
     */
    void brake();

    int getEncoderCount();

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
};

extern Motor *motor1;
extern Motor *motor2;
extern Motor *motor3;