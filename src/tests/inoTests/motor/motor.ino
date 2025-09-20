
#define ENA 3 // set speed with PWM
#define IN1 4 // reverse
#define IN2 2 // forward

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println("Forward Speed 100%");
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 255);
  delay(2000);
}
