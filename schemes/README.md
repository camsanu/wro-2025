# Schemes
On this directory you can find the schematic diagram for the vehicle's circuitry `scheme.pdf`, and an additional more visual 
one saved as `diagram.jpg`. We'll go into detail into the connections and functionality below.

## Arduino Uno
The main controller of the circuit. It receives the data from all other components on the circuit and determines what to do 
with it by following the corresponding programming (see [src/README.md](/src/README.md)). Its used pins are separated in two 
types, analog and digital, out of which we mainly use digital pins. The difference between them is digital pins allow for 
signals where a binary state is needed, meanwhile analog ones allow for continuous voltage output using PWM (Pulse Width 
Modulation).

## HC-SR04 Ultrasonic Sensors
The main navigation tool of the circuit, collecting distances by measuring the time it takes an emitted sound wave to bounce 
back into the sensor. The three of them have their own echo pins and grounds on the Arduino board, and they all share a power 
and trigger pin. The lack of common grounds for the sensors came about as we found out making them share a ground pin would 
often make the sensors go unresponsive due to unforeseen wiring issues, leading us to leaving them independent of each other.

## SG90 Micro Servo
The motor in charge of steering the vehicle into the right positions. It allows for precise angle orientation in a 180 degree 
span, making it so we are able to use formulas involving the distances taken with our ultrasonic sensors to derive the needed
angle at any given moment, whether it is to ensure the vehicle goes in a straight line, or to avoid crashing into walls or 
obstacles. Connecting the ground pin and the power pin to 5V, besides a connection to a PWM-enabled digital pin (in our case 
number 6) is enough to establish a proper connection to the Arduino.

## L298N Motor Driver
The regulator responsible for supplying enough power to move the motors, as the power the controllers can provide is not 
enough to support the full weight of the vehicle. The motor driver allows us to not only supply this additional power without
feeding it back into the circuit and risk damaging components running at lower voltages, but also to command the motor and
regulate its speed using the INPUT and ENABLE pins, which are connected to the Arduino.

## Raspberry Pi 3	
The computer vision of the circuit. The Pi Camera Module is plugged into the Raspberry Pi directly through its camera port, 
allowing for no-fuzz connection to the Pi. In the Pi (see [src/README.md](/src/README.md)) we execute code to apply masks over 
the captured image in order to derive the existence of specifically colored objects, those being the obstacles. The Pi then 
sends this data through serial communication by USB with the Arduino, who will then process it and react accordingly. 
