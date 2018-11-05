#include <Servo.h>

// Variables para el servomotor del eje X
Servo xAxisServo;
int xAxisServoPin = 6;
// Variables para el servomotor del eje Y
Servo yAxisServo;
int yAxisServoPin = 7;

// Variables para los servomotores de los ejes X e Y
const int servoFastLeftOrUpValue = 0; // velocidad máxima de giro del servomotor en sentido antihorario
const int servoSlowLeftOrUpValue = 80; // velocidad lenta de giro del servomotor en sentido antihorario
const int servoFastRightOrDownValue = 180; // velocidad máxima de giro del servomotor en sentido horario
const int servoSlowRightOrDownValue = 100; // velocidad lenta de giro del servomotor en sentido horario
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
// Valores en los que cambia la velocidad en el eje X
const int Y_up_speed_change_point_value = 15; // entre 0 y 15 se considera que se quiere mover el actuador a máxima velocidad a la izda
const int Y_down_speed_change_point_value = 1000; // entre 1000 y 1023 se considera que se quiere mover el actuador a máxima velocidad a la dcha



void setup() {
  Serial.begin(9600);
  Serial.println(F("\n---------\n  Setup\n---------"));
  xAxisServo.attach(xAxisServoPin);
  yAxisServo.attach(yAxisServoPin);
}

void loop() {
  checkIfHaveToMoveXAxisActuator();
  checkIfHaveToMoveYAxisActuator();
}


/* Comprueba si es necesario mover el actuador lineal del eje X. */
void checkIfHaveToMoveXAxisActuator() {
  // Obtenemos el valor del eje X del joystick 1
  int XValue = getJoystickXValue(1);

  // Si el valor de X es menor que el valor límite mínimo de la posicion central es que el joystick está hacia la izda
  if (XValue < X_low_center_value) {
    moveLinealActuatorToTheLeft(XValue);
  }
  // Si el valor de X es mayor que el valor límite máximo de la posicion central es que el joystick está hacia la dcha
  else if (XValue > X_high_center_value) {
    moveLinealActuatorToTheRight(XValue);
  }
  // Si el valor de X está entre los valores límite mínimo y límite máximo de la posicion central es que el joystick está en el centro
  else {
    stopLinealActuator('X'); // se le indica que es el actuador del eje X
  }
}

/* Comprueba si es necesario mover el actuador lineal del eje Y. */
void checkIfHaveToMoveYAxisActuator() {
  // Obtenemos el valor del eje X del joystick 1
  int YValue = getJoystick1YValue();

  // Si el valor de Y es menor que el valor límite mínimo de la posicion central es que el joystick está hacia arriba
  if (YValue < Y_low_center_value) {
    moveLinealActuatorUp(YValue);
  }
  // Si el valor de Y es mayor que el valor límite máximo de la posicion central es que el joystick está hacia abajo
  else if (YValue > Y_high_center_value) {
    moveLinealActuatorDown(YValue);
  }
  // Si el valor de Y está entre los valores límite mínimo y límite máximo de la posicion central es que el joystick está en el centro
  else {
    stopLinealActuator('Y'); // se le indica que es el actuador del eje Y
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

/* Devuelve el valor en el ejeY para el jostick indicado. */
int getJoystick1YValue() {
  int YValue = analogRead(joystick1_Y_pin);
  Serial.println("Y: " + String(YValue));
  return YValue;
}

/* Mueve el actuador lineal del eje X hacia la izda. */
void moveLinealActuatorToTheLeft(int XValue) {
  if (XValue <= X_left_speed_change_point_value) {
    Serial.println(F("Se mueve el actuador lineal hacia la izquierda: RAPIDO"));
    xAxisServo.write(servoFastLeftOrUpValue);
  }
  else {
    Serial.println(F("Se mueve el actuador lineal hacia la izquierda: LENTO"));
    xAxisServo.write(servoSlowLeftOrUpValue);
  }
}

/* Mueve el actuador lineal del eje X hacia la dcha. */
void moveLinealActuatorToTheRight(int XValue) {
  if (XValue >= X_right_speed_change_point_value) {
    Serial.println(F("Se mueve el actuador lineal hacia la derecha: RAPIDO"));
    xAxisServo.write(servoFastRightOrDownValue);
  }
  else {
    Serial.println(F("Se mueve el actuador lineal hacia la derecha: LENTO"));
    xAxisServo.write(servoSlowRightOrDownValue);
  }
}

/* Mueve el actuador lineal del eje Y hacia arriba. */
void moveLinealActuatorUp(int YValue) {
  if (YValue <= Y_up_speed_change_point_value) {
    Serial.println(F("Se mueve el actuador lineal hacia arriba: RAPIDO"));
    yAxisServo.write(servoFastLeftOrUpValue);
  }
  else {
    Serial.println(F("Se mueve el actuador lineal hacia arriba: LENTO"));
    yAxisServo.write(servoSlowLeftOrUpValue);
  }
}

/* Mueve el actuador lineal del eje Y hacia abajo. */
void moveLinealActuatorDown(int YValue) {
  if (YValue >= Y_down_speed_change_point_value) {
    Serial.println(F("Se mueve el actuador lineal hacia abajo: RAPIDO"));
    yAxisServo.write(servoFastRightOrDownValue);
  }
  else {
    Serial.println(F("Se mueve el actuador lineal hacia abajo: LENTO"));
    yAxisServo.write(servoSlowRightOrDownValue);
  }
}

/* Detiene el actuador lineal del eje indicado. */
void stopLinealActuator(char actuatorAxis) {
  Serial.println("Se para actuador lineal del eje " + String(actuatorAxis));
  if(actuatorAxis == 'X') xAxisServo.write(servoStopValue);
  else yAxisServo.write(servoStopValue);
}
