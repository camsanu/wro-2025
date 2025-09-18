#include <Servo.h>

Servo myservo;

const int buttonPin = 8;
bool pushEd = false; // button state
int pushEs = 0; // button state counter

void setup(){
  pinMode(buttonPin, INPUT_PULLUP);
  myservo.attach(13);
  Serial.begin(9600);
}

void loop(){
  if(digitalRead(buttonPin) == LOW){ // save button state
    pushEd = true;
    pushEs++;
    if(pushEs == 2){ // kill switch
      pushEd = false;
    }
  }
  if(pushEd == true){
    Serial.println("Right");
    myservo.write(0);
    delay(1000);
    Serial.println("Center");
    myservo.write(90);
    delay(1000);
    Serial.println("Left");
    myservo.write(180);
    delay(1000);
  }
  if(pushEd == false && pushEs > 1){
    Serial.println("Killed");
  }
}