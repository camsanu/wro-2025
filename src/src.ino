#include <Servo.h>

#define motFwd  2
#define motRev  3

Servo myservo;

const int trigPin = 8, echoPin_r = 6, echoPin_l = 7, echoPin_f = 9; // ultrasonic pins
const int buttonPin = 11;

int lDistance, rDistance, fDistance;
int corners = 0; // amount of corners turned so far
int Gp = 1; // proportional gain
int Gd = 1; // derivative gain
int Gi = 1; // integral gain
static int prevErr = 0; // store previous for derivative
static int integ = 0; // store integral

int numReadings = 3; // samples

bool pushEd = false; // button state
int pushEs = 0; // button state counter
bool red = false; // obj detection
bool green = false; 

long getDistance(int echoPin){ // distance capture function
    long duration;
    int sum = 0;

    for(int i = 0; i < numReadings; i++){
      digitalWrite(trigPin, LOW);
      delay(2);
      digitalWrite(trigPin, HIGH);
      delay(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);
      sum += (duration * 0.034 / 2);
    }
  return sum / numReadings;
}

void saveDistances(){
  fDistance = getDistance(echoPin_f);
  lDistance = getDistance(echoPin_l); 
  rDistance = getDistance(echoPin_r);
}

void printDistances(){
  Serial.print("F "); // print to serial
  Serial.print(fDistance);
  Serial.print("; L ");
  Serial.print(lDistance);
  Serial.print("; R ");
  Serial.println(rDistance);
}

// motor control
void fwd(){
  digitalWrite(motFwd, HIGH); 
  digitalWrite(motRev, LOW);
}

void rev(){
  digitalWrite(motFwd, LOW); 
  digitalWrite(motRev, HIGH);
}

void dead(){
  digitalWrite(motFwd, LOW); 
  digitalWrite(motRev, LOW);
}

void endP() { // route natural end
  dead();
  Serial.println("Over");
  while (true) {
  }
}

void mCalib(){ // motor calibration
  Serial.println("Start Motor calibration");
  Serial.println("Forward check");
  fwd();
  delay(100);
  dead();
  delay(100);
  Serial.println("Reverse check");
  rev();
  delay(100);
  dead();
  Serial.println("End of Motor calibration");
}

void sCalib(){ // servo calibration
  Serial.println("Servo check");
  myservo.write(180);
  delay(100);
  myservo.write(90);
  Serial.println("End of Servo calibration"); 
}

void lTurn(){ // turn left corner
  myservo.write(0);
  delay(800);
  if(lDistance < 100){
    myservo.write(90);
    corners++;
    Serial.println("Turned Left");
  }
}

void rTurn(){ // turn right corner
  myservo.write(180);
  delay(800);
  if(rDistance < 100){
    myservo.write(90);
    corners++;
    Serial.println("Turned Right");
  }
}

// setup function

void setup() {
  pinMode(motFwd, OUTPUT);
  pinMode(motRev, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin_f, INPUT);
  pinMode(echoPin_r, INPUT);  
  pinMode(echoPin_l, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  myservo.attach(10);
  Serial.begin(9600);

  mCalib();
  sCalib();
  dead();
}

// main loop

void loop() {
  if(digitalRead(buttonPin) == LOW){ // save button state
    pushEd = true;
    pushEs++;
    Serial.println("Started");
    if(pushEs == 2){ // kill switch
      pushEd = false;
    }
  }

  if(pushEd == true){
    fwd();
    Serial.println("Forward");
    saveDistances();
    printDistances();

    while(lDistance == rDistance){
      myservo.write(90); // straight
    }
  
    while(lDistance != rDistance && fDistance>100 && (red && green == false)){ // overshoot
      int err = lDistance - rDistance;
      int deriv = err - prevErr;
      int servoAngle = 90 + (Gp * err) + (Gd * deriv) + (Gi * integ); // proportional + derivative control
      prevErr = err;
      integ += err;
      servoAngle = constrain(servoAngle, 50, 130); // limit to safe range
      integ = constrain(integ, -100, 100); // limit integral windup
      if(lDistance > rDistance){
        myservo.write(servoAngle);
        Serial.println("Left Overshoot");
      }
      else if(rDistance > lDistance){
        myservo.write(servoAngle);
        Serial.println("Right Overshoot");
      }
    }

    if(fDistance < 100 && (red && green == false)){
      if(lDistance > rDistance){ // left turn
        lTurn();
      }
      else if(rDistance > lDistance){ // right turn
        rTurn();
      }
    } 
  
    if(corners==4 && (red && green == false)){
      delay(800);
      endP();
    }
  } 
  if(pushEd == false && pushEs < 0){ // stop motors
    dead();
    Serial.println("Killed");
  }
}
