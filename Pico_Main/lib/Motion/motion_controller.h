#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include <cstdio>
#include <cstdlib>
#include <Arduino.h>
#include "motor.h"



/**
 * @brief Class that hanbdles the motion of the robot. This only handles the
 * driving of the robot, not the navigation nor the control of any appendages.
 *
 */
class MotionController
{
public:
    MotionController();

    void initMotionController();

    /**
     * @brief Sets the speed of the robot
     *
     * @param speed Speed of the robot in m/s
     * @param theta Angle to head in radians relative to global orientation
     * @param omega Angular velocity in rad/s
     */
    static void setSpeed(float speed, float theta, float omega);

    /**
     * @brief Set the robot's speed after processing the I2C message
     *
     * @param speeds array from the I2C interrupt that contains the speeds for each motor
     */
    static void setSpeedFromI2C(const uint8_t *speeds);

    /**
     * @brief Updates the speed of the motors using the PID control
     */
    static void runPIDUpdate();

    /**
     * @brief Stops the robot and motors are in a passive state
     */
    static void stop();


    /**
     * @brief Sets both PWM channels high so that the motors have holding force
     */
    static void brake();

    static int *getEncoderValues();

    static int *runMotorCalibration();

    void debugMotorSpeeds();
};

extern MotionController motionController;

#endif