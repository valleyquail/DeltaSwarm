#include "status_LED.h"

StatusLED::StatusLED(uint8_t pin)
{
    this->pixel = Adafruit_NeoPixel(NUMPIXELS, pin, NEO_GRB + NEO_KHZ800);
    this->pixel.begin();
}

// Set the LED to red
void StatusLED::SetError()
{
    this->pixel.setPixelColor(0, 255, 0, 0);
    this->pixel.show();
}

// Set the LED to yellow
void StatusLED::SetWarning()
{
    this->pixel.setPixelColor(0, 255, 255, 0);
    this->pixel.show();
}

// Set the LED to green
void StatusLED::SetOK()
{
    this->pixel.setPixelColor(0, 0, 255, 0);
    this->pixel.show();
#ifdef DEBUG
    Serial.printf("LED on\n");
#endif
}

// Turn off the LED
void StatusLED::SetOff()
{
    this->pixel.setPixelColor(0, 0, 0, 0);
    this->pixel.show();
#ifdef DEBUG
    Serial.printf("LED off\n");
#endif
}

void StatusLED::showI2CFromESP()
{
    this->pixel.setPixelColor(0, 0, 0, 255);
    this->pixel.show();
#ifdef DEBUG
    Serial.printf("Getting I2C Read\n");
#endif
}