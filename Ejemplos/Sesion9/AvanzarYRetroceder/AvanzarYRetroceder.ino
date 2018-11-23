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
  // Atras
  servoIzq.write(180); // Velocidad Máxima
  servoDer.write(0);
  delay(1000);

  //servoIzq.write(100); // Próximo a 90, despacio
  //servoDer.write(80);
  //delay(3000);

  // Detener 
  servoIzq.write(90);
  servoDer.write(90); 
  delay(500);
  
  // Adelante
  //servoIzq.write(80); // Próximo a 90, despacio
  //servoDer.write(100);
  //delay(3000);
  
  servoIzq.write(0); // Velocidad Máxima
  servoDer.write(180);
  delay(1000);
}
