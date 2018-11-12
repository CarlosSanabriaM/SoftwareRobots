#include <Servo.h>

Servo servo;
int pinServo = 8;

void setup() {
  Serial.begin(9600);
  servo.attach(pinServo);
}

void loop() {
  /*
      0º  -> Máxima velocidad sentido antihorario
      90º -> No se mueve
      180º-> Máxima velocidad sentido horario
      A pesar de las pausas, el servomotor sigue manteniendo el sentido de giro indicado
  */

  // Posición giro antihorario (0) esperar 5 s 
  Serial.println("Enviar: giro en sentido antihorario (max velocidad)"); 
  servo.write(0);
  delay(5000);
  
  // Posición detener (90) esperar 5 s
  Serial.println("Enviar: detener"); 
  servo.write(90);
  delay(5000);
  
  // Posición giro horario (180) esperar 5 s
  Serial.println("Enviar: giro en sentido horario (max velocidad)"); 
  servo.write(180);
  delay(5000);
  
  // Otros valores entre (0-84) y (93-180) hacen que gire más despacio
}
