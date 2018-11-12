void setup() {
  // No hace falta declarar las lecturas analógicas 
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // Leer lectura analógica
  int lightValue = analogRead(A4); 
  Serial.println("Valor: " + String(lightValue));
  
  if (lightValue < 200) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
