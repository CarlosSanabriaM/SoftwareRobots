#include <Servo.h>

const int collisionSensorPin = 6;

void setup() {
  Serial.begin(9600); 
  pinMode(collisionSensorPin, INPUT);
}
void loop() {
  Serial.println("Sensor Crash: " + String(digitalRead(collisionSensorPin)));
}
