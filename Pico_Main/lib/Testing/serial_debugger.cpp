#include "serial_debugger.h"
#include "../Motion/motion_controller.h"

extern MotionController motionController;

void SerialDebugger::processRequest(String buffer)
{

    if (buffer.length() > 0)
    {
        switch (buffer[0])
        { // Based on the format:
          // T####F####W####
        case SET_MOTOR_SPEED:
        {
            buffer = buffer.substring(1);
            float theta = buffer.substring(1, 5).toFloat();
            float speed = buffer.substring(6, 10).toFloat();
            float omega = buffer.substring(12, 16).toFloat();
            motionController.setSpeed(speed, theta, omega);
            Serial.println("Set motor speed");
            Serial.print("Encoder counts at speed change");
            // Serial.print(motionController.getEncoderValues()[0]);
            // Serial.print(" ");
            // Serial.print(motionController.getEncoderValues()[1]);
            // Serial.print(" ");
            // Serial.println(motionController.getEncoderValues()[2]);
            // delay(1000);
            // Serial.print("Encoder counts 1 second after speed change");
            // Serial.print(motionController.getEncoderValues()[0]);
            // Serial.print(" ");
            // Serial.print(motionController.getEncoderValues()[1]);
            // Serial.print(" ");
            // Serial.println(motionController.getEncoderValues()[2]);
            break;
        }
        case STOP:
            motionController.stop();
            break;
        case IMU_DATA:
            // getIMUData();
            break;
        case CHECK_DSP:
            // checkDSP();
            break;
        case CHECK_ENCODER_COUNTS:
            // checkEncoderCounts();
            break;
        case CHECK_INTERTUPS:
            // checkInterrupts();
            break;
        default:
            break;
        }
    }
}
