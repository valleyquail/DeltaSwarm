#include <Arduino.h>
#include "../include/config.h"
#include <FreeRTOS.h>
#include "../lib/Motion/motion_controller.h"
#include <status_LED.h>
#include <pico/stdlib.h>
#include <../lib/I2C_Control/i2c_control.h>
#include <../lib/I2C_Control/IMU.h>
#include <../lib/Testing/serial_debugger.h>

MotionController motionController = MotionController();
StatusLED statusLED = StatusLED(NEOPIXEL_PIN);

void setup()
{
  Serial.begin(115200);
  gpio_init(1);
  gpio_set_dir(1, GPIO_OUT);
  gpio_put(1, 1);
  // pinMode(1, OUTPUT);
  for (int i = 0; i < 100; ++i)
  {
    Serial.printf("Launching in %i ms\n", (100 - i) * 20);
    sleep_ms(20);
  }

  // statusLED.SetWarning();
  delay(1000);
  gpio_put(1, 0);
  Serial.printf("Testing?");
  delay(1000);
  Serial.printf("Testing?");
  motionController.setSpeed(0.5, 0, 0);
  Serial.printf("Testing?");
#if PICO_USE_USB_SERIAL
  SerialDebugger serialDebugger = SerialDebugger();
  // statusLED.SetOK();
  while (true)
  {
    if (!Serial.available())
      serialDebugger.receive();
    else
      delay(20);
  }
#endif

// For testing without the ESP
#ifdef NO_ESP_CONNECTION
  printf("I2C from ESP\n");
  initPicoClient();
#endif

#ifdef NO_IMU
  printf("IMU\n");
  IMU_I2C *imu = new IMU_I2C();
#endif

  // statusLED.SetOK();
}

void loop()
{
  gpio_put(1, 1);
  Serial.printf("looped\n");
  // statusLED.SetWarning();
  delay(1000);
  gpio_put(1, 0);
  delay(1000);
  motionController.runPIDUpdate();

  // put your main code here, to run repeatedly:
}
