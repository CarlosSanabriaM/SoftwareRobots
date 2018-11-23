#include <Servo.h>

Servo leftServo;
Servo rightServo;

const int rightIRSensorPin = 3;
const int leftIRSensorPin = 2;
const int rightServoPin = 9;
const int leftServoPin = 8;

const int NO_LINE = LOW; // valor que devuelve el sensor IR cuando no detecta linea
const int LINE = HIGH; // valor que devuelve el sensor IR cuando detecta linea

const int leftServoForward = 0;
const int rightServoForward = 180;
const int leftServoBackward = 180;
const int rightServoBackward = 0;
const int stopServo = 90;



void setup() {
  //Serial.begin(9600); // Descomentar para debuguear por consola
  pinMode(rightIRSensorPin, INPUT);
  pinMode(leftIRSensorPin, INPUT);

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
}

void loop() {
  if (leftIRSensor() == LINE && rightIRSensor() == LINE) {
    goForward();
  }
  else if (leftIRSensor() == LINE && rightIRSensor() == NO_LINE) {
    turnLeft();
  }
  else if (leftIRSensor() == NO_LINE && rightIRSensor() == LINE) {
    turnRight();
  }
  else { // Si no detecta linea por ninguno de los dos sensores, se detiene
    stopRobot();
  }
}


/* Le indica al robot que avanze hacia delante a máxima velocidad. */
void goForward() {
  leftServo.write(leftServoForward);
  rightServo.write(rightServoForward);
}

/* Le indica al robot que gire hacia la izquierda. */
void turnLeft() {
  leftServo.write(stopServo);
  rightServo.write(rightServoForward);
}

/* Le indica al robot que gire hacia la derecha. */
void turnRight() {
  leftServo.write(leftServoForward);
  rightServo.write(stopServo);
}

/* Le indica al robot que avanze hacia atrás a máxima velocidad. */
void goBackward() {
  leftServo.write(leftServoBackward);
  rightServo.write(rightServoBackward);
}

/* Le indica al robot que se detenga. */
void stopRobot() {
  leftServo.write(stopServo);
  rightServo.write(stopServo);
}

/* Devuelve la medición del sensor de infrarrojos de detección de linea izquierdo. */
int leftIRSensor() {
  return digitalRead(leftIRSensorPin);
}

/* Devuelve la medición del sensor de infrarrojos de detección de linea derecho. */
int rightIRSensor() {
  return digitalRead(rightIRSensorPin);
}
