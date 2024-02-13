# Introduction
This is the repository for hosting all of the code for the Delta Rho Robot for
the WPI SRL collective manipulation project.

The repository for the schematics, BOM, and PCB for the robot can be found here:
https://github.com/valleyquail/DeltaRhoPCB

# Setup
Eventually, there will be a Docker container that will simplify the entire
process, but to set up currently requires quite a few steps.

The ESP_Main code depends on 3 different libraries/frameworks to function and
compile:

* ESP-IDF provided by Espressif
* MicroROS ESP-IDF component provided by MicroROS
* Arduino-ESP32 for Arduino libaries as an ESP-IDF component proivded by
  Espressif

To get all of these to work together requires quite a bit of (somewhat simple)
work. The bulk of the hard work is handled by the sdkconfig file to make sure
that all of these libraries play nice with eachother.

## ESP-IDF
The simplest way to get the ESP-IDF to work is to use VSCode and install the
Espressif plug in. This loads all of the requried libraries and frameworks for
the ESP32.

## MicroROS
This is fairly simple to install actually. Basically, go into the `components/`
folder and then run a git clone using:
> git clone git@github.com:micro-ROS/micro_ros_espidf_component.git

More information can be found the the GitHub repository for MicroROS found here:

https://github.com/micro-ROS/micro_ros_espidf_component

## Arduino-ESP32 
This is the hardest and most annoying to install. The best way to do this is to
follow the directions found on the actual site:

https://github.com/micro-ROS/micro_ros_espidf_component

To summarize the instructions, there are three things to git clone: 
* The Arduino component
* The Arduino library builder
* TinyUSB

To install the Arduino component, you need to go into the `components/` folder
and then run:
> git clone https://github.com/espressif/arduino-esp32.git arduino && \
cd arduino && \
git submodule update --init --recursive

After that, go to the folder above `ESP_Main` and then run:

> git clone https://github.com/espressif/esp32-arduino-lib-builder.git esp32-arduino-lib-builder && \
git clone https://github.com/hathach/tinyusb.git
esp32-arduino-lib-builder/components/arduino_tinyusb/tinyusb

This should make the `ESP_Main` functional and set up the workflow.

To actually compile the code and test things, you need to go to the directory
where the Espressif component is installed and then run:

> source ~/esp/esp-idf/exports.sh

This will allow you to run `idf.py build` that compiles the code together.

## rp2040 Pico Development

To set up the rp2040 development environment, there is not too much to do. Much
of this can be set up by following the official Raspbetty Pi Foundation guide to
getting started with the Pico found here:

https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf

Most of the set up comes from running two commands which can be found on the 7th
page of the guide.