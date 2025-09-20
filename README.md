Engineering materials
====

This repository contains all engineering materials for team Wired Wizards, from CETYS Universidad campus Mexicali as part of Sentinel Fox Engineering, competing on the WRO Future Engineers 2025 season, soon to go to the 2025 Mexican National.

## Team Members
* Yumian Del Mar Rodríguez Téllez
* Ahn Montserrat Salgado González
* Constanza Sandoval Ureña
* `Coach` Leonardo Daniel Médina Madrazo

## Content


`models:` files used by 3D printers to produce the vehicle's chassis.   
`schemes:` PDF schematic diagram illustrating the vehicle's circuitry.  
`src:` code of control software for all components programmed to participate.  
`t-photos:` 2 photos of the team (an official one and a funny photo).  
`v-photos:` 6 photos of the vehicle (from every side, top and bottom).  
`video:` video.md file with the link to a n second long driving demonstration.

# Solution

## Mechanism
For our prototype's motor, we used a DC motor gear system, where the motor is connected to a differential wrist mechanism that drives the back wheels and provides consistent, even movement. The front wheels do not receive any direct power from the motor, instead both the front wheels are hooked to the servo by a simple mechanism that we designed and 3d printed, with this we can proportionally steer the vehicle by turning both wheels at the same direction, at the same time.

## Electronic components
_For a more detailed explanation on the vehicle's circuitry, see [/schemes/README.md](./schemes/README.md)._
| Component  | Quantity |
| --- | --- |
| Arduino Uno  | 1  |
| Raspberry Pi 3  | 1  |
| Raspberry Pi Camera Module 2  | 1  |
| L298N Motor Driver | 1  |
| Hobby Gearmotor  | 1  |
| HC-SR04 Ultrasonic Sensor  | 3  |
| SG90 Micro Servo  | 1  |

The circuit is centered around our controller, the Arduino Uno, managing all components in the circuit to execute adequate responses. For navigation, we use three HC-SR04 Ultrasonic Sensors in order to use the measured distances as limits where the vehicle should react swiftly. Each sensor has its own echo and ground pin to avoid reliability issues, while sharing common power and trigger pins. Steering is managed by the SG90 Micro Servo, which provides precise 180-degree control, avoiding obstacles or staying in its route based on calculations from sensor data. To move the vehicle, an L298N motor driver hooked to a separate 9V battery powers and regulates the speed of the Hobby Gearmotor.

In order to identify the color of the obstacles and the parking, a Raspberry Pi 3 with a Raspberry Pi Camera Module detects objects through image processing. Applying color masks allows the Pi to see the object as a simple block and know its color, which it then communicates to the Arduino via USB serial communication. The Arduino then integrates the sensor and camera data to make quick decisions, such as amount of time it should steer or on what direction, allowing for precise maneuvering of the vehicle during rounds.

## Code
_For a more detailed explanation on the processes and logic behind the vehicle's source code, see [/src/README.md](./src/README.md)._
