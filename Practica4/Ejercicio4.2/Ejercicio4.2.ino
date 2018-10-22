#include <Servo.h>

// Variables para el servomotor
Servo servo;
int pinServo = 7;
const int servoFastLeftValue = 0;
const int servoSlowLeftValue = 80;
const int servoFastRightValue = 180;
const int servoSlowRightValue = 100;
const int servoStopValue = 90;

// Variables para el joystick
const int boton_pin = 8; // Pin digital para el botón
const int X_pin = A0; // Pin analógico para leer eje X
const int Y_pin = A1; // Pin analógico para leer eje Y
// La posicion central del joystick es (530, 494)
// Entre 500 y 560 se considera que el joystick está en el centro del eje x
const int X_low_center_value = 500;
const int X_high_center_value = 560;
// Entre 465 y 525 se considera que el joystick está en el centro del eje y
const int Y_low_center_value = 465;
const int Y_high_center_value = 525;
// Valores en los que cambia la velocidad en el eje X
const int X_left_speed_change_point_value = 15;
const int X_right_speed_change_point_value = 1000;

void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");
  servo.attach(pinServo);
  pinMode(boton_pin, INPUT_PULLUP); // usamos la resistencia interna del Arduino
}

void loop() {
  // Obtenemos el valor del eje X del joystick
  int XValue = getJoystickXValue();

  // Si el valor de X es menor que el valor límite mínimo de la posicion central es que está hacia la izda
  if (XValue < X_low_center_value) {
    moveLinealActuatorToTheLeft(XValue);
  }
  // Si el valor de X es mayor que el valor límite máximo de la posicion central es que está hacia la dcha
  else if (XValue > X_high_center_value) {
    moveLinealActuatorToTheRight(XValue);
  }
  // Si el valor de X está entre los valores límite mínimo y límite máximo de la posicion central es que está en el centro
  else {
    stopLinealActuator();
  }
}

int getJoystickXValue() {
  int XValue = analogRead(X_pin);
  Serial.println("X: " + String(XValue));
  return XValue;
}

void moveLinealActuatorToTheLeft(int XValue) {
  if (XValue < X_left_speed_change_point_value) {
    Serial.println("Se mueve el actuador lineal hacia la izquierda: RAPIDO");
    servo.write(servoFastLeftValue);
  }
  else {
    Serial.println("Se mueve el actuador lineal hacia la izquierda: LENTO");
    servo.write(servoSlowLeftValue);
  }
}

void moveLinealActuatorToTheRight(int XValue) {
  if (XValue > X_right_speed_change_point_value) {
    Serial.println("Se mueve el actuador lineal hacia la derecha: RAPIDO");
    servo.write(servoFastRightValue);
  }
  else {
    Serial.println("Se mueve el actuador lineal hacia la derecha: LENTO");
    servo.write(servoSlowRightValue);
  }
}

void stopLinealActuator() {
  Serial.println("Se para actuador lineal");
  servo.write(servoStopValue);
}
