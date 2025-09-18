
#define IN2Fwd 2
#define IN1Rev 4
#define ENA 3

void setup() {
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(ENA, OUTPUT);

  Serial.begin(9600);
  Serial.println("Started");
}

void loop() {
  Serial.println("Forward: 100%");
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 255);
  delay(2000);
  Serial.println("Stopped");
  digitalWrite(IN2, LOW);
  digitalWrite(IN1, LOW);
  delay(2000);
  Serial.println("Reverse: 100%");
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 255);
  delay(2000);
  Serial.println("Stopped");
  digitalWrite(IN2, LOW);
  digitalWrite(IN1, HIGH);
  delay(2000);
}
