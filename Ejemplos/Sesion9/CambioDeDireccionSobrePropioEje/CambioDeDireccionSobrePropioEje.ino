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
  // Las dos ruedas giran a la misma velocidad en sentidos contrarios.
  // Aproximadamente debemos girar las ruedas durante 1.000 ms. para hacer un giro de 180 grados.
  // Este tiempo puede variar en función del robot, su calibración, el estado de los motores y el tipo de los motores (marca, modelo, etc.).

  // Gira sobre sí mismo en sentido anti-horario
  servoIzq.write(180);
  servoDer.write(180);
  delay(1000); 

  // Se para
  servoIzq.write(90); 
  servoDer.write(90); 
  delay(3000); 

  // Gira sobre sí mismo en sentido horario
  servoIzq.write(0); 
  servoDer.write(0); 
  delay(1000); 

  // Se para
  servoIzq.write(90); 
  servoDer.write(90); 
  delay(3000);
}
