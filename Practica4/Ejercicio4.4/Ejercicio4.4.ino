#include <Servo.h>

// Variables para el servomotor
Servo servo;
int servoPin = 7;
const int servoFastLeftValue = 0; // velocidad máxima de giro del servomotor en sentido antihorario
const int servoFastRightValue = 180; // velocidad máxima de giro del servomotor en sentido horario
const int servoStopValue = 90; // valor en el que el servomotor no gira

// Variables para los sensores de colisión
const int leftCollisionSensorPin = 6; // pin digital para el sensor de colisión de la izda
const int rightCollisionSensorPin = 5; // pin digital para el sensor de colisión de la dcha
const int collisionSensorActiveValue = 0; // Valor 1 -> No está activo. Valor 0 -> Está activo.

// Variables para la lógica
unsigned long timeLinealActuatorHitsLeftCollisionSensor; // instante en el que, en modo calibración, el actuador colisiona con el sensor de la izda.
unsigned long timeBetweenCoordinates; // tiempo (en ms) que tarda en moverse de una coordenada a la otra.
const int MAX_COORDINATE = 24; // coordenada máxima
int currentCoordinate; // coordenada actual en la que se encuentra el actuador lineal
const int TIME_DELAY_BETWEEN_MOVEMENTS = 1000; // tiempo de espera en cada coordenada

// Variables para las coordenadas harcodeada
int harcodedCoordinates[13] = {10, 5, 6, 8, 4, 0, 5, -1, 24, 20, 25, 15, 200}; // coordenadas a las que se va a mover
int harcodedCoordinateToChoose = 0; // indica la posicion del array donde está la coordenada a la que se tiene que mover ahora



void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");

  servo.attach(servoPin);

  moveInCalibrationMode();
}

void loop() {
  checkIfUserHasEnteredACoordinate();
}


/*
  Al estar en modo calibración, inicialmente mueve el actuador lineal a la izda del todo,
  y una vez ahí, empieza a contar el tiempo que tarda en moverlo hacia la dcha del todo.
*/
void moveInCalibrationMode() {
  moveInCalibrationModeToTheLeft();
  moveInCalibrationModeToTheRight();
}

/*
  Mueve el actuador lineal a la izda del todo (hasta chocar con el sensor de colisión).
  En ese momento, empieza a medir el tiempo.
*/
void moveInCalibrationModeToTheLeft() {
  moveLinealActuatorToTheLeft();
  while (!hitsLeftCollisionSensor());
  timeLinealActuatorHitsLeftCollisionSensor = millis(); // aquí ya colisionó con el sensor de la izda -> empieza a medir tiempo

  Serial.println("\n# El actuador colisiona con el sensor de colisión de la izquierda."
                 "\n# Se cambia la dirección de movimiento del actuador a la derecha.\n");
}

/*
  Mueve el actuador lineal a la dcha del todo (hasta chocar con el sensor de colisión).
  En ese momento, lo para y mide el tiempo que tardó en moverse desde la izda hasta la dcha,
  y calcula el tiempo que debe de haber entre cada coordenada.
*/
void moveInCalibrationModeToTheRight() {
  moveLinealActuatorToTheRight();
  while (!hitsRightCollisionSensor());
  stopLinealActuator();

  // Se calcula el tiempo del recorrido completo
  unsigned long timeFullTravel = millis() - timeLinealActuatorHitsLeftCollisionSensor;
  Serial.println("\n# El actuador colisiona con el sensor de colisión de la izquierda."
                 "\n# Se para el actuador y se calcula el tiempo del recorrido y el tiempo entre coordenadas.\n");

  // Se calcula el tiempo entre coordenadas
  timeBetweenCoordinates = timeFullTravel / MAX_COORDINATE;
  Serial.println("\n@ Tiempo en ms del recorrido completo: " + String(timeFullTravel));
  Serial.println("\n@ Tiempo en ms entre coordenadas: " + String(timeBetweenCoordinates));

  // La coordenada actual en la que queda el actuador es la coordenda máxima
  currentCoordinate = MAX_COORDINATE;
}

