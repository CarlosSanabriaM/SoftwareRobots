#include <Servo.h>

Servo leftServo;
Servo rightServo;

// Pines de los sensores de luz infrarroja
const int IRSensorsPins[4] = {10, 2, 3, 11}; // izquierdo, central-izquierdo, central-derecho, derecho
// Ultimos valores leidos para los sensores de luz infrarroja
int IRSensorsValues[4];
// Pines de los servomotores de las ruedas
const int rightServoPin = 9;
const int leftServoPin = 8;

// Valores para linea y no linea
const int NO_LINE = LOW; // valor que devuelve el sensor IR cuando no detecta linea
const int LINE = HIGH; // valor que devuelve el sensor IR cuando detecta linea

// Velocidades de giro de los servomotores
const int leftServoForward = 0;
const int rightServoForward = 180;
const int leftServoBackward = 180;
const int rightServoBackward = 0;
const int stopServo = 90;

const int INITIAL_DELAY_TIME = 5000; // Inicialmente, el robot espera 5 segundos antes de ejecutar el loop



void setup() {
  //Serial.begin(9600); // Descomentar para debuguear por consola

  // Para cada uno de los 4 pines de los sensores IR, los inicializamos como pines de entrada
  for(int i=0; i < 4; i++) {
    pinMode(IRSensorsPins[i], INPUT);
  }

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  delay(INITIAL_DELAY_TIME);
}

void loop() {
  updateAllIRSensors();

  // _ X X _
  if (assertIRSensorsValues(NO_LINE, LINE, LINE, NO_LINE)) {
    goForward();
  }
  // _ _ X _
  else if (assertIRSensorsValues(NO_LINE, NO_LINE, LINE, NO_LINE)) {
    correctionToTheRight();
  }
  // _ X _ _
  else if (assertIRSensorsValues(NO_LINE, LINE, NO_LINE, NO_LINE)) {
    correctionToTheLeft();
  }
  // _ X X X
  else if (assertIRSensorsValues(NO_LINE, LINE, LINE, LINE)) {
    // Puede ser: "curva a la derecha" o "curva a la derecha y de frente"
    checkTypeOfCrossingRight();
  }
  // X X X _
  else if (assertIRSensorsValues(LINE, LINE, LINE, NO_LINE)) {
    // Puede ser: "curva a la izquierda" o "curva a la izquierda y de frente"
    checkTypeOfCrossingLeft();
  }
  // X X X X
  else if (assertIRSensorsValues(LINE, LINE, LINE, LINE)) {
    // Puede ser: "cruce a la izquierda y a la derecha",
    // "cruce a la izquierda, a la derecha y de frente" o "meta"
    checkTypeOfCrossingAllLine();
  }
  // _ _ _ _
  else {
    turnAround();
  }
}


/* Retorna si los valores pasados como parámetro coinciden con las últimas mediciones de los sensores IR. */
boolean assertIRSensorsValues(int IRSensor0, int IRSensor1, int IRSensor2, int IRSensor3) {
  return IRSensorsValues[0] == IRSensor0 && IRSensorsValues[1] == IRSensor1 &&
         IRSensorsValues[2] == IRSensor2 && IRSensorsValues[3] == IRSensor3;
}

/* Le indica al robot que se detenga. */
void stopRobot() {
  leftServo.write(stopServo);
  rightServo.write(stopServo);
}

/* Le indica al robot que avanze hacia delante a máxima velocidad. */
void goForward() {
  leftServo.write(leftServoForward);
  rightServo.write(rightServoForward);
}

/* Le indica al robot que corriga su camino hacia la izquierda, para poder seguir recto. */
void correctionToTheLeft() {
  leftServo.write(stopServo);
  rightServo.write(rightServoForward);
}

/* Le indica al robot que corriga su camino hacia la derecha, para poder seguir recto. */
void correctionToTheRight() {
  leftServo.write(leftServoForward);
  rightServo.write(stopServo);
}

