#ifndef STATUS_LED_H
#define STATUS_LED_H

#include "pico/stdlib.h"
#include <Adafruit_NeoPixel.h>

const uint8_t NUMPIXELS = 1;

class StatusLED
{
private:
    Adafruit_NeoPixel pixel;

public:
    StatusLED(uint8_t pin);

    void SetError();
    void SetWarning();
    void SetOK();
    void SetOff();
};

#endif