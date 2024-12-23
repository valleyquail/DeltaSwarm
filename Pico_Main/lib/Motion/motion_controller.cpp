#include "motion_controller.h"
#include "../../include/config.h"
#include <cmath>
#include "RPi_Pico_TimerInterrupt.h"
#include "encoder.h"
#include "../I2C_Control/i2c_control.h"
#include "pico/double.h"
#include "quad_substep.h"
#include "quadrature_substep_pio.pio.h"


// Physical constants for the robot that detmine how the robot moves
const float WHEEL_RADIUS = 0.02; // meters --> 2 cm
const float ROBOT_DIAMETER = 0.15; // meters
const int ENCODER_COUNTS_PER_REV = 7 * 4 * 150; // 7 PPR = 28 counts per revolution with quadrature, 150:1 gear ratio
constexpr int TIMER_INTERVAL_MS = 15;

#ifdef USE_ENCODER_INTERRUPTS
// PID values for each motor

const float M1_KP = .12;
const float M1_KI = .05;
const float M1_KD = .3;

const float M2_KP = .2;
const float M2_KI = .1;
const float M2_KD = .2;

const float M3_KP = .2;
const float M3_KI = .1;
const float M3_KD = .2;

Motor motor1(MOTOR1_B, MOTOR1_A, MOTOR1_A_ENC, MOTOR1_B_ENC);
Motor motor2(MOTOR2_A, MOTOR2_B, MOTOR2_A_ENC, MOTOR2_B_ENC);
Motor motor3(MOTOR3_A, MOTOR3_B, MOTOR3_A_ENC, MOTOR3_B_ENC);
#else

// PID values for each motor
const int M1_CALIBRATION[] = {0, 66, 126, 192};
const float M1_KP = .22;
const float M1_KI = .4;
const float M1_KD = .35;

const int M2_CALIBRATION[] = {0, 45, 114, 190};
const float M2_KP = .2;
const float M2_KI = .1;
const float M2_KD = .2;

const int M3_CALIBRATION[] = {0, 54, 105, 198};
const float M3_KP = .2;
const float M3_KI = .1;
const float M3_KD = .2;
Motor motor1;
Motor motor2;
Motor motor3;
substep_state_t state1;
substep_state_t state2;
substep_state_t state3;
#endif

RPI_PICO_TimerInterrupt timer(0);

bool timerISR(struct repeating_timer *t);

bool newEncoderValues = false;

MotionController::MotionController() {};

void MotionController::initMotionController() {
#ifdef USE_ENCODER_INTERRUPTS
    // Set the GPIO pins to trigger the encoder interrupts using the interrupt
    // callback defined in encoder.cpp
    gpio_set_irq_enabled_with_callback(MOTOR1_A_ENC, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_callback(&gpio_callback);
    motor1.initIRQ();
    motor2.initIRQ();
    motor3.initIRQ();
#else
// Initialize the motors
    //Use PIO block 0
    PIO pio = pio0;
    //Add the pio program into the pio block
    pio_add_program(pio, &quadrature_encoder_substep_program);
    //Initialize the pio state machines to reduce the arguments needed for the initMotor function
    state1.pio = pio;
    state1.sm = 0;
    state2.pio = pio;
    state2.sm = 1;
    state3.pio = pio;
    state3.sm = 2;

    // Initialize the motors
    motor1.initMotor(MOTOR1_A, MOTOR1_B, MOTOR1_A_ENC, MOTOR1_B_ENC, &state1, M1_CALIBRATION);
    motor2.initMotor(MOTOR2_A, MOTOR2_B, MOTOR2_A_ENC, MOTOR2_B_ENC, &state2, M2_CALIBRATION);
    motor3.initMotor(MOTOR6_A, MOTOR6_B, MOTOR6_A_ENC, MOTOR6_B_ENC, &state3, M3_CALIBRATION);
    //Claim the pio states so that the Neopixel uses the pio1 block
    pio_claim_sm_mask(pio, 0b1111);

#endif
    // Set PID values for each motor
    motor1.setPIDVals(M1_KP, M1_KI, M1_KD);
    motor2.setPIDVals(M2_KP, M2_KI, M2_KD);
    motor3.setPIDVals(M3_KP, M3_KI, M3_KD);


    // Initialize the encoder interrupt timer
    timer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, timerISR);
}

/**
 * @brief Converts the speed of the wheel to the encoder counts per timer interval
 * @param speed the speed of the wheel in m/s
 * @return encoder counts per second
 */
inline int speedToEncoder(float speed) {
    return (int) (speed * ENCODER_COUNTS_PER_REV / (2 * PI * WHEEL_RADIUS));

}

