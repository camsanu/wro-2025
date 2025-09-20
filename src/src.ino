#include <Servo.h>
// servo variables and requirements
Servo myservo;
const int sMin = 50, sMax = 130, sCenter = 90; // limits

// motor pins
#define IN2Fwd  2 
#define IN1Rev  4
#define ENA 3 // set speed with PWM

// ultrasonic pins and distance variables
const int trigPin = 12, echoPin_f = 11, echoPin_r = 10, echoPin_l = 9;
int lDistance, rDistance, fDistance;
float startL, startR, startF; // initial position
int numReadings = 3; // samples for reading averaging
const int threshold = 100; // threshold for turning
const float tolerance = 5.0; // tolerance for returning to initial position

// button variables
const int buttonPin = 8;
bool pushEd = false;
int pushEs = 0;
bool lastState = HIGH;

// pid control
int corners = 0, laps = 0; // turned so far
float Gp = 0.6; // proportional gain
float Gd = 1; // derivative gain
float Gi = 0.02; // integral gain
static float prevError = 0;
static float integ = 0; // store integral

// object detection
bool red = false;
bool green = false;
bool parking = false;
int parks = 0; // parking state

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

void saveStart(){
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
  digitalWrite(IN2Fwd, HIGH); 
  digitalWrite(IN1Rev, LOW);
  analogWrite(ENA, 255);
}

void rev(){
  digitalWrite(IN2Fwd, LOW); 
  digitalWrite(IN1Rev, HIGH);
  analogWrite(ENA, 255);
}

void dead(){
  digitalWrite(IN2Fwd, LOW); 
  digitalWrite(IN1Rev, LOW);
}

void endP() { // end program
  dead();
  myservo.write(sCenter);
  Serial.println("Over");
  for(;;);
}

void lTurn(){ // turn left corner
  static unsigned long turnStart = 0;
  static bool turning = false;
  if(!turning){
    myservo.write(sMin);
    turnStart = millis();
    turning = true;
    Serial.println("Turning Left");
  }

  if(millis() - turnStart >= 800){
    if(lDistance < threshold){
      myservo.write(sCenter);
      corners++;
      Serial.println("Turned Left");
    }
    turning = false;
  }
}

void rTurn(){ // turn right corner
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

void redTurn(){
  static unsigned long turnStart = 0;
  static bool turning = false;
  if(!turning){
    myservo.write(sMax);
    turnStart = millis();
    turning = true;
    Serial.println("Avoiding Red");
  }

  if(millis() - turnStart >= 300){
    if(rDistance < threshold){
      myservo.write(sCenter);
      Serial.println("Avoided Red");
      red = false;
    }
    turning = false;
  }
}

void greenTurn(){
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
  digitalWrite(IN2Fwd, HIGH); 
  digitalWrite(IN1Rev, LOW);
  analogWrite(ENA, 200);
  if(fDistance <= 10){
    dead();
  }
  Serial.println("Exiting Parking Stage One");

  if(lDistance < rDistance){
    myservo.write(sMax);
    fwd();
    Serial.println("Exiting Parking Stage Two");
  }
  if(lDistance == rDistance){
    myservo.write(sCenter);
  }
}

void unparkRight(){
  myservo.write(sMax);
  digitalWrite(IN2Fwd, HIGH); 
  digitalWrite(IN1Rev, LOW);
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
    digitalWrite(IN2Fwd, HIGH); 
    digitalWrite(IN1Rev, LOW);
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

void parkRight(){
  digitalWrite(IN2Fwd, HIGH); 
  digitalWrite(IN1Rev, LOW);
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
  pinMode(IN2Fwd, OUTPUT);
  pinMode(IN1Rev, OUTPUT);
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
  if(lastState == HIGH && buttonState == LOW){ // save button state
    pushEs++;
    if(pushEs == 1){
      pushEd = true;
      Serial.println("Started");
    } else if(pushEs > 1){ // kill switch
      pushEd = false;
      Serial.println("Killed");
    }
    delay(50); // debounce
  }
  lastState = buttonState;

  if(pushEd){
    if(Serial.available() > 0){
      char inChar = (char)Serial.read();
      if(inChar == 'R'){
        red = true;
        Serial.println("R");
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

    if(parking && parks == 0){
      parks++;
      if(lDistance > rDistance){
        unparkLeft();
      } else if(rDistance > lDistance){
        unparkRight();
      }
    }

    if(parking && parks == 1){
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

    if(lDistance == rDistance){
      myservo.write(sCenter);
    }
  
    if(lDistance != rDistance && fDistance>threshold && (!red && !green)){ // overshoot
      float error = lDistance - rDistance;
      float deriv = error - prevError;
      float servoAngle = sCenter + (Gp * error) + (Gd * deriv) + (Gi * integ); // pid control
      prevError = error;
      integ += error;
      servoAngle = constrain(servoAngle, sMin, sMax); // limit to safe range
      integ = constrain(integ, -threshold, threshold); // limit integral windup
      if(lDistance > rDistance){
        myservo.write(servoAngle);
        Serial.println("Left Overshoot");
      }
      else if(rDistance > lDistance){
        myservo.write(servoAngle);
        Serial.println("Right Overshoot");
      }
    }

    if(red || green){ // object detected
      if(red){
        redTurn();
      }
      else if(green){
        greenTurn();
      }
    }

    if(fDistance < threshold && (!red && !green)){
      if(lDistance > rDistance){ // left turn
        lTurn();
        integ = 0;
      }
      else if(rDistance > lDistance){ // right turn
        rTurn();
        integ = 0;
      }
    }
    
    if(corners == 4){
      laps++;
      corners = 0;
    }
  
    if(laps==3 && (!red && !green)){
      bool closeF = abs(fDistance-startF) <= tolerance;
      bool closeL = abs(lDistance-startL) <= tolerance;
      bool closeR = abs(rDistance-startR) <= tolerance;
      if(closeF && closeL && closeR){
        static unsigned long shutStart = 0; // when the shut down started
        static bool shutting = false;
        if(!shutting){
          shutStart = millis();
          shutting = true;
          Serial.println("Shutting down");
        }
        if(millis() - shutStart >= 800){
          Serial.println("Shut down complete");
          shutting = false;
          endP();
        }
      }
    }
  } 
  if(pushEd == false && pushEs > 1){
    endP();
  }
}
