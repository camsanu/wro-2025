char dataString[50] = {0};
int a = 0; 

void setup() {
Serial.begin(9600);
}
  
void loop() {
  a++;
  sprintf(dataString,"%02X",a); // convert to hexa 
  Serial.println(dataString); // send
  delay(1000);
}
