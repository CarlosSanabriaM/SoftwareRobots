#include <Servo.h>

Servo servo;
int pinServo = 8;

void setup() {
  Serial.begin(9600);
  servo.attach(pinServo);
}

void loop() {
  servo.write(180); 
  Serial.println("180"); 
  delay(3000); 
  
  servo.write(90); 
  Serial.println("90"); 
  delay(3000); 
  
  servo.write(0);
  Serial.println("0"); 
  delay(3000);
}
