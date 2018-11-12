int pinSensor = 2;

void setup() {
  Serial.begin(9600); 
  pinMode(pinSensor, INPUT);
}

void loop() {
  int pirValue = digitalRead(pinSensor); 
  Serial.println("Valor: " + String(pirValue));
  
  if (pirValue == HIGH) {
    Serial.println("Detectado movimiento");
  } else {
    Serial.println("- - - -");
  }
  
  delay(50);
}
