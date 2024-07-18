#include "motion_controller.h"
#include "../../include/config.h"
#include <cmath>
#include "RPi_Pico_TimerInterrupt.h"
#include "encoder.h"
#include "../I2C_Control/i2c_control.h"

// Physical constants for the robot that detmine how the robot moves
const float WHEEL_RADIUS = 0.0325; // meters
const float ROBOT_DIAMETER = 0.15; // meters
const int ENCODER_COUNTS_PER_REV = 12 * 70;

// PID values for each motor
const float MOTOR1_KP = .5;
const float MOTOR1_KI = 1;
const float MOTOR1_KD = 1;

const float MOTOR2_KP = .5;
const float MOTOR2_KI = 1;
const float MOTOR2_KD = 1;

const float MOTOR3_KP = .5;
const float MOTOR3_KI = 1;
const float MOTOR3_KD = 1;

Motor motor1(MOTOR1_A, MOTOR1_B, MOTOR1_A_ENC, MOTOR1_B_ENC);
Motor motor2(MOTOR2_A, MOTOR2_B, MOTOR2_A_ENC, MOTOR2_B_ENC);
Motor motor3(MOTOR3_A, MOTOR3_B, MOTOR3_A_ENC, MOTOR3_B_ENC);

RPI_PICO_TimerInterrupt timer(0);

bool timerISR(struct repeating_timer *t);

MotionController::MotionController() {

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

inline int speedToEncoder(float speed) {
    // Convert the speed to the encoder speed
    return (int) (speed * ENCODER_COUNTS_PER_REV / (2 * PI * WHEEL_RADIUS));
}

// TODO: Implement a keep orientation option so that it either arcs or it rotates
// to keep the same orientation while moving forward
void MotionController::setSpeed(float speed, float theta, float omega) {
    auto theta_one = (float) (theta + PI / 3);
    auto theta_two = (float) (theta - PI / 3);
    // Convert the speed, theta, and omega to the speed of each wheel in m/s
    float v1 = speed * sin(theta) - omega * ROBOT_DIAMETER / 2;
    float v2 = speed * sin(theta_one) - omega * ROBOT_DIAMETER / 2;
    float v3 = speed * sin(theta_two) - omega * ROBOT_DIAMETER / 2;

    // Convert the speed of each wheel to the encoder speed
    int encoderSpeed1 = speedToEncoder(v1);
    int encoderSpeed2 = speedToEncoder(v2);
    int encoderSpeed3 = speedToEncoder(v3);
#ifdef SPEEDS_DEBUG
    Serial.printf("Input speeds: %f, %f, %f\n", speed, theta, omega);
    Serial.printf("Speeds: %f, %f, %f\n", v1, v2, v3);
    Serial.printf("Encoder speeds: E1: %i, E2: %i, E3: %i\n\n", encoderSpeed1, encoderSpeed2, encoderSpeed3);
#endif
    // Set the speed of each wheel
    motor1.setTargetSpeed(encoderSpeed1);
    motor2.setTargetSpeed(encoderSpeed2);
    motor3.setTargetSpeed(encoderSpeed3);
}

void MotionController::setSpeedFromI2C(const uint8_t *speeds) {
    // Bit shift the speeds to get the float values
    auto speed = (float) (0xFFFF & (speeds[0] << 24 | speeds[1] << 16 | speeds[2] << 8 | speeds[3]));
    auto theta = (float) (0xFFFF & (speeds[4] << 24 | speeds[5] << 16 | speeds[6] << 8 | speeds[7]));
    auto omega = (float) (0xFFFF & (speeds[8] << 24 | speeds[9] << 16 | speeds[10] << 8 | speeds[11]));
    // Get a boolean value for if the robot should keep its orientation
    bool orientation = speeds[16] & 0x01;
#ifdef DEBUG
    Serial.printf("Speed: %f, Theta: %f, Omega: %f\n", speed, theta, omega);
#endif
    setSpeed(speed, theta, omega);
}

void MotionController::stop() {
#ifdef DEBUG
    Serial.printf("Stopping\n");
#endif
    motor1.setTargetSpeed(0);
    motor2.setTargetSpeed(0);
    motor3.setTargetSpeed(0);
}

void MotionController::brake() {
    motor1.brake();
    motor2.brake();
    motor3.brake();
}

void MotionController::debugMotorSpeeds() {
#ifdef PID_DEBUG

    int target1 = motor1.getTargetSpeed();
    int target2 = motor2.getTargetSpeed();
    int target3 = motor3.getTargetSpeed();

    int speed1 = motor1.getEncoderSpeed() * 1000 / TIMER_INTERVAL_MS;
    int speed2 = motor2.getEncoderSpeed() * 1000 / TIMER_INTERVAL_MS;
    int speed3 = motor3.getEncoderSpeed() * 1000 / TIMER_INTERVAL_MS;
    Serial.printf("Motor 1: Target: %i, Speed: %i\n", target1, speed1);
    Serial.printf("Motor 2: Target: %i, Speed: %i\n", target2, speed2);
    Serial.printf("Motor 3: Target: %i, Speed: %i\n", target3, speed3);
#endif
#ifdef PID_CAL
    Serial.printf("%i\t%i\n", motor1.getTargetSpeed(),  motor1.getEncoderSpeed() * 1000 / 20 );
#endif
}


int *MotionController::getEncoderValues() {
    int *encoderValues = new int[3];
    encoderValues[0] = motor1.getCurrEncoderCount();
    encoderValues[1] = motor2.getCurrEncoderCount();
    encoderValues[2] = motor3.getCurrEncoderCount();
    return encoderValues;
}

void MotionController::runPIDUpdate() {
#ifdef DEBUG
    Serial.printf("Running PID update\n");
#endif
    motor1.updateSpeed();
    motor2.updateSpeed();
    motor3.updateSpeed();
}


bool timerISR(struct repeating_timer *t) {
    (void) t;
    calcEncoderDelta(&motor1);
    calcEncoderDelta(&motor2);
    calcEncoderDelta(&motor3);
    return true;
}