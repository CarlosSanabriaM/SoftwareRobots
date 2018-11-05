#include <Servo.h>

// Variables para el servomotor del eje X
Servo xAxisServo;
int xAxisServoPin = 6;
// Variables para el servomotor del eje Y
Servo yAxisServo;
int yAxisServoPin = 7;

// Variables para los servomotores de los ejes X e Y
const int servoFastLeftOrDownValue = 0; // velocidad máxima de giro del servomotor en sentido antihorario
const int servoSlowLeftOrDownValue = 80; // velocidad lenta de giro del servomotor en sentido antihorario
const int servoFastRightOrUpValue = 180; // velocidad máxima de giro del servomotor en sentido horario
const int servoSlowRightOrUpValue = 100; // velocidad lenta de giro del servomotor en sentido horario
const int servoStopValue = 90; // valor en el que el servomotor no gira

// Variables para el joystick 1 (controla los actuadores de los ejes X e Y)
const int joystick1_X_pin = A0; // Pin analógico para leer eje X
const int joystick1_Y_pin = A1; // Pin analógico para leer eje Y
// Variables para el joystick 2 (controla la pinza)
const int joystick2_X_pin = A2; // Pin analógico para leer eje X


// La posicion central del joystick es (530, 494)

// Entre 500 y 560 se considera que el joystick está en el centro del eje x
const int X_low_center_value = 500;
const int X_high_center_value = 560;
// Valores en los que cambia la velocidad en el eje X
const int X_left_speed_change_point_value = 15; // entre 0 y 15 se considera que se quiere mover el actuador a máxima velocidad a la izda
const int X_right_speed_change_point_value = 1000; // entre 1000 y 1023 se considera que se quiere mover el actuador a máxima velocidad a la dcha

// Entre 465 y 525 se considera que el joystick está en el centro del eje y
const int Y_low_center_value = 465;
const int Y_high_center_value = 525;



void setup() {
  Serial.begin(9600);
  Serial.println(F("\n---------\n  Setup\n---------"));
  xAxisServo.attach(xAxisServoPin);
  yAxisServo.attach(yAxisServoPin);
}

void loop() {
  checkIfHaveToMoveXAxisActuator();
}


/* Comprueba si es necesario mover el actuador lineal del eje X. */
void checkIfHaveToMoveXAxisActuator() {
  // Obtenemos el valor del eje X del joystick 1
  int XValue = getJoystickXValue(1);

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
    stopLinealActuator('X'); // se le indica que es el actuador del eje X
  }
}

/* Devuelve el valor en el ejeX para el jostick indicado. */
int getJoystickXValue(int jostickNum) {
  int XValue;
  if(jostickNum == 1) XValue = analogRead(joystick1_X_pin);
  else XValue = analogRead(joystick2_X_pin);
  
  Serial.println("X: " + String(XValue));
  return XValue;
}

/* Mueve el actuador lineal del eje X hacia la izda. */
void moveLinealActuatorToTheLeft(int XValue) {
  if (XValue <= X_left_speed_change_point_value) {
    Serial.println(F("Se mueve el actuador lineal hacia la izquierda: RAPIDO"));
    xAxisServo.write(servoFastLeftOrDownValue);
  }
  else {
    Serial.println(F("Se mueve el actuador lineal hacia la izquierda: LENTO"));
    xAxisServo.write(servoSlowLeftOrDownValue);
  }
}

/* Mueve el actuador lineal del eje X hacia la dcha. */
void moveLinealActuatorToTheRight(int XValue) {
  if (XValue >= X_right_speed_change_point_value) {
    Serial.println(F("Se mueve el actuador lineal hacia la derecha: RAPIDO"));
    xAxisServo.write(servoFastRightOrUpValue);
  }
  else {
    Serial.println(F("Se mueve el actuador lineal hacia la derecha: LENTO"));
    xAxisServo.write(servoSlowRightOrUpValue);
  }
}

/* Detiene el actuador lineal del eje indicado. */
void stopLinealActuator(char actuatorAxis) {
  Serial.println("Se para actuador lineal del eje " + String(actuatorAxis));
  if(actuatorAxis == 'X') xAxisServo.write(servoStopValue);
  else yAxisServo.write(servoStopValue);
}
