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
// Valores maximos del eje X
const int X_max_left_value = 0;
const int X_max_right_value = 1023;
// Valores en los que cambia la velocidad en el eje X
const int X_left_speed_change_point_value = 15;
const int X_right_speed_change_point_value = 1000;

// Variables para los sensores de colisión
const int leftCollisionSensorPin = 6;
const int rightCollisionSensorPin = 5;
const int collisionSensorActiveValue = 0; // Valor 1 -> No está activo. Valor 0 -> Está activo.
const int joystickButtonActiveValue = 0; // Valor 1 -> No está activo. Valor 0 -> Está activo.

boolean isInAutomaticMode = false; // indica si está en modo automático o manual
boolean linealActuatorInAutomaticModeHasToGoToTheRight = false; // si está a false, tiene que ir a la izda, y si no a la dcha


void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");
  servo.attach(pinServo);
  pinMode(boton_pin, INPUT_PULLUP); // usamos la resistencia interna del Arduino
}

void loop() {
  checkIfJoystickButtonIsPressed();

  if (isInAutomaticMode) {
    moveInAutomaticMode();
  }
  else {
    moveInManualMode();
  }
}

void checkIfJoystickButtonIsPressed() {
  if (digitalRead(boton_pin) == joystickButtonActiveValue) {
    Serial.println("Botón pulsado. Se cambia el modo.");
    isInAutomaticMode = !isInAutomaticMode;
    delay(500);
  }
}

void moveInAutomaticMode() {
  // Se mueve hacia la dcha
  if (linealActuatorInAutomaticModeHasToGoToTheRight) {
    checkIfHitsRightCollisionSensor();
    moveLinealActuatorToTheRight(X_max_right_value);
  }
  // Se mueve hacia la izda
  else {
    checkIfHitsLeftCollisionSensor();
    moveLinealActuatorToTheLeft(X_max_left_value);
  }
}

void checkIfHitsLeftCollisionSensor() {
  int leftCollisionSensorValue = digitalRead(leftCollisionSensorPin);

  // Si el actuador choca con el sensor de colisión de la izda, le cambiamos la dirección a la derecha
  if (leftCollisionSensorValue == collisionSensorActiveValue) {
    Serial.println("El actuador colisiona con el sensor de colisión de la izquierda");
    linealActuatorInAutomaticModeHasToGoToTheRight = true;
  }
}

void checkIfHitsRightCollisionSensor() {
  int rightCollisionSensorValue = digitalRead(rightCollisionSensorPin);

  // Si el actuador choca con el sensor de colisión de la dcha, le cambiamos la dirección a la izda
  if (rightCollisionSensorValue == collisionSensorActiveValue) {
    Serial.println("El actuador colisiona con el sensor de colisión de la derecha");   
    linealActuatorInAutomaticModeHasToGoToTheRight = false;
  }
}

void moveInManualMode() {
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
