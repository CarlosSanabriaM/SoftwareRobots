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
String coordinatesString; // String con las coordenadas y los delays introducido por el usuario (c1,d1;c2,d2;c3,d3; ...)



void setup() {
  Serial.begin(9600);
  Serial.setTimeout(5);
  Serial.println("\n---------\n  Setup\n---------");

  servo.attach(servoPin);

  moveInCalibrationMode();
}

void loop() {
  checkIfUserHasEnteredCoordinates();
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

/* Comprueba si el usuario ha introducido coordenadas por consola. */
void checkIfUserHasEnteredCoordinates() {
  // Si hay información en el buffer de entrada
  if (Serial.available() > 0) {
    userHasEnteredCoordinates();
  }
}

/*
  Se obtienen uno a uno los pares (coordenada,tiempo_delay) introducidos por el usuario
  y se le indica al actuador lineal que se mueva a esas coordenadas y espere el tiempo indicado.
*/
void userHasEnteredCoordinates() {
  // Se lee esa información y se almacena en la variable global
  coordinatesString = Serial.readString();
  Serial.print("$ El usuario ha introducido: " + coordinatesString);

  // Mientras haya coordenadas en el String
  while (hasCoordenatesAndDelay()) {
    // Se obtiene el String del primer grupo (coordenada,delay)
    String coordinateAndDelayString = getCoordinateAndDelayString();

    // Se convierte en un array de dos enteros (1ªpos -> coordenada, 2ªpos -> tiempo delay)
    int coordinateAndDelay[2];
    getCoordinateAndDelayFromString(coordinateAndDelayString, coordinateAndDelay); // el valor queda en el array pasado como parámetro

    // Se pasa la coordenada y el delay al método que efectua el movimiento
    userHasEnteredACoordinate(coordinateAndDelay[0], coordinateAndDelay[1]);
  }
}

/*
  Retorna cierto si el tamaño de la variable global coordinatesString es mayor de 0, es decir,
  que aún hay coordenadas en la variable global coordinatesString que no han sido utilizadas.
*/
boolean hasCoordenatesAndDelay() {
  return coordinatesString.length() > 0;
}

/*
  Retorna el substring desde el comienzo de la variable global coordinatesString hasta que
  encuentre el separador ';' y elimina de la variable global dicho substring (incluido el separador).
*/
String getCoordinateAndDelayString() {
  return getStringUntilSeparator(';');
}

/*
  Retorna el substring desde el comienzo de la variable global coordinatesString hasta que encuentre el
  separador indicado (no lo incluye), y elimina de la variable global dicho substring (incluido el separador).
*/
String getStringUntilSeparator(char separator) {
  int i = 0;
  while (coordinatesString.charAt(i) != separator) {
    i++;
  }

  String result = coordinatesString.substring(0, i); // Almacenamos el String hasta el ; (sin incluir)
  coordinatesString.remove(0, i + 1); // Eliminamos los caracteres del String hasta el ; (incluido)

  return result;
}

/*
  Recibe un String con el formato "coordenada,tiempo_delay", y un array que va a utilizar como parámetro de salida
  para almacenar en la posición 0 la coordenada, y en la posición 1 el tiempo_delay (ambos transformados a enteros).
*/
void getCoordinateAndDelayFromString(String coordinateAndDelayString, int coordinateAndDelay[]) {
  int i = 0;
  while (coordinateAndDelayString.charAt(i) != ',') {
    i++;
  }

  String coordinateString = coordinateAndDelayString.substring(0, i); // Almacenamos el String hasta el char ',' (sin incluir)
  String delayString = coordinateAndDelayString.substring(i + 1); // Almacenamos el String desde el siguiente char a ',' hasta el final

  // Modificamos el array (que es pasado por referencia), introduciendo los valores como enteros
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
