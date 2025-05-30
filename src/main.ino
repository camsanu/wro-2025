#include <Servo.h>

Servo myservo;

#define MotFwd1  2  // Motor 1 Forward pin
#define MotRev1  3 // Motor 1 Reverse pin
#define MotFwd2  4  // Motor 2 Forward pin
#define MotRev2  5 // Motor 2 Reverse pin

const int trigPin = 8;
const int echoPin_1 = 6; // right sensor echo
const int echoPin_2 = 7; // left sensor echo
const int echoPin_3 = 9; // front sensor echo

int n = 0; // amount of times loop has executed
int b = 10; // amount of times loop must execute before parking
bool parkEd = false; // car hasn't parked yet
bool lane; // stores what lane the car is running in
int turns = 0;

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
  myservo.attach(10); // servo pin
  Serial.begin(9600); // start serial comunication
  myservo.write(103); // reset servo position
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

void endP() { // ends the route
  stop();
  while (true) {
  }
}

void loop() {
  if(getDistance(echoPin_2)>getDistance(echoPin_1)){ // checks if the car is on the left lane or right lane
    lane = false; // right lane
  } else{
    lane = true; // left lane
  }

  if(getDistance(echoPin_3)>=100){ // checks if there is space to move forward
	  forward();
  }

  if(getDistance(echoPin_2)>getDistance(echoPin_1) && getDistance(echoPin_3)<100){ // checks if there is space to turn left and no more space to move forward
	  myservo.write(0); // turns left
	  if(getDistance(echoPin_2)<100){ // checks if there  is no more space to turn left
      myservo.write(103); // reset servo position
      turns++; // counts a turn
    }
  } 
  
  if(getDistance(echoPin_2)<getDistance(echoPin_1) && getDistance(echoPin_3)<100){ // checks if there is space to turn right
    myservo.write(180); // turns right
    if(getDistance(echoPin_1)<100){ // checks if there is no more space to turn right
      myservo.write(103); // reset servo position
      turns++;
    }
  }
  
  if(turns=4){
    endP();
  }
}
