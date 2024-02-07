cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(Pico_Controller C CXX ASM)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(Pico_Main
  src/test.c
  lib/Communication/I2C_Control.h lib/Communication/I2C_Control.c
)
pico_enable_stdio_usb(Pico_Main 1)
pico_enable_stdio_uart(Pico_Main 1)
pico_add_extra_outputs(Pico_Main)
target_link_libraries(Pico_Main 
pico_stdlib
hardware_pwm
hardware_gpio
hardware_timer
)