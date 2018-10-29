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
String coordinatesString; // almacena el String con las coordenadas y los delays introducido por el usuario

struct coordinateAndDelay
{
  int coordinate;
  int timeDelay;
};

typedef struct coordinateAndDelay CoordinateAndDelay;

CoordinateAndDelay arrayOfCoordinates[10];

void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");
  Serial.setTimeout(5);

  servo.attach(servoPin);

  moveInCalibrationMode();
}

void loop() {
  checkIfUserEntersCoordinates();
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

void checkIfUserEntersCoordinates() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    coordinatesString = Serial.readString();
    Serial.print("El usuario ha introucido: " + coordinatesString);

    // Mientras haya coordenadas con delay en el String
    while (hasCoordenatesAndDelay()) {
      // Se obtiene el String de el primer grupo (coordenada,delay)
      String coordinateAndDelayString = getCoordinateAndDelayString();
      // Se convierte en un objeto CoordinateAndDelay
      CoordinateAndDelay coordinateAndDelay = getCoordinateAndDelayStructFromCoordinateAndDelayString(coordinateAndDelayString);
      Serial.println("coordinate: " + coordinateAndDelay.coordinate);
      Serial.println("timeDelay: " + coordinateAndDelay.timeDelay);
    }

  }
}

boolean hasCoordenatesAndDelay() {
  return coordinatesString.length() > 0;
}

String getCoordinateAndDelayString() {
  return getStringUntilSeparator(';');
}

String getStringUntilSeparator(char separator) {
  int i = 0;
  while (coordinatesString.charAt(i) != separator) {
    i++;
  }
  // Almacenamos el String hasta el ; (sin incluir)
  String result = coordinatesString.substring(0, i);
  // Eliminamos los caracteres del String hasta el ; (incluido)
  coordinatesString.remove(0, i + 1);

  return result;
}

CoordinateAndDelay getCoordinateAndDelayStructFromCoordinateAndDelayString(String coordinateAndDelayString) {
  int i = 0;
  while (coordinateAndDelayString.charAt(i) != ',') {
    i++;
  }
  // Almacenamos el String hasta el , (sin incluir)
  String coordinateString = coordinateAndDelayString.substring(0, i);
  // Almacenamos el String desde el sgte char al , hasta el final
  String delayString = coordinateAndDelayString.substring(i + 1);

  Serial.println("coordinateString: " + coordinateString);
  Serial.println("delayString: " + delayString);

  // Creamos un struct con esos valores
  CoordinateAndDelay coordinateAndDelay;
  coordinateAndDelay.coordinate = coordinateString.toInt();
  coordinateAndDelay.timeDelay = delayString.toInt();

  return coordinateAndDelay;
}

void userHasEnteredACoordinate(int coordinate, int timeDelay) {
  Serial.println("\n- Coordenada introducida: " + String(coordinate));
  Serial.println("\n- Tiempo introducido: " + String(timeDelay));

  // Si el valor de X es menor que el valor límite mínimo de la posicion central es que está hacia la izda
  if (coordinate <= 0) {
    // Lo movemos a la coordenada 0 (hasta que llegue al sensor colision izda)
    moveLinealActuatorToTheLeft();
    while (!hitsLeftCollisionSensor()) {}
    stopLinealActuator();
    updateCurrentCoordinateAndDelay(0, timeDelay);
  }
  else if (coordinate >= NUM_POSITIONS) {
    // Lo movemos a la última coordenada (hasta que llegue al sensor colision dcha)
    moveLinealActuatorToTheRight();
    while (!hitsRightCollisionSensor()) {}
    stopLinealActuator();
    updateCurrentCoordinateAndDelay(NUM_POSITIONS, timeDelay);
  }
  else {
    // Movemos el actuador a la coordenada indicada
    moveLinealActuatorToCoordinate(coordinate);
    updateCurrentCoordinateAndDelay(coordinate, timeDelay);
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

void updateCurrentCoordinateAndDelay(int coordinate, int timeDelay) {
  // Actualizamos la coordenada actual en la que está el actuador
  currentCoordinate = coordinate;
  // Esperamos medio segundo entre cada movimiento
  delay(timeDelay);
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
