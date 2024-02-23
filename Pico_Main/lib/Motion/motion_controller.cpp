#include "motion_controller.h"
#include "../../include/config.h"
#include <math.h>
#include "RPi_Pico_TimerInterrupt.h"
#include "../../include/pin_definitions.h"
#include "motor.h"
#include "encoder.h"
#include "../I2C_Control/i2c_control.h"

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

Motor motor1(MOTOR1_A, MOTOR1_B, MOTOR1_A_ENC, MOTOR1_B_ENC);
Motor motor2(MOTOR2_A, MOTOR2_B, MOTOR2_A_ENC, MOTOR2_B_ENC);
Motor motor3(MOTOR3_A, MOTOR3_B, MOTOR3_A_ENC, MOTOR3_B_ENC);

RPI_PICO_TimerInterrupt timer(0);
bool timerISR(struct repeating_timer *t);

MotionController::MotionController()
{
    // Set PID values for each motor
    motor1.setPIDVals(MOTOR1_KP, MOTOR1_KI, MOTOR1_KD);
    motor2.setPIDVals(MOTOR2_KP, MOTOR2_KI, MOTOR2_KD);
    motor3.setPIDVals(MOTOR3_KP, MOTOR3_KI, MOTOR3_KD);

    // Initialize the encoder interrupt timer
    timer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, timerISR);
    // Set the GPIO pins to trigger the encoder interrupts using the interrupt
    // callback defined in encoder.cpp
    gpio_set_irq_enabled_with_callback(MOTOR1_A_ENC, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
}

inline int speedToEncoder(float speed)
{
    // Convert the speed to the encoder speed
    return (int)floor(speed * ENCODER_COUNTS_PER_REV / (2 * PI * WHEEL_RADIUS));
}
// TODO: Implement a keep orientation option so that it either arcs or it rotates
// to keep the same orientation while moving forward
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
#ifdef DEBUG
    Serial.printf("Speeds: %f, %f, %f\n", v1, v2, v3);
    Serial.printf("Encoder speeds: E1: %i, E2: %i, E3: %i\n", encoderSpeed1, encoderSpeed2, encoderSpeed3);
#endif
    // Set the speed of each wheel

    motor1.setTargetSpeed(encoderSpeed1);
    motor2.setTargetSpeed(encoderSpeed2);
    motor3.setTargetSpeed(encoderSpeed3);
}

void MotionController::setSpeedFromI2C(uint8_t *speeds)
{
    // Bit shift the speeds to get the float values
    float speed = speeds[1] << 24 | speeds[2] << 16 | speeds[3] << 8 | speeds[4];
    float theta = speeds[6] << 24 | speeds[7] << 16 | speeds[8] << 8 | speeds[9];
    float omega = speeds[11] << 24 | speeds[12] << 16 | speeds[13] << 8 | speeds[14];
    // Get a boolean value for if the robot should keep its orientation
    // bool orientation = speeds[16] & 0x01;
#ifdef DEBUG
    Serial.printf("Speed: %f, Theta: %f, Omega: %f\n", speed, theta, omega);
#endif
    setSpeed(speed, theta, omega);
}

void MotionController::stop()
{
#ifdef DEBUG
    Serial.printf("Stopping\n");
#endif
    motor1.setTargetSpeed(0);
    motor2.setTargetSpeed(0);
    motor3.setTargetSpeed(0);
}

void MotionController::brake()
{
    motor1.brake();
    motor2.brake();
    motor3.brake();
}

int *MotionController::getEncoderValues()
{
    int *encoderValues = new int[3];
    encoderValues[0] = motor1.getEncoderCount();
    encoderValues[1] = motor2.getEncoderCount();
    encoderValues[2] = motor3.getEncoderCount();
    return encoderValues;
}

void MotionController::runPIDUpdate()
{
#ifdef DEBUG
    Serial.printf("Running PID update\n");
#endif
    motor1.updateSpeed();
    motor2.updateSpeed();
    motor3.updateSpeed();
}

bool timerISR(struct repeating_timer *t)
{
    (void)t;
    calcEncoderDelta(&motor1);
    calcEncoderDelta(&motor2);
    calcEncoderDelta(&motor3);
    return true;
}