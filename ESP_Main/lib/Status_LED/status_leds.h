#ifndef STATUS_LED_H
#define STATUS_LED_H

#include <stdio.h>


void initStatusLED(int8_t pin);

// Red
void statusLEDSetError();

// Yellow
void statusLEDSetWarning();

// Green
void statusLEDSetOK();

// Off
void SetStatusLEDOff();

void SetBatteryLEDColor(uint8_t level);

#endif
