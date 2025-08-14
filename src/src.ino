#include <Servo.h>

#define motFwd1  2
#define motRev1  3
#define motFwd2  4
#define motRev2  5

Servo myservo;

const int trigPin = 8, echoPin_r = 6, echoPin_l = 7, echoPin_f = 9; // ultrasonic pins
const int buttonPin = 11;

int lDistance, rDistance, fDistance;
int corners = 0; // amount of corners turned so far

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
  digitalWrite(motFwd1, HIGH); 
  digitalWrite(motRev1, LOW);
  digitalWrite(motFwd2, HIGH); 
  digitalWrite(motRev2, LOW);
}

void rev(){
  digitalWrite(motFwd1, LOW); 
  digitalWrite(motRev1, HIGH);
  digitalWrite(motFwd2, LOW); 
  digitalWrite(motRev2, HIGH);
}

void dead(){
  digitalWrite(motFwd1, LOW); 
  digitalWrite(motRev1, LOW);
  digitalWrite(motFwd2, LOW); 
  digitalWrite(motRev2, LOW);
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
    Serial.println("Corner");
  }
}

void rTurn(){ // turn right corner
  myservo.write(180);
  delay(800);
  if(rDistance < 100){
    myservo.write(90);
    corners++;
    Serial.println("Corner");
  }
}

// setup function

void setup() {
  pinMode(motFwd1, OUTPUT); 
  pinMode(motFwd2, OUTPUT);
  pinMode(motRev1, OUTPUT); 
  pinMode(motRev2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin_f, INPUT);
  pinMode(echoPin_r, INPUT);  
  pinMode(echoPin_l, INPUT);

  myservo.attach(10);
  Serial.begin(9600);

  mCalib();
  sCalib();
}

// main loop

void loop() {
  if(digitalRead(buttonPin) == LOW){ // save button state
    pushEd = true;
    pushEs++;
    if (pushEs == 2) { // reset button state
      pushEd = false; 
    }
  }

  while(pushEd == true){
    fwd();
    Serial.println("Forward");
    saveDistances();
    printDistances();

    while(lDistance == rDistance){
      myservo.write(90); // straight
    }
  
    while(lDistance != rDistance && fDistance>100 && (red && green == false)){ // overshoot
      if(lDistance > rDistance){
        myservo.write(110);
        Serial.println("Left Overshoot");
      }
      else if(rDistance > lDistance){
        myservo.write(70);
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
}
