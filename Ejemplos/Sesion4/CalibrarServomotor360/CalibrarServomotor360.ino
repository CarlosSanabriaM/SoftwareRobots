#include <Servo.h>

Servo servo;
int pinServo = 7;

void setup() {
  Serial.begin(9600);
  servo.attach(pinServo);
  Serial.println("90º.\nNo debería girar!\nSi gira o hace ruido, desconectar y apretar el tornillo."); 
}

void loop() {
  /*
   * La primera vez que se utiliza requiere calibración. 
   * Para calibrar un servomotor se le envía la señal de detener motor servo.write(90). 
   * Si al ejecutar esta instrucción el servomotor continúa moviéndose debemos girar el tornillo 
   * situado en la parte delantera del servomotor hasta que esté completamente detenido y no haga ningún ruido (zumbido). 
   * En ocasiones, la calibración también se puede perder por el uso.
   */
  
  servo.write(0); 
}
