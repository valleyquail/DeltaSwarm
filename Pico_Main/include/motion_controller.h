#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "../lib/Motion/motor.h"

/**
 * @brief Class that hanbdles the motion of the robot. This only handles the
 * driving of the robot, not the navigation nor the control of any appendages.
 *
 */
class MotionController
{

private:
    Motor *motor1 = nullptr;
    Motor *motor2 = nullptr;
    Motor *motor3 = nullptr;

public:
    MotionController();

    /**
     * @brief Sets the speed of the robot
     *
     * @param speed Speed of the robot in m/s
     * @param theta Angle to head in radians relative to global orientation
     * @param omega Angular velocity in rad/s
     */
    void setSpeed(float speed, float theta, float omega);

    // Stops the robot and sets the motors to idle
    void stop();

    // Sets both PWM channels high so that the motors have some force to hold the robot in place
    void brake();
    int *getEncoderValues();

    void runPIDUpdate();
};
