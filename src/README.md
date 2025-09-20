Control software
===

This directory contains all code used for the controllers in the vehicle. Additionally, the `tests` folder contains simple, 
component-specific code to debug logic in the source code and verify the proper functioning of all components involved 
individually. Code is to be downloaded into the controllers by USB connection, for both the Arduino Uno and the Raspberry Pi.

# `src.ino`
Code uploaded to the Arduino Uno to control all sensors, motors and overall movement of the vehicle.
## Round Start and Parking
## Obstacle Identification
## Auto-Alignment
The vehicle is programmed to automatically go towards the middle lane of the track, independently of its starting position or orientation, as we see below: 

```c++
    // line 327
    fwd();
    saveDistances();
    printDistances();

    if(lDistance == rDistance){ // stay on the track's center lane by default
      myservo.write(sCenter);
    }
```

Naturally, the vehicle will start drifting from its straight line due to deformities on the track, wheels drifting, mistimed corner stops, etcetera, and if no corrections occur the vehicle might end up hitting obstacles or walls due to being miscalibrated. Therefore, we applied PID control in order to keep our servo's angle shifting enough to compensate. See the following code, which will be explained step by step:

```c++
// line 27
float Gp = 0.6; // proportional gain
float Gd = 1; // derivative gain
float Gi = 0.02; // integral gain
static float prevError = 0;
static float integ = 0;

// line 335
if(lDistance != rDistance && fDistance>threshold && (!red && !green)){ // check if vehicle is drifting away from center
      float error = rDistance - lDistance; // get error margin
      float deriv = error - prevError; // get change in error
      float servoAngle = sCenter + (Gp * error) + (Gd * deriv) + (Gi * integ); // pid control
      prevError = error;
      integ += error; // get cumulative error
      servoAngle = constrain(servoAngle, sMin, sMax); // limit angle to safe range
      integ = constrain(integ, -threshold, threshold); // limit integral windup
      if(lDistance > rDistance){ // print direction of overshoot for debugging
        Serial.println("Left Overshoot");
      }
      else if(rDistance > lDistance){
        Serial.println("Right Overshoot");
      }
      myservo.write(servoAngle);
    }
```
Starting off by variables, we have 5 variables that will determine how strongly each PID term affects the final angle:
```c++
// line 27
float Gp = 0.6; // proportional gain
float Gd = 1; // derivative gain
float Gi = 0.02; // integral gain
static float prevError = 0;
static float integ = 0;
```
The proportional gain reacts to how wide the current error is, that is, how offcenter the vehicle is at that moment; The derivative gain handles the change in error, supressing very sudden swings; The integral gain reacts to the sum of past errors, in order fix light offsets. These variables are necessary to make sure the correction isn't too relaxed offset builds up, nor too strict it slows down the vehicle by oscillating. The next two variables are static variables (that is, they keep their value through multiple loop iterations) that need to be updated to ensure the formula keeps working as intended.

## Steering

# `src.py`
Code uploaded to the Raspberry Pi to identify color obstacles and send this data to the Arduino Uno.
