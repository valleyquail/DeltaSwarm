#include "motion_controller.h"
#include "../include/pin_definitions.h"
#include <math.h>
#include "RPi_Pico_TimerInterrupt.h"
#include "config.h"

// Physical constants for the robot that detmine how the robot moves
const float WHEEL_RADIUS = 0.0325; // meters
const float ROBOT_DIAMETER = 0.15; // meters
const int ENCODER_COUNTS_PER_REV = 12 * 70;

// PID values for each motor
const float MOTOR1_KP = 0.1;
const float MOTOR1_KI = 0.1;
const float MOTOR1_KD = 0.1;

const float MOTOR2_KP = 0.1;
const float MOTOR2_KI = 0.1;
const float MOTOR2_KD = 0.1;

const float MOTOR3_KP = 0.1;
const float MOTOR3_KI = 0.1;
const float MOTOR3_KD = 0.1;

Motor *motor1 = new Motor(MOTOR1_A, MOTOR1_B, MOTOR1_A_ENC, MOTOR1_B_ENC);
Motor *motor2 = new Motor(MOTOR2_A, MOTOR2_B, MOTOR2_A_ENC, MOTOR2_B_ENC);
Motor *motor3 = new Motor(MOTOR3_A, MOTOR3_B, MOTOR3_A_ENC, MOTOR3_B_ENC);

RPI_PICO_TimerInterrupt timer(0);

MotionController::MotionController()
{
    this->motor1 = motor1;
    this->motor2 = motor2;
    this->motor3 = motor3;

    // Set PID values for each motor
    motor1->setPIDVals(MOTOR1_KP, MOTOR1_KI, MOTOR1_KD);
    motor2->setPIDVals(MOTOR2_KP, MOTOR2_KI, MOTOR2_KD);
    motor3->setPIDVals(MOTOR3_KP, MOTOR3_KI, MOTOR3_KD);

    // Initialize the encoder interrupt timer
}

inline int speedToEncoder(float speed)
{
    // Convert the speed to the encoder speed
    return (int)floor(speed * ENCODER_COUNTS_PER_REV / (2 * PI * WHEEL_RADIUS));
}

void MotionController::setSpeed(float speed, float theta, float omega)
{
    // Convert the speed, theta, and omega to the speed of each wheel
    float v1 = speed * cos(theta) - omega * ROBOT_DIAMETER / 2;
    float v2 = speed * cos(theta + 2 * PI / 3) - omega * ROBOT_DIAMETER / 2;
    float v3 = speed * cos(theta - 2 * PI / 3) - omega * ROBOT_DIAMETER / 2;

    // Convert the speed of each wheel to the encoder speed
    int encoderSpeed1 = speedToEncoder(v1);
    int encoderSpeed2 = speedToEncoder(v2);
    int encoderSpeed3 = speedToEncoder(v3);

    // Set the speed of each wheel
    motor1->setSpeed(encoderSpeed1);
    motor2->setSpeed(encoderSpeed2);
    motor3->setSpeed(encoderSpeed3);
}

void MotionController::stop()
{
    motor1->setSpeed(0);
    motor2->setSpeed(0);
    motor3->setSpeed(0);
}

void MotionController::brake()
{
    motor1->brake();
    motor2->brake();
    motor3->brake();
}

int *MotionController::getEncoderValues()
{
    int *encoderValues = new int[3];
    encoderValues[0] = motor1->getEncoderCount();
    encoderValues[1] = motor2->getEncoderCount();
    encoderValues[2] = motor3->getEncoderCount();
    return encoderValues;
}

void MotionController::runPIDUpdate()
{
    motor1->updateSpeed();
    motor2->updateSpeed();
    motor3->updateSpeed();
}

void timerISR()
{
    calcEncoderDelta(motor1);
    calcEncoderDelta(motor2);
    calcEncoderDelta(motor3);
}