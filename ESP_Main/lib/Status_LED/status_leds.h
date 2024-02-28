#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class StatusLED
{
private:
    Adafruit_NeoPixel pixels;
    const uint8_t statusLED_index = 0;
    const uint8_t batteryLED_index = 1;

public:
    StatusLED(uint8_t pin);
    // Red
    void SetError();
    // Yellow
    void SetWarning();
    // Green
    void SetOK();
    // Off
    void SetOff();

    void SetBattery(uint8_t level);
};

extern StatusLED statusLED;
