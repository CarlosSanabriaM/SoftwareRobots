#include <Servo.h>

Servo servoIzq;
Servo servoDer;

const int pinIrDer = 3;
const int pinIrIzq = 2;
const int pinServoIzq = 8;
const int pinServoDer = 9;

// Los sensores que utilizamos para detectar la línea son digitales.
// Retornan un 1 si consiguen leer la línea.
// De lo contrario, retornarán un 0 al no leer la línea.
// ¡Cuidado! Según el modelo, el sensor podría funcionar a la inversa.
const int NO_LINEA = LOW; // También podría ponerse 0
const int LINEA = HIGH; // También podría ponerse 1



void setup() {
  pinMode(pinIrDer, INPUT);
  pinMode(pinIrIzq, INPUT);

  servoIzq.attach(pinServoIzq);
  servoDer.attach(pinServoDer);
}

void loop() {
  // Si no detecta una linea en el suelo, avanza
  if (digitalRead(pinIrIzq) == NO_LINEA || digitalRead(pinIrDer) == NO_LINEA) {
    servoIzq.write(0); 
    servoDer.write(180);
  } 
  // Si detecta una linea en el suelo, da media vuelta
  else {
    // Da media vuelta
    servoIzq.write(180); 
    servoDer.write(180); 
    delay(1500);
  }
}
