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
unsigned long timeLinealActuatorHitsLeftCollisionSensor; // instante en el que, en modo calibración, el actuador colisiona con el sensor de la izda.

// Variables para la lógica
// Si el modo es calibración y esta variable está a false, se tiene que mover a la izda, y si no, a la dcha
unsigned long timeBetweenCoordinates; // tiempo (en ms) que tarda en moverse de una coordenada a la otra.
const int NUM_POSITIONS = 24; // número de coordenadas del recorrido
int currentCoordinate; // almacena la coordenada actual en la que se encuentra el actuador lineal
const int TIME_DELAY_BETWEEN_MOVEMENTS = 1000;

// Variables para las coordenadas harcodeada
int harcodedCoordinates[13] = {10, 5, 6, 8, 4, 0, 5, -1, 24, 20, 25, 15, 200};
int harcodedCoordinateToChoose = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");
  servo.attach(servoPin);

  moveInCalibrationMode();
}

void loop() {
  checkIfUserEntersACoordinate();
}


void moveInCalibrationMode() {
  moveInCalibrationModeToTheLeft();
  moveInCalibrationModeToTheRight();
}

void moveInCalibrationModeToTheLeft() {
  // Mientras no colisione con el sensor de la izda
  moveLinealActuatorToTheLeft();
  while (!hitsLeftCollisionSensor()) {}
  // Aqui ya colisionó con el sensor de la izda
  // Se empieza a contar el tiempo
  timeLinealActuatorHitsLeftCollisionSensor = millis();

  Serial.println("\n# El actuador colisiona con el sensor de colisión de la izquierda."
                 "\n# Se cambia la dirección de movimiento del actuador a la derecha.\n");
}

void moveInCalibrationModeToTheRight() {
  // Mientras no colisione con el sensor de la dcha
  moveLinealActuatorToTheRight();
  while (!hitsRightCollisionSensor()) { }

  // Se calcula el tiempo que se tarda en hacer el recorrido completo, de izda a dcha.
  unsigned long timeFullTravel = millis() - timeLinealActuatorHitsLeftCollisionSensor;
  timeBetweenCoordinates = timeFullTravel / NUM_POSITIONS;
  Serial.println("\n@ Tiempo en ms del recorrido completo: " + String(timeFullTravel));

  // La coordenada actual en la que queda es la máxima
  currentCoordinate = NUM_POSITIONS;

  Serial.println("\n# El actuador colisiona con el sensor de colisión de la derecha."
                 "\n# Se cambia la dirección de movimiento del actuador a la izquierda.\n");
}

boolean hitsLeftCollisionSensor() {
  return digitalRead(leftCollisionSensorPin) == collisionSensorActiveValue;
}

boolean hitsRightCollisionSensor() {
  return digitalRead(rightCollisionSensorPin) == collisionSensorActiveValue;
}

void checkIfUserEntersACoordinate() {
  // Si se terminaron las coordenadas, no hace nada
  if (harcodedCoordinateToChoose + 1 > sizeof(harcodedCoordinates) / sizeof(int)) {
    return;
  }

  // Obtenemos el valor del la coordenada
  int coordinate = harcodedCoordinates[harcodedCoordinateToChoose++];
  Serial.println("\n- Coordenada introducida: " + String(coordinate));

  // Si el valor de X es menor que el valor límite mínimo de la posicion central es que está hacia la izda
  if (coordinate <= 0) {
    // Lo movemos a la coordenada 0 (hasta que llegue al sensor colision izda)
    moveLinealActuatorToTheLeft();
    while (!hitsLeftCollisionSensor()) {}
    stopLinealActuator();
    updateCurrentCoordinateAndDelay(0);
  }
  else if (coordinate >= NUM_POSITIONS) {
    // Lo movemos a la última coordenada (hasta que llegue al sensor colision dcha)
    moveLinealActuatorToTheRight();
    while (!hitsRightCollisionSensor()) {}
    stopLinealActuator();
    updateCurrentCoordinateAndDelay(NUM_POSITIONS);
  }
  else {
    // Movemos el actuador a la coordenada indicada
    moveLinealActuatorToCoordinate(coordinate);
    updateCurrentCoordinateAndDelay(coordinate);
  }
}

void moveLinealActuatorToCoordinate(int coordinate) {
  // En función de la coordenada donde está, se tendrá que mover a la izda o a la dcha un determinado tiempo
  int coordinatesToMove = coordinate - currentCoordinate;
  unsigned long timeToMove = abs(coordinatesToMove) * timeBetweenCoordinates;

  // Si las coordenadas a moverse son negativas, se mueve a la izda
  if (coordinatesToMove < 0) {
    moveLinealActuatorToTheLeft();
    unsigned long timeLinealActuatorStartedToMove = millis();
    while (millis() - timeLinealActuatorStartedToMove < timeToMove) { }
    stopLinealActuator();
  }
  else if (coordinatesToMove > 0) {
    moveLinealActuatorToTheRight();
    unsigned long timeLinealActuatorStartedToMove = millis();
    while (millis() - timeLinealActuatorStartedToMove < timeToMove) { }
    stopLinealActuator();
  }
}

void updateCurrentCoordinateAndDelay(int coordinate) {
  // Actualizamos la coordenada actual en la que está el actuador
  currentCoordinate = coordinate;
  // Esperamos medio segundo entre cada movimiento
  delay(TIME_DELAY_BETWEEN_MOVEMENTS);
}

void moveLinealActuatorToTheLeft() {
  Serial.println("Se mueve el actuador lineal hacia la izquierda.");
  servo.write(servoFastLeftValue);
}

void moveLinealActuatorToTheRight() {
  Serial.println("Se mueve el actuador lineal hacia la derecha.");
  servo.write(servoFastRightValue);
}

void stopLinealActuator() {
  Serial.println("Se para actuador lineal");
  servo.write(servoStopValue);
}
