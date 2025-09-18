const int trigPin = 12, echoPin_f = 5, echoPin_r = 10, echoPin_l = 9;
const int buttonPin = 8;

int fDistance, rDistance, lDistance;

int numReadings = 3; // number of samples before printing
bool pushEd = false;



void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin_f, INPUT);
  pinMode(echoPin_r, INPUT);  
  pinMode(echoPin_l, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

long getDistance(int echoPin){
    long duration;
    int sum = 0;

    for(int i = 0; i < numReadings; i++){
      digitalWrite(trigPin, LOW);
      delay(2);
      digitalWrite(trigPin, HIGH);
      delay(10);
      digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    sum += (duration * 0.034 / 2); // converts distance to cm
  }
    return sum / numReadings;
  }


void loop() {
  if (digitalRead(buttonPin) == LOW) {
    pushEd = true;
  }
  if (pushEd == true) {
    fDistance = getDistance(echoPin_f);
    lDistance = getDistance(echoPin_l); 
    rDistance = getDistance(echoPin_r);
  
    Serial.print("FLR: ");
    Serial.print(fDistance);
    Serial.print(" ");
    Serial.print(lDistance);
    Serial.print(" ");
    Serial.println(rDistance);
  }
}
