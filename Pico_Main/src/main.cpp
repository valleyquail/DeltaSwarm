#include <Arduino.h>
#include "config.h"
#include <FreeRTOS.h>
#include <motion_controller.h>
#include <status_LED.h>
#include <pico/stdlib.h>

MotionController *motionController = new MotionController();

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