/* Le indica al robot que avanze hacia atrás a máxima velocidad. */
void goBackward() {
  leftServo.write(leftServoBackward);
  rightServo.write(rightServoBackward);
}

/* Le indica al robot que se de la vuelta. */
void turnAround() {
  // Gira sobre sí mismo en sentido anti-horario
  leftServo.write(leftServoBackward);
  rightServo.write(rightServoForward);
  delay(1000);
}

/* Comprueba si es un cruce a la derecha o a la derecha y de frente. */
void checkTypeOfCrossingRight() {
  goForwardAndUpdateAllIRSensors();

  // _ X X _
  if(assertIRSensorsValues(NO_LINE, LINE, LINE, NO_LINE)) {
    // Algo de línea delante, es una curva a la derecha y de frente
    // TODO - moverse en funcion del alg usado
  }
  // _ _ _ _
  else { // else if(assertIRSensorsValues(NO_LINE, NO_LINE, NO_LINE, NO_LINE)) {
    // Es una curva solo a la derecha
    // TODO - moverse en funcion del alg usado
  }
}

/* Comprueba si es un cruce a la izquierda o a la izquierda y de frente. */
void checkTypeOfCrossingLeft() {
  goForwardAndUpdateAllIRSensors();

  // _ X X _
  if(assertIRSensorsValues(NO_LINE, LINE, LINE, NO_LINE)) {
    // Algo de línea delante, es una curva a la izquierda y de frente
    // TODO - moverse en funcion del alg usado
  }
  // _ _ _ _
  else { // else if(assertIRSensorsValues(NO_LINE, NO_LINE, NO_LINE, NO_LINE)) {
    // Es una curva solo a la izquierda
    // TODO - moverse en funcion del alg usado
  }
}

/* Comprueba si es un cruce:
  - A la izquierda y a la derecha.
  - A la izquierda, a la derecha y de frente.
  - Meta.
*/
void checkTypeOfCrossingAllLine() {
  goForwardAndUpdateAllIRSensors();

  // _ _ _ _
  if(assertIRSensorsValues(NO_LINE, NO_LINE, NO_LINE, NO_LINE)) {
    // Es un cruce a la izquierda y a la derecha
    // TODO - moverse en funcion del alg usado
  }
  // _ X X _
  else if(assertIRSensorsValues(NO_LINE, LINE, LINE, NO_LINE)) {
    // Es un cruce a la izquierda, a la derecha y de frente
    // TODO - moverse en funcion del alg usado
  }
  // X X X X
  else if(assertIRSensorsValues(LINE, LINE, LINE, LINE)) {
    // Es la meta
    // TODO - moverse en funcion del alg usado
  }
}

/* Avanza ligeramente y actualiza las mediciones de los sensores IR. */
void goForwardAndUpdateAllIRSensors() {
  // Avanzamos un poco para comprobar qué tipo de cruce es
  goForward();
  delay(300);

  // Paramos un corto periodo de tiempo
  stopRobot();
  delay(300);

  updateAllIRSensors();
}

/* Actualiza las mediciones de los sensores de infrarrojos. */
void updateAllIRSensors() {
  // Para cada uno de los 4 sensores IR, actualiza su valor
  for(int i=0; i < 4; i++) {
    IRSensorsValues[i] = digitalRead(IRSensorsPins[i]);
  }
  // Se realizan 4 lecturas adicionales, para estar seguros del valor de cada sensor
  // Se guardan las que detectan línea en cualquiera de los 4 barridos
  for(int i=0; i < 4; i++) { // 4 barridos
    delay(20); // Delay con el movimiento actual

    for(int i=0; i < 4; i++) { // para cada 1 de los 4 sensores IR
      if (digitalRead(IRSensorsPins[i]) == LINE )
        IRSensorsValues[i] = LINE;
    }
  }
}
