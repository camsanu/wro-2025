Engineering materials
====

This repository contains all engineering materials for team Wired Wizards, from CETYS Universidad campus Mexicali as part of Sentinel Fox Engineering, competing on the WRO Future Engineers 2025 season, soon to go to the 2025 Mexican National.

## Team Members
* Yumian Del Mar Rodríguez Téllez
* Ahn Montserrat Salgado González
* Constanza Sandoval Ureña
* `Coach` Leonardo Daniel Médina Madrazo

## Content


* `models` is for the files for models used by 3D printers to produce the vehicle elements.
* `schemes` contains a PDF schematic diagram illustrating all the electronic components used in the vehicle and their connections.
* `src` contains code of control software for all components which were programmed to participate in the competition.
* `t-photos` contains 2 photos of the team (an official one and one funny photo with all team members).
* `v-photos` contains 6 photos of the vehicle (from every side, top and bottom).
* `video` contains the video.md file with the link to a n second long driving demonstration.


## Introduction

_This part must be filled by participants with the technical clarifications about the code: which modules the code consists of, how they are related to the electromechanical components of the vehicle, and what is the process to build/compile/upload the code to the vehicle’s controllers._

## Electronic components

| Component  | Quantity |
| --- | --- |
| Arduino Uno  | 1  |
| Raspberry Pi 3  | 1  |
| Raspberry Pi Camera Module 2  | 1  |
| L298N Motor Driver | 1  |
| Hobby Gearmotor  | 1  |
| HC-SR04 Ultrasonic Sensor  | 3  |
| SG90 Micro Servo  | 1  |

## Mechanism
For our prototype's motor, we used a DC motor gear system, where the motor is connected to a differential wrist mechanism that moves the back wheels. Our front wheels do not get any direct power from the motor, instead being hooked to the servo to proportionally steer the vehicle.
