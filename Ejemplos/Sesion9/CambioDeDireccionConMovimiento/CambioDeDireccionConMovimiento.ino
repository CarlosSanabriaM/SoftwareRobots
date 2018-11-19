#include <Servo.h>

Servo servoIzq;
Servo servoDer;
int pinServoIzq = 8;
int pinServoDer = 9;



void setup() {
  servoIzq.attach(pinServoIzq);
  servoDer.attach(pinServoDer);
}

void loop() {
  // Una rueda permanece parada, mientras la otra gira.

  // En este caso, gira hacia la derecha (avanzando hacia delante)
  servoIzq.write(90); // rueda izda parada
  servoDer.write(180); // rueda derecha a maxima velocidad
  delay(3000); 

  // En este caso, gira hacia la derecha (avanzando hacia atrás)
  servoIzq.write(90); 
  servoDer.write(0);
  delay(3000);

  // En este caso, gira hacia la izquierda (avanzando hacia delante)
  servoIzq.write(0); // rueda izda parada
  servoDer.write(90); // rueda derecha a maxima velocidad
  delay(3000); 

  // En este caso, gira hacia la izquierda (avanzando hacia atrás)
  servoIzq.write(180); 
  servoDer.write(90);
  delay(3000);
}
