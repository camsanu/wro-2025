// Pines del puente H
#define ENA 3   // PWM para velocidad
#define IN1 4   // Dirección
#define IN2 2   // Dirección

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);

  Serial.begin(9600);   // Inicia comunicación serial
  Serial.println("Iniciando control de motor...");
}

void loop() {
  // ---- Giro hacia adelante ----
  Serial.println("Motor: Adelante al 100%");
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 255);   // Velocidad máxima
  delay(2000);
}