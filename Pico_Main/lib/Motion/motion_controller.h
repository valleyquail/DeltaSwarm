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

    /**
     * @brief Sets the speed of the robot
     *
     * @param speed Speed of the robot in m/s
     * @param theta Angle to head in radians relative to global orientation
     * @param omega Angular velocity in rad/s
     */
    static void setSpeed(float speed, float theta, float omega);

    /**
     * @brief Set the Speed object
     *
     * @param speeds array from the I2C interrupt that contains the speeds for each motor
     */
    static void setSpeedFromI2C(const uint8_t *speeds);
//    static void setM1PWM(int speed);
    // Stops the robot and sets the motors to idle
    static void stop();
    void debugMotorSpeeds();
    // Sets both PWM channels high so that the motors have some force to hold the robot in place
    static void brake();
    static int *getEncoderValues();
    void runPIDUpdate();
    static void runMotorCalibration();

};

extern MotionController motionController;

#endif