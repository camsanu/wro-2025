#include <Servo.h>

Servo myservo;
const int sMin = 50, sMax = 130, sCenter = 90; // servo limits; left, right, center respectively

// motor pins
#define IN1  4
#define IN2  2 
#define ENA 3

// ultrasonic pins and distance variables (all distances are in cm)
const int trigPin = 12, echoPin_f = 11, echoPin_r = 10, echoPin_l = 9;
int lDistance, rDistance, fDistance;
float startL, startR, startF;
int numReadings = 3; // samples for reading averaging
const int threshold = 100; // threshold for turning
const float tolerance = 5.0; // tolerance for returning to initial position

// button variables
const int buttonPin = 8;
bool pushEd = false;
int pushEs = 0;
bool lastState = HIGH;

// pid control
int corners = 0, laps = 0;
float Gp = 0.6; // proportional gain
float Gd = 1; // derivative gain
float Gi = 0.02; // integral gain
static float prevError = 0;
static float integ = 0;

// object detection
bool red = false;
bool green = false;
bool parking = false;
int parks = 0; // initial or final parking

// distance and control functions
long getDistance(int echoPin){ // ultrasonic distance capture
    long duration;
    long sum = 0;

    for(int i = 0; i < numReadings; i++){
      digitalWrite(trigPin, LOW);
      delay(2);
      digitalWrite(trigPin, HIGH);
      delay(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH, 20000);
      sum += (duration * 0.034 / 2);
    }
  return sum / numReadings;
}

void saveDistances(){
  fDistance = getDistance(echoPin_f);
  lDistance = getDistance(echoPin_l); 
  rDistance = getDistance(echoPin_r);
}

void saveStart(){ // get initial position
  startF = getDistance(echoPin_f);
  startL = getDistance(echoPin_l); 
  startR = getDistance(echoPin_r);
}

void printDistances(){
  Serial.print("F ");
  Serial.print(fDistance);
  Serial.print("; L ");
  Serial.print(lDistance);
  Serial.print("; R ");
  Serial.println(rDistance);
}

// motor and movement control
void fwd(){
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 255);
}

void rev(){
  digitalWrite(IN2, LOW); 
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 255);
}

void dead(){
  digitalWrite(IN2, LOW); 
  digitalWrite(IN1, LOW);
}

void endP(){
  dead();
  myservo.write(sCenter);
  Serial.println("Over");
  for(;;);
}

void lTurn(){
  static unsigned long turnStart = 0;
  static bool turning = false;
  if(!turning){ 
    myservo.write(sMin); // turn left
    turnStart = millis(); // start timing turn
    turning = true;
    Serial.println("Turning Left");
  }

  if(millis() - turnStart >= 800){ // finish turn after set time
    if(lDistance < threshold){
      myservo.write(sCenter); // recenter
      corners++; // count corner as done
      Serial.println("Turned Left");
    }
    turning = false;
  }
}

void rTurn(){ // same as above
  static unsigned long turnStart = 0;
  static bool turning = false;
  if(!turning){
    myservo.write(sMax);
    turnStart = millis();
    turning = true;
    Serial.println("Turning Right");
  }

  if(millis() - turnStart >= 800){
    if(rDistance < threshold){
      myservo.write(sCenter);
      corners++;
      Serial.println("Turned Right");
    }
    turning = false;
  }
}

void redTurn(){ // red obstacle turn
  static unsigned long turnStart = 0;
  static bool turning = false;
  if(!turning){
    myservo.write(sMax); // turn right
    turnStart = millis(); // start timing turn
    turning = true;
    Serial.println("Avoiding Red");
  }

  if(millis() - turnStart >= 300){ // shorter set time than corner turns to make up for space
    if(rDistance < threshold){
      myservo.write(sCenter); // recenter
      Serial.println("Avoided Red");
      red = false; // reset red obstacle detection
    }
    turning = false;
  }
}

void greenTurn(){ // same as above
  static unsigned long turnStart = 0;
  static bool turning = false;
  if(!turning){
    myservo.write(sMin);
    turnStart = millis();
    turning = true;
    Serial.println("Avoiding Green");
  }

  if(millis() - turnStart >= 300){
    if(lDistance < threshold){
      myservo.write(sCenter);
      Serial.println("Avoided Green");
      green = false;
    }
    turning = false;
  }
}

void unparkLeft(){
  myservo.write(sMin);
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 200); // adjust speed for softer exit
  if(fDistance <= 10){ // avoid hitting wall
    dead();
  }
  Serial.println("Exiting Parking: Stage One");

  if(lDistance < rDistance){
    myservo.write(sMax); // turn right
    analogWrite(ENA, 255); // return to normal speed
    Serial.println("Exiting Parking: Stage Two");
  }
  if(lDistance == rDistance){
    myservo.write(sCenter); // recenter
  }
}

void unparkRight(){ // same as above
  myservo.write(sMax);
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 200);
  if(fDistance <= 10){
    dead();
  }
  Serial.println("Exiting Parking Stage One");
  
  if(lDistance > rDistance){
    myservo.write(sMin);
    fwd();
    Serial.println("Exiting Parking Stage Two");
  }
  if(lDistance == rDistance){
    myservo.write(sCenter);
  }
}

void parkLeft(){
    digitalWrite(IN2, HIGH); 
    digitalWrite(IN1, LOW);
    analogWrite(ENA, 200); // adjust speed for softer exit
    myservo.write(sMax); // turn right to enter
    if(fDistance <= 10){ // avoid hitting wall
      dead();
    }
    if(lDistance < rDistance){
      myservo.write(sMin); // adjust to get straight parking
      if(lDistance == rDistance){
        myservo.write(sCenter); // recenter
        dead(); // standby
      }
    }
}

void parkRight(){ // same as above
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 200);
  myservo.write(sMax);
  if(fDistance <= 10){
    dead();
  }
  if(lDistance < rDistance){
    myservo.write(sMin);
    if(lDistance == rDistance){
      myservo.write(sCenter);
      dead();
    }
  }
}


// setup function
void setup() {
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin_f, INPUT);
  pinMode(echoPin_r, INPUT);  
  pinMode(echoPin_l, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  myservo.attach(6);
  Serial.begin(9600);
  saveStart();
  dead();
}

// main loop
void loop() {
  bool buttonState = digitalRead(buttonPin);
  if(lastState == HIGH && buttonState == LOW){ 
    pushEs++;
    if(pushEs == 1){
      pushEd = true;
      Serial.println("Started");
    } else if(pushEs > 1){
      pushEd = false;
      Serial.println("Killed");
    }
    delay(50); // debounce
  }
  lastState = buttonState;

  if(pushEd){
    if(Serial.available() > 0){ // check for raspi color input
      char inChar = (char)Serial.read();
      if(inChar == 'R'){
        red = true;
        Serial.println("R"); // respond to raspi input for debugging
      }
      else if(inChar == 'G'){
        green = true;
        Serial.println("G");
      }
      else if(inChar == 'M'){
        parking = true;
        Serial.println("M");
      }
    }

    if(parking && parks == 0){ // checks if inside parking
      parks++; // initial parking done
      if(lDistance > rDistance){
        unparkLeft();
      } else if(rDistance > lDistance){
        unparkRight();
      }
    }

    if(parking && parks == 1){ // checks if outside parking
      if(lDistance > rDistance){
        parkLeft();
      } else if(rDistance > lDistance){
        parkRight();
      }
      endP();
    }

    fwd();
    saveDistances();
    printDistances();

    if(lDistance == rDistance){ // stay on the track's center lane by default
      myservo.write(sCenter);
    }
  
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

    // obstacle detection
    if(red || green){ 
      if(red){
        redTurn();
      }
      else if(green){
        greenTurn();
      }
    }

    if(fDistance < threshold && (!red && !green)){ // check if at open corner
      if(lDistance > rDistance){
        lTurn();
        integ = 0; // reset to avoid integral windup
      }
      else if(rDistance > lDistance){
        rTurn();
        integ = 0;
      }
    }
    
    if(corners == 4){ // reset corners after finishing lap
      laps++;
      corners = 0;
    }
  
    if(laps==3 && (!red && !green)){ // natural end, finished laps and no objects
      bool closeF = abs(fDistance-startF) <= tolerance; // checks if initial and current position are within tolerance range
      bool closeL = abs(lDistance-startL) <= tolerance;
      bool closeR = abs(rDistance-startR) <= tolerance;
      if(closeF && closeL && closeR){ // begin shut down
        static unsigned long shutStart = 0; 
        static bool shutting = false;
        if(!shutting){
          shutStart = millis();
          shutting = true;
          Serial.println("Shutting down");
        }
        if(millis() - shutStart >= 800){ // wait before shutting down
          Serial.println("Shut down complete");
          shutting = false;
          endP(); // end program
        }
      }
    }
  } 
  if(pushEd == false && pushEs > 1){ // kill switch
    endP();
  }
}
