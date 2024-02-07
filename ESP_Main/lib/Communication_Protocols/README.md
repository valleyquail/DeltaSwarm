# Pico Communication Over UART Outline

The ESP32 on this board communicates via UART to the an rp2040 which serves as the motor
controller. This offloads a lot interrupts and pin requirements from the ESP32
to the rp2040 with relatively few consequences. The motor control only really
requires 3 components:

Component | Value| Data Type| Meaning|
---|-----|---|---
Theta | Radians | Float |Represents the angle relative to the heading in which to travel |
Feed | m/s | Float |Represents the speed in meters per second at which the robot should travel |
Omega | Radians/s | Float|The rate at which the robot should rotate about is center

Thus, the format of the information sent over UART to the should be in a format
like such:
> T####F####W#### 

Alternatively, there is a shorter command for stop to prevent the rp2040 from
having to parse floats from the data. The command for stop should just be:

>SM

for "Stop Motors"

Note: The protocol for controlling motors 4-6 has not been implemented yet since
their functionality is yet to be determined

Where `T` is a flag indicating the theta component, `F` is a flag indicating the
speed component, and `W` is a flag indicating the omega component

Furthermore, the ESP32 can request data from the rp2040 such as 
* Current encoder counts from each of the motors
* Current encoder counts for the current movement
* Processed data from the ICM-20948 IMU 
* TBD


# SPI Communication Outline

The ESP32 on this board supports 3 SPI channels:
1. Communication to the ESP32-CAM
2. Communication to the Raspberry Pi port
3. One spare header that can be used as an expansion port
