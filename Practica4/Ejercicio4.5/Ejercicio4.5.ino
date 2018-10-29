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
const int MAX_COORDINATE = 24; // número de coordenadas del recorrido
int currentCoordinate; // almacena la coordenada actual en la que se encuentra el actuador lineal
String coordinatesString; // almacena el String con las coordenadas y los delays introducido por el usuario



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
  while (!hitsLeftCollisionSensor());
  // Aqui ya colisionó con el sensor de la izda
  // Se empieza a contar el tiempo
  timeLinealActuatorHitsLeftCollisionSensor = millis();

  Serial.println("\n# El actuador colisiona con el sensor de colisión de la izquierda."
                 "\n# Se cambia la dirección de movimiento del actuador a la derecha.\n");
}

void moveInCalibrationModeToTheRight() {
  // Mientras no colisione con el sensor de la dcha
  moveLinealActuatorToTheRight();
  while (!hitsRightCollisionSensor());

  // Se calcula el tiempo que se tarda en hacer el recorrido completo, de izda a dcha.
  unsigned long timeFullTravel = millis() - timeLinealActuatorHitsLeftCollisionSensor;
  timeBetweenCoordinates = timeFullTravel / MAX_COORDINATE;
  Serial.println("\n@ Tiempo en ms del recorrido completo: " + String(timeFullTravel));

  // La coordenada actual en la que queda es la máxima
  currentCoordinate = MAX_COORDINATE;

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
  // Si hay información en el buffer de entrada
  if (Serial.available() > 0) {
    // Se lee esa información y se almacena en el string
    coordinatesString = Serial.readString();
    Serial.print("El usuario ha introducido: " + coordinatesString);

    // Mientras haya coordenadas con delay en el String
    while (hasCoordenatesAndDelay()) {
      // Se obtiene el String del primer grupo (coordenada,delay)
      String coordinateAndDelayString = getCoordinateAndDelayString();

      // Se convierte en un array de dos enteros (1ªpos -> coordenada, 2ªpos -> tiempo delay)
      int coordinateAndDelay[2];
      getCoordinateAndDelayFromString(coordinateAndDelayString, coordinateAndDelay);

      // Se pasa la coordenada y el delay al método que efectua el movimiento
      userHasEnteredACoordinate(coordinateAndDelay[0], coordinateAndDelay[1]);
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

void getCoordinateAndDelayFromString(String coordinateAndDelayString, int coordinateAndDelay[]) {
  int i = 0;
  while (coordinateAndDelayString.charAt(i) != ',') {
    i++;
  }
  // Almacenamos el String hasta el , (sin incluir)
  String coordinateString = coordinateAndDelayString.substring(0, i);
  // Almacenamos el String desde el siguiente char al , hasta el final
  String delayString = coordinateAndDelayString.substring(i + 1);

  //Serial.println("coordinateString: " + coordinateString);
  //Serial.println("delayString: " + delayString);

  // Modificamos el array, que es pasado por referencia
  coordinateAndDelay[0] = coordinateString.toInt();
  coordinateAndDelay[1] = delayString.toInt();
}

/* Mueve el actuador lineal a la coordeanda indicada, y espera el tiempo indicado antes del sgte movimiento. */
void userHasEnteredACoordinate(int coordinate, int timeDelay) {
  Serial.println("\n- Coordenada introducida: " + String(coordinate));
  Serial.println("\n- Tiempo introducido: " + String(timeDelay));

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

  updateCurrentCoordinateAndDelay(coordinate, timeDelay);
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
void updateCurrentCoordinateAndDelay(int coordinate, int timeDelay) {
  // Actualizamos la coordenada actual en la que está el actuador
  currentCoordinate = coordinate;
  // Esperamos el tiempo indicado entre cada movimiento
  delay(timeDelay);
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
