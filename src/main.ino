#include <Wire.h>
#include <Servo.h>

Servo myservo;

#define MotFwd1  2  // Motor 1 Forward pin
#define MotRev1  3 // Motor 1 Reverse pin
#define MotFwd2  4  // Motor 2 Forward pin
#define MotRev2  5 // Motor 2 Reverse pin

const int trigPin = 9;
const int echoPin_1 = 6; // right sensor echo
const int echoPin_2 = 7; // left sensor echo
const int echoPin_3 = 8; // front sensor echo

int n = 0; // amount of times loop has executed
int b = 10; // amount of times loop must execute before parking
bool parkEd = false; // car hasn't parked yet

int numReadings = 3; // number of samples before printing


void setup() {
  pinMode(MotFwd1, OUTPUT); 
  pinMode(MotRev1, OUTPUT); 
  pinMode(MotFwd2, OUTPUT); 
  pinMode(MotRev2, OUTPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(echoPin_2, INPUT);  
  pinMode(echoPin_3, INPUT);
  myservo.attach(5); // servo pin
  Serial.begin(9600); // start serial comunication
  myservo.write(90); // reset servo position
}

void forward(){
  digitalWrite(MotFwd1, HIGH); 
  digitalWrite(MotRev1, LOW);
  digitalWrite(MotFwd2, HIGH); 
  digitalWrite(MotRev2, LOW);
}

void reverse(){
  digitalWrite(MotFwd1, LOW); 
  digitalWrite(MotRev1, HIGH);
  digitalWrite(MotFwd2, LOW); 
  digitalWrite(MotRev2, HIGH);
}

void stop(){
  digitalWrite(MotFwd1, LOW); 
  digitalWrite(MotRev1, LOW);
  digitalWrite(MotFwd2, LOW); 
  digitalWrite(MotRev2, LOW);
}

void swerve(){}

void start(){ // exit parking sequence
  Serial.print("Start");
  myservo.write(180);
  for(int i = 0; i <= 10; i++){ //
    forward();
    Serial.print("Forward");
    stop();
  }
  delay(100);
  myservo.write(0);
  for(int i = 0; i <= 10; i++){ //
    reverse();
    Serial.print("Reverse"); 
    stop();
  }
  delay(100);
  myservo.write(90);
}

void loop() {
  n++;
  Serial.println(n);
  if(n >= b && !parkEd){
    park();
    parkEd = true;
  }

  forward();

  if(getDistance(echoPin_3)<15 && getDistance(echoPin_1)<5){
    myservo.write(180);
    delay(100);
    myservo.write(90);
  }

  if(getDistance(echoPin_3)<30 && getDistance(echoPin_3)>15 && getDistance(echoPin_1)>5){
    myservo.write(180);
    delay(200);
    myservo.write(90);
  }

  Serial.print("Distances FRL: ");
  Serial.print(getDistance(echoPin_3)); // distance from the front sensor
  Serial.print(" ");
  Serial.print(getDistance(echoPin_2)); // distance from the right sensor
  Serial.print(" ");
  Serial.println(getDistance(echoPin_1)); // distance from the left sensor
}

long getDistance(int echoPin){ // captures distance for an n number of times and averages them for more consistent output
    long duration;
    int sum = 0;

    for(int i = 0; i < numReadings; i++){
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    sum += (duration * 0.034 / 2); // converts distance to cm
  }
    return sum / numReadings;
  }

void park(){ // parking sequence
  Serial.print("Parking Start");
  myservo.write(0);
  for(int i = 0; i <= 10; i++){ //
    reverse();
    Serial.print("Reverse");
    stop();
  }
  delay(100);
  myservo.write(180);
  for(int i = 0; i <= 10; i++){ //
    forward();
    Serial.print("Forward");
    stop();
  }
  delay(100);
  myservo.write(90);
  Serial.print("Parking End");
  n = 0;
  exit();    
}

void exit() { 
  while (true) {
  }
}