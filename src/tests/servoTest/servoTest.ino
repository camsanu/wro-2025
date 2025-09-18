#include <Servo.h>

Servo myservo;

const int buttonPin = 8;
const int sMin = 80, sMax = 180, sCenter = 130; // servo limits

bool pushEd = false; // button state
int pushEs = 0; // button state counter

void setup(){
  pinMode(buttonPin, INPUT_PULLUP);
  myservo.attach(6);
  Serial.begin(9600);
}

void loop(){
  if(digitalRead(buttonPin) == LOW){ // save button state
    pushEd = true;
  }
  if(pushEd == true){
    Serial.println("Right");
    myservo.write(sMin);
    delay(1000);
    Serial.println("Center");
    myservo.write(sCenter);
    delay(1000);
    Serial.println("Left");
    myservo.write(sMax);
    delay(1000);
  }
}