// TODO: Implement a keep orientation option so that it either arcs or it rotates
// to keep the same orientation while moving forward
void MotionController::setSpeed(float speed, float theta, float omega) {
    auto theta_one = (float) (theta + PI / 3);
    auto theta_two = (float) (theta + 2 * PI / 3);
    // Convert the speed, theta, and omega to the speed of each wheel in m/s
    float v1 = speed * sin(theta) - omega * ROBOT_DIAMETER / 2;
    float v2 = speed * sin(theta_one) - omega * ROBOT_DIAMETER / 2;
    float v3 = speed * sin(theta_two) - omega * ROBOT_DIAMETER / 2;

    // Convert the speed of each wheel to the encoder ticks per timer interval
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
//    motor2.setTargetSpeed(encoderSpeed2);
//    motor3.setTargetSpeed(encoderSpeed3);
}

inline float bytesToFloat(const uint8_t *bytes) {
    float value;
    memcpy(&value, bytes, sizeof(value));
    return value;
}

void MotionController::setSpeedFromI2C(const uint8_t *speeds) {
    // Bit shift the speeds to get the float values
    float speed = bytesToFloat(&speeds[0]);
    float theta = bytesToFloat(&speeds[4]);
    float omega = bytesToFloat(&speeds[8]);
    // Get a boolean value for if the robot should keep its orientation
    bool orientation = speeds[16] & 0x01;
#ifdef SPEEDS_DEBUG
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


    int target1 = motor1.getTargetSpeed();
    int target2 = motor2.getTargetSpeed();
    int target3 = motor3.getTargetSpeed();

    int speed1 = motor1.getEncoderSpeed();
//    int speed2 = motor2.getEncoderSpeed() * 1000/TIMER_INTERVAL_MS;
//    int speed3 = motor3.getEncoderSpeed() * 1000/TIMER_INTERVAL_MS;
    Serial.printf("Motor 1: Target: %i, Speed: %i\n", target1, speed1);
//    Serial.printf("Motor 2: Target: %i, Speed: %i\n", target2, speed2);
//    Serial.printf("Motor 3: Target: %i, Speed: %i\n", target3, speed3);

//    Serial.printf("%i\t%i\n", motor1.getTargetSpeed(),  motor1.getEncoderSpeed() * 1000 / TIMER_INTERVAL_MS );

}

int encoderValues[] = {0, 0, 0};
int *MotionController::getEncoderValues() {
    encoderValues[0] = motor1.getCurrEncoderCount();
    encoderValues[1] = motor2.getCurrEncoderCount();
    encoderValues[2] = motor3.getCurrEncoderCount();
    return encoderValues;
}

void MotionController::runPIDUpdate() {
    // If there are no new encoder values, then don't run the PID update
    if (!newEncoderValues) {
        return;
    }
#ifdef DEBUG
    Serial.printf("Running PID update\n");
#endif
    // Update the speed of each motor and disable interrupts while updating the speed
    // so that the timer ISR doesn't overwrite the speed
//    uint32_t status = save_and_disable_interrupts();

    motor1.updateSpeed();
//    motor2.updateSpeed();
//    motor3.updateSpeed();

//    restore_interrupts(status);
    newEncoderValues = false;
}

/***
 * @brief Runs the motor calibration code to determine the phase sizes of the motors
 */
void MotionController::runMotorCalibration() {
    PIO pio = pio0;
    uint sm = 0;
    Serial.printf("Hello from quadrature encoder substep\n");


    Serial.printf("Calibrating\n");
    Motor motorArray[] = {motor1, motor2, motor3};
    for (int i = 0; i < 3; i++) {
        substep_state_t state;

        motorArray[i].setSpeed(2000); // 2000 encoder counts per second
        // - wait for the motor to reach a reasonably stable speed
        sleep_ms(2000);
        // - run the phase size calibration code
        substep_init_state(pio, i, motorArray[i].encoder_pin_A, &state);
        substep_calibrate_phases(pio, sm);
        Serial.printf("Calibrated\n");
        motorArray[i].setSpeed(0);
    }
    Serial.println("All motors are calibrated");
}


bool timerISR(struct repeating_timer *t) {
    (void) t;
#ifdef USE_ENCODER_INTERRUPTS
    calcEncoderDelta(&motor1);
    calcEncoderDelta(&motor2);
    calcEncoderDelta(&motor3);
#else
    substep_update(&state1);
    substep_update(&state2);
    substep_update(&state3);
#endif
    newEncoderValues = true;
    return true;
}
