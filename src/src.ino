#include <Servo.h>

#define motFwd  7
#define motRev  6

// rgb red (238, 39, 55)
// rgb green (68, 214, 44)
// rgb mag (255, 0, 255)

Servo myservo;

const int trigPin = 12, echoPin_f = 11, echoPin_r = 10, echoPin_l = 9; // ultrasonic pins
const int buttonPin = 8;
const int sMin = 50, sMax = 130, sCenter = 90; // servo limits
const int threshold = 100; // distance threshold

int lDistance, rDistance, fDistance;
int corners = 0; // corners turned so far
float Gp = 0.6; // proportional gain
float Gd = 1; // derivative gain
float Gi = 0.02; // integral gain
static float prevError = 0;
static float integ = 0; // store integral

int numReadings = 3; // samples

bool pushEd = false; // button state
int pushEs = 0; // button state counter
bool lastState = HIGH; // save button state
bool red = false; // obj detection
bool green = false; 

long getDistance(int echoPin){ // distance capture function
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
  myservo.write(sCenter);
  Serial.println("Over");
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
  myservo.write(sCenter);
  Serial.println("End of Servo calibration"); 
}

void lTurn(){ // turn left corner
  static unsigned long turnStart = 0; // when the turn started
  static bool turning = false;
  if(!turning){
    myservo.write(0);
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
  static unsigned long turnStart = 0; // when the turn started
  static bool turning = false;
  if(!turning){
    myservo.write(180);
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

// setup function

void setup() {
  pinMode(motFwd, OUTPUT);
  pinMode(motRev, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin_f, INPUT);
  pinMode(echoPin_r, INPUT);  
  pinMode(echoPin_l, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  myservo.attach(13);
  Serial.begin(9600);

  mCalib();
  sCalib();
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

  if(pushEd == true){
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
  
    if(corners==4 && (!red && !green)){
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
  if(pushEd == false && pushEs > 1){
    endP();
  }
}
