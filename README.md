Engineering materials
====

This repository contains all engineering materials for team Wired Wizards, from CETYS Universidad campus Mexicali team Sentinel Fox Engineering, compeeting on WRO Future Engineers 2025 season, soon to go to the 2025 Mexican National.

# Team Members
* Yumian Del Mar Rodríguez Téllez
* Ahn Salgado
* Constanza Sandoval
* `Coach` Leonardo Médina

## Content


* `models` is for the files for models used by 3D printers to produce the vehicle elements.
* `schemes` contains a PDF schematic diagram illustrating all the electronic components and motors used in the vehicle and how they connect to each other.
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
| Servo Motor  | 1  |

## Mechanism
For our prototype's motor, we use a DC motor gear system, the motor is connected to a differential wrist mechanism that moves the back wheels. Our front wheels don't get any direct power from the motor but instead we use proportional steering with our Servo. For this to work the sensors send a signal to the Servo that's connceted to the wheels so it can turn evenly depending on how strong the signal is. This gives smooth, precise control instead of just full left or full right steering.
