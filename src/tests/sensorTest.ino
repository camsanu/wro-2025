const int trigPin = 8;
const int echoPin_1 = 6; // right sensor echo
const int echoPin_2 = 7; // left sensor echo
const int echoPin_3 = 9; // front sensor echo
// Pin del botón
const int buttonPin = 11;

int fDistance; // front
int lDistance; // left 
int rDistance; // right
int turns = 0; // number of corners turned

int numReadings = 3; // number of samples before printing
bool pushEd = false;



void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(echoPin_2, INPUT);  
  pinMode(echoPin_3, INPUT);

  // Configurar el botón con resistencia pull-up interna
  pinMode(buttonPin, INPUT_PULLUP);
}

long getDistance(int echoPin){ // captures distance for an n number of times and averages them for more consistent output
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
    // Medir distancias con cada sensor
    fDistance = getDistance(echoPin_3); // collect distances
    lDistance = getDistance(echoPin_2); 
    rDistance = getDistance(echoPin_1);
  
    Serial.print("FLR: "); // print them out to serial monitor
    Serial.print(fDistance);
    Serial.print(" ");
    Serial.print(lDistance);
    Serial.print(" ");
    Serial.println(rDistance);

    Serial.println("Forward");
  }
}
