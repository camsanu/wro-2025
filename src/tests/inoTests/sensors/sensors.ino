
const int trigPin = 8, echoPin_r = 6, echoPin_l = 7, echoPin_f = 9; // ultrasonic pins
const int buttonPin = 11; 

int lDistance, rDistance, fDistance;

int numReadings = 3; // samples
bool pushEd = false; // button state
int pushEs = 0; // button state counter

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

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin_f, INPUT);
  pinMode(echoPin_l, INPUT);  
  pinMode(echoPin_r, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
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
    sum += (duration * 0.034 / 2);
  }
    return sum / numReadings;
  }


void loop() {
  if (digitalRead(buttonPin) == LOW) { // save button state
    pushEd = true;
    pushEs++;
    if (pushEs == 2) { // reset button state
      pushEd = false; 
      pushEs = 0;
    }
  }
  while (pushEd == true) {
    saveDistances();
    printDistances();
  }
}