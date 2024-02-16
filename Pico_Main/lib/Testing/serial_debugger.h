#include <Arduino.h>
#include "../../include/pin_definitions.h"

/**
 * @brief This class is purely used for debugging purposes. It is used to send and receive data from the serial monitor.
 * Ideally, this is only used for when the Pico firmware is under development
 * and needs to be tested to make sure that certian functions are working as expected.
 *
 * Used for:
 * - Testing the PID controller
 * - Testing the motor drivers
 * - Testing the I2C communication to the IMU
 *
 */

enum RequestType
{
    SET_MOTOR_SPEED = 'M',
    STOP = 'S',
    IMU_DATA = 'I',
    CHECK_DSP = 'D',
    CHECK_ENCODER_COUNTS = 'E',
    CHECK_INTERTUPS = 'Q',
};

class SerialDebugger
{

private:
    String buffer = "";
    void processRequest(String buffer);

public:
    bool begin(unsigned long baudRate)
    {
        Serial.begin(baudRate);
        return true;
    }

    void send(const char *data)
    {
        Serial.print(data);
    }

    void receive()
    {
        buffer = "";
        if (Serial.available() > 0)
        {
            char receivedData = Serial.read();
            buffer += receivedData;
        }
        processRequest(buffer);
    }
};
