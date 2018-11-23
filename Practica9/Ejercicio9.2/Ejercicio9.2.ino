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

const int leftServoForwardSlow = 20; // La rueda izquierda avanza hacia delante, pero más despacio que a la velocidad máxima.


boolean haveToResetTurnVariables = true; // Indica que cuando pierda la línea, debe resetear las variables de giro.

int timeIncreaseTurn; // Tiempo entre incrementos del giro cuando está buscando línea.
const int INITIAL_TIME_INCREASE_TURN = 5000; // Tiempo entre incrementos del giro cuando está buscando línea.
const int TIME_INCREASE_TURN_INCREASE = 2000; // Incremento que se aplica al tiempo entre incrementos del giro.

int turn; // Indica como se incrementa el giro cuando está buscando línea.
unsigned long timeLastTurnIncrease; // Almacena el instante en el que incrementó el giro por primera vez.
const int INITIAL_TURN = 10; // Almacena el giro inicial al que se somete el robot cuando empieza a buscar línea.
const int TURN_INCREASE = 5; // Incrementos en el giro del robot cuando está buscando línea.
const int TURN_LIMIT = 40; // Incrementos en el giro del robot cuando está buscando línea.



void setup() {
  //Serial.begin(9600); // Descomentar para debuguear por consola
  pinMode(rightIRSensorPin, INPUT);
  pinMode(leftIRSensorPin, INPUT);

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
}

void loop() {
  if (leftIRSensor() == LINE && rightIRSensor() == LINE) {
    hasFoundLine();
    goForward();
  }
  else if (leftIRSensor() == LINE && rightIRSensor() == NO_LINE) {
    hasFoundLine();
    turnLeft();
  }
  else if (leftIRSensor() == NO_LINE && rightIRSensor() == LINE) {
    hasFoundLine();
    turnRight();
  }
  else { // Si no detecta linea por ninguno de los dos sensores, empieza a buscar la linea.
    lookForLine();
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

/* Se mueve en una espiral intentando encontrar la línea. */
void lookForLine() {
  // El primer instante que comienza a buscar la línea establece el instante en el que comenzó a buscarla como el actual
  if (haveToResetTurnVariables) {
    timeLastTurnIncrease = millis();
    turn = INITIAL_TURN; // El giro empieza en el valor inicial
    timeIncreaseTurn = INITIAL_TIME_INCREASE_TURN; // El tiempo entre incrementos empieza en el valor inicial
    haveToResetTurnVariables = false;
  }

  // Si han pasado más de timeIncreaseTurn segundos desde el ultimo incremento de giro y no se supera el giro límite (sumando el incremento)
  if (millis() - timeLastTurnIncrease > timeIncreaseTurn && turn + TURN_INCREASE <= TURN_LIMIT ) {
    timeLastTurnIncrease = millis();
    turn += TURN_INCREASE; // se incrementa el giro.
    timeIncreaseTurn += TIME_INCREASE_TURN_INCREASE; // se incrementa el tiempo entre incrementos.
  }

  spiralPath();
}

/* Le indica al robot que avanze en espiral, en sentido horario. */
void spiralPath() {
  leftServo.write(leftServoForwardSlow);
  rightServo.write(stopServo + turn);
}

/* Ha encontrado la línea, por lo que no es la primera vez que la busca. */
void hasFoundLine() {
  // Aquí ya encontró la línea, así que ya no es el primer momento en el que busca la línea
  // (esa variable se pondrá a cierto la prox vez que pierda la línea)
  haveToResetTurnVariables = true;
}
