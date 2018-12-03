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

// Variables para el sensor de ultrasonidos (sensor exterior)
const int pinTrig = 4;
const int pinEcho = 5;
const long DISTANCE_SOMETHING_IN_FRONT = 15; // distancia en cm a partir de la cual se considera que hay algo delante del robot
const int DELAY_BETWEEN_ULTRASONIC_SENSOR_MEASUREMENTS = 300; // tiempo mínimo (en milisegundos) que se ha de esperar entre cada medición del sensor de ultrasonidos (recomendable más de 20 microsegundos)
unsigned long timeLastCheckIfThereIsSomethingInFront; // Almacena el instante el que se comprobó por última vez si algo se movía (en milisegundos)

// Variables para el tiempo al esquivar un objeto
const int TIME_STOP_AVOIDING_OBJECT = 1500; // Tiempo durante el cual se detiene cuando encuentra un objeto delante
const int TIME_TURN_LEFT_AVOIDING_OBJECT = 1000; // Tiempo durante el cual se mueve hacia la izquierda cuando encuentra un objeto delante
const int TIME_TURN_RIGHT_AVOIDING_OBJECT = 1000; // Tiempo durante el cual se mueve hacia la derecha cuando encuentra un objeto delante
const int TIME_GO_FORWARD_AVOIDING_OBJECT = 500; // Tiempo durante el cual se mueve hacia delante cuando encuentra un objeto delante



void setup() {
  Serial.begin(9600);
  pinMode(rightIRSensorPin, INPUT);
  pinMode(leftIRSensorPin, INPUT);

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  // Lanzamos una primera medición para ignorar la primera medición del sensor de ultrasonidos, que suele dar 0cm
  getDistanceFromUltrasonicDistanceSensor();
}

void loop() {
  tryToCheckIfThereIsSomethingInFront();

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

/* Si ha pasado el tiempo minimo entre mediciones del sensor de ultrasonidos, comprueba si hay algo delante del robot. */
void tryToCheckIfThereIsSomethingInFront() {
  // Si ha pasado el tiempo mínimo entre mediciones con el sensor de ultrasonidos desde la última vez que se comprobó si había algo delante
  if (millis() - timeLastCheckIfThereIsSomethingInFront >= DELAY_BETWEEN_ULTRASONIC_SENSOR_MEASUREMENTS) {
    checkIfThereIsSomethingInFront();
  }
}

/* Comprueba si hay algo delante del robot. */
void checkIfThereIsSomethingInFront() {
  timeLastCheckIfThereIsSomethingInFront = millis(); // se establece el tiempo actual como el de la última comprobación

  // Si detecta algo a una distancia menor o igual que la indicada
  if (getDistanceFromUltrasonicDistanceSensor() <= DISTANCE_SOMETHING_IN_FRONT) {
    avoidObject();
  }
}

/* Esquiva el obstáculo que tiene delante. */
void avoidObject() {
  Serial.println("Esquivar objeto.");

  // Se para el robot durante cierto tiempo
  stopRobot();
  delay(TIME_STOP_AVOIDING_OBJECT);

  // Se mueve el robot hacia la izquierda durante cierto tiempo
  turnLeft();
  delay(TIME_TURN_LEFT_AVOIDING_OBJECT);

  // Avanza durante cierto tiempo
  goForward();
  delay(TIME_GO_FORWARD_AVOIDING_OBJECT);


  // Se mueve el robot hacia la derecha durante cierto tiempo
  turnRight();
  delay(TIME_TURN_RIGHT_AVOIDING_OBJECT);

  // Mientras no encuentre línea, se mueve hacia la derecha ligeramente
  leftServo.write(10);
  rightServo.write(110);
  while (leftIRSensor() == NO_LINE && rightIRSensor() == NO_LINE);
}

/* Devuelve la distancia en cm al sensor de ultrasonidos */
long getDistanceFromUltrasonicDistanceSensor() {
  // Por seguridad volvemos a poner el Trig a LOW
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(5);

  // Emitimos el pulso ultrasónico
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);

  // Medimos la longitud del pulso entrante
  long responseTime = pulseIn(pinEcho, HIGH);

  // Calculamos la distancia conociendo la velocidad
  long distance = int(0.01716 * responseTime);
  Serial.println("Distancia sensor ultrasonidos: " + String(distance) + "cm");

  return distance;
}
