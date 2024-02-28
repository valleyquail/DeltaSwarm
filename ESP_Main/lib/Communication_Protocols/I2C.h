#include <stdio.h>
#include <stdlib.h>
#include "driver/i2c.h"
#include "../../include/config.h"
class Pico_I2C
{
private:
    uint8_t motorCommandBuffer[MOTOR_COMMAND_SIZE + 2];
    uint8_t encoderCountBuffer[16];
    uint8_t IMUDataBuffer[16];

public:
    Pico_I2C();
    bool init();
    void sendMovement(float speed, float theta, float omega, bool orientation);
    void requestEncoderCounts();
    // TODO: Break out the different types of IMU data
    void requestIMUData();
};