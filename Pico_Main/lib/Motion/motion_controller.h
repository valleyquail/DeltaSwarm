#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include "motor.h"
#include <etl/array.h>



/**
 * @brief Class that handles the motion of the robot. This only handles the
 * driving of the robot, not the navigation nor the control of any appendages.
 *
 */
class MotionController
{
public:

    static void initMotionController();
    /**
     * @param speed Speed of the robot in m/s
     * @param theta Angle to head in radians relative to global orientation
     * @param omega Angular velocity in rad/s
     */
    static void setSpeed(float speed, float theta, float omega);
    static void setSpeedFromI2C(const uint8_t *speeds);
    static void runPIDUpdate();
    static void stop();


    /**
     * @brief Sets both PWM channels high so that the motors have holding force
     */
    static void brake();
    static etl::array<int, 3> getEncoderValues();
    static etl::array<int, 12> runMotorCalibration();

    static void debugMotorSpeeds();
};

extern MotionController motionController;

#endif