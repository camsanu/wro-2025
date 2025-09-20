char rChar;
bool received = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read and print
  if (!received && Serial.available() > 0) {
    rChar = Serial.read();
    Serial.println(rChar);
    received = true;
  }
  // reset received if no new data
  if (received && Serial.available() == 0) {
    received = false;
  }
}
