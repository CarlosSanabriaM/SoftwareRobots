long distance; 
long responseTime;
int pinTrig = 4; 
int pinEcho = 5;

void setup() {
  Serial.begin(9600);
  pinMode(pinTrig, OUTPUT); /* Trig envía el pulso ultrasónico */ 
  pinMode(pinEcho, INPUT); /* Echo capta el rebote del pulso ultrasónico*/
}
void loop() {
  digitalWrite(pinTrig, LOW); /* Por seguridad volvemos a poner el Trig a LOW*/
  delayMicroseconds(5);
  
  digitalWrite(pinTrig, HIGH); /* Emitimos el pulso ultrasónico */ 
  delayMicroseconds(10);
  
  responseTime = pulseIn(pinEcho, HIGH); /* Medimos la longitud del pulso entrante: Cuanto tiempo tarda la entrada en pasar de HIGH a LOW retorna microsegundos */
  
  //Serial.println("Tiempo " + String(responseTime) + " microsegundos"); 
  
  distance = int(0.01716 * responseTime); /* Calcular la distancia conociendo la velocidad */
  
  Serial.println("Distancia " + String(distance) + "cm");
  delay(1000);
}
