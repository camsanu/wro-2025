Control software
===

This directory contains all code used for the controllers in the vehicle. Additionally, the `tests` folder contains simple, 
component-specific code to debug logic in the source code and verify the proper functioning of all components involved 
individually.

## `src.ino`
Code uploaded to the Arduino Uno to control all sensors, motors and overall movement of the vehicle.

## `src.py`
Code uploaded to the Raspberry Pi to identify color obstacles and send this data to the Arduino Uno.