/* Comprueba si el actuador lineal colisiona con el sensor de colisión de la izda. */
boolean hitsLeftCollisionSensor() {
  return digitalRead(leftCollisionSensorPin) == collisionSensorActiveValue;
}

/* Comprueba si el actuador lineal colisiona con el sensor de colisión de la dcha. */
boolean hitsRightCollisionSensor() {
  return digitalRead(rightCollisionSensorPin) == collisionSensorActiveValue;
}

/* Comprueba si quedan coordenadas de las harcodeadas. */
void checkIfUserHasEnteredACoordinate() {
  // Si se terminaron las coordenadas harcodeadas, no hace nada
  if (harcodedCoordinateToChoose + 1 > sizeof(harcodedCoordinates) / sizeof(int)) {
    return;
  }

  // Si no, obtenemos el valor del la coordenada y movemos al actuador a la misma
  int coordinate = harcodedCoordinates[harcodedCoordinateToChoose++];
  userHasEnteredACoordinate(coordinate);
}

/* Se mueve al actuador a la coordenada indicada. */
void userHasEnteredACoordinate(int coordinate) {
  Serial.println("\n- Coordenada introducida: " + String(coordinate));

  if (coordinate <= 0) { // coordenadas menores de 0 se convierten a coordenada 0
    coordinate = 0;
    moveLinealActuatorToCoordinate0();
  }
  else if (coordinate >= MAX_COORDINATE) { // coordenadas mayores de MAX_COORDINATE se convierten a coordenada MAX_COORDINATE
    coordinate = MAX_COORDINATE;
    moveLinealActuatorToMaxCoordinate();
  }
  else {
    moveLinealActuatorToIntermediateCoordinate(coordinate);
  }

  updateCurrentCoordinateAndDelay(coordinate);
}

/* Mueve el actuador lineal hacia la izda, hasta que toca el sensor de colisión de la izda. */
void moveLinealActuatorToCoordinate0() {
  moveLinealActuatorToTheLeft();
  while (!hitsLeftCollisionSensor());
  stopLinealActuator();
}

/* Mueve el actuador lineal hacia la dcha, hasta que toca el sensor de colisión de la dcha. */
void moveLinealActuatorToMaxCoordinate() {
  moveLinealActuatorToTheRight();
  while (!hitsRightCollisionSensor());
  stopLinealActuator();
}

/* Mueve el actuador lineal a la la coordenada intermedia indicada. */
void moveLinealActuatorToIntermediateCoordinate(int coordinate) {
  int coordinatesToMove = coordinate - currentCoordinate;
  unsigned long timeToMove = abs(coordinatesToMove) * timeBetweenCoordinates;

  // Le indicamos al actuador que se mueva hacia la izda o hacia la dcha, en función de la coordenada
  if (coordinatesToMove < 0)
    moveLinealActuatorToTheLeft();
  else if (coordinatesToMove > 0)
    moveLinealActuatorToTheRight();

  // Mientras no transcurra el tiempo que debe moverse el actuador hasta llegar a la coordenada, no lo paramos
  unsigned long timeLinealActuatorStartedToMove = millis();
  while (millis() - timeLinealActuatorStartedToMove < timeToMove);
  stopLinealActuator();
}

/* Actualiza la coordenada actual donde está el actuador, y espera cierto tiempo entre cada movimiento. */
void updateCurrentCoordinateAndDelay(int coordinate) {
  // Actualizamos la coordenada actual en la que está el actuador
  currentCoordinate = coordinate;
  // Esperamos el tiempo indicado entre cada movimiento
  delay(TIME_DELAY_BETWEEN_MOVEMENTS);
}

/* Mueve el actuador lineal a la izquierda. */
void moveLinealActuatorToTheLeft() {
  Serial.println("Se mueve el actuador lineal hacia la izquierda.");
  servo.write(servoFastLeftValue);
}

/* Mueve el actuador lineal a la derecha. */
void moveLinealActuatorToTheRight() {
  Serial.println("Se mueve el actuador lineal hacia la derecha.");
  servo.write(servoFastRightValue);
}

/* Detiene el actuador lineal. */
void stopLinealActuator() {
  Serial.println("Se para actuador lineal");
  servo.write(servoStopValue);
}
