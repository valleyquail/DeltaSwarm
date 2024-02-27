#include "status_leds.h"

StatusLED::StatusLED(uint8_t pin)
{
    this->pixels = Adafruit_NeoPixel(NUMPIXELS, pin, NEO_GRB + NEO_KHZ800);
    this->pixels.begin();
}

// Set the LED to red
void StatusLED::SetError()
{
    this->pixels.setPixelColor(statusLED_index, 255, 0, 0);
    this->pixels.show();
}

// Set the LED to yellow
void StatusLED::SetWarning()
{
    this->pixels.setPixelColor(statusLED_index, 255, 255, 0);
    this->pixels.show();
}

// Set the LED to green
void StatusLED::SetOK()
{
    this->pixels.setPixelColor(statusLED_index, 0, 255, 0);
    this->pixels.show();
#ifdef DEBUG
    Serial.printf("LED on\n");
#endif
}

// Turn off the LED
void StatusLED::SetOff()
{
    this->pixels.setPixelColor(statusLED_index, 0, 0, 0);
    this->pixels.show();
#ifdef DEBUG
    Serial.printf("LED off\n");
#endif
}

void StatusLED::SetBattery(uint8_t level)
{

    // set the battery LED to red if the battery is below 20%
    if (level < 20)
    {
        this->pixels.setPixelColor(batteryLED_index, 255, 0, 0);
    }
    // set the battery LED to yellow if the battery is below 50%
    else if (level < 50)
    {
        this->pixels.setPixelColor(batteryLED_index, 255, 255, 0);
    }
    // set the battery LED to green if the battery is above 50%
    else
    {
        this->pixels.setPixelColor(batteryLED_index, 0, 255, 0);
    }
}
