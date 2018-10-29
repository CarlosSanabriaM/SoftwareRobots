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
boolean linealActuatorInCalibrationModeHasToGoToTheRight = false;
unsigned long timeFullTravel; // tiempo (en ms) que tarda en moverse el actuador lineal de la izda a la dcha del todo.
const int NUM_POSITIONS = 24; // número de coordenadas del recorrido
int currentCoordinate; // almacena la coordenada actual en la que se encuentra el actuador lineal

// Variables para las coordenadas harcodeada
int harcodedCoordinates[5] = {10, 5, 6, 8, 4};
int harcodedCoordinateToChoose = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");
  servo.attach(servoPin);
  pinMode(joystickButtonPin, INPUT_PULLUP); // usamos la resistencia interna del Arduino

  moveInCalibrationMode();
}

void loop() {
  checkIfUserEntersACoordinate();
}


void moveInCalibrationMode() {
  // Se mueve hacia la dcha
  if (linealActuatorInCalibrationModeHasToGoToTheRight ) {
    checkIfHitsRightCollisionSensor();
    moveLinealActuatorToTheRight();
  }
  // Se mueve hacia la izda
  else {
    checkIfHitsLeftCollisionSensor();
    moveLinealActuatorToTheLeft();
  }
}

void checkIfHitsLeftCollisionSensor() {
  // Si el actuador choca con el sensor de colisión de la izda, le cambiamos la dirección a la derecha
  if (digitalRead(leftCollisionSensorPin) == collisionSensorActiveValue) {
    Serial.println("\n# El actuador colisiona con el sensor de colisión de la izquierda."
                   "\n# Se cambia la dirección de movimiento del actuador a la derecha.\n");

    linealActuatorInCalibrationModeHasToGoToTheRight = true;

    // Se empieza a contar el tiempo
    timeLinealActuatorHitsLeftCollisionSensor = millis();
  }
}

void checkIfHitsRightCollisionSensor() {
  // Si el actuador choca con el sensor de colisión de la dcha, le cambiamos la dirección a la izda
  if (digitalRead(rightCollisionSensorPin) == collisionSensorActiveValue) {
    Serial.println("\n# El actuador colisiona con el sensor de colisión de la derecha."
                   "\n# Se cambia la dirección de movimiento del actuador a la izquierda.\n");

    linealActuatorInCalibrationModeHasToGoToTheRight = false;

    // Se calcula el tiempo que se tarda en hacer el recorrido completo, de izda a dcha.
    timeFullTravel = millis() - timeLinealActuatorHitsLeftCollisionSensor;
    Serial.println("\n@ Tiempo en ms del recorrido completo: " + String(timeFullTravel));

    // La coordenada actual en la que queda es la máxima
    currentCoordinate = NUM_POSITIONS;
  }
}

void checkIfUserEntersACoordinate() {
  // Obtenemos el valor del la coordenada
  int coordinate = harcodedCoordinates[harcodedCoordinateToChoose++];
  Serial.println("\n- Coordenada introducida: " + String(coordinate));

  // Si el valor de X es menor que el valor límite mínimo de la posicion central es que está hacia la izda
  if (coordinate <= 0) {
    // Lo movemos a la coordenada 0 (hasta que llegue al sensor colision izda)
    moveLinealActuatorToTheLeft();
    // COMPROBAR QUE NO CHOCA CON EL SENSOR IZDA
  }
  else if (coordinate >= NUM_POSITIONS) {
    moveLinealActuatorToTheRight();
    // COMPROBAR QUE NO CHOCA CON EL SENSOR IZDA
  }
  else {
    // Movemos el actuador a la coordenada indicada
    moveLinealActuatorToCoordinate(coordinate);
  }
}

void moveLinealActuatorToCoordinate(int coordinate) {
  // En función de la coordenada donde está, se tendrá que mover a la izda o a la dcha un determinado tiempo
  currentCoordinate;

  timeFullTravel / NUM_POSITIONS * 3 ms

  currentCoordinate = coordinate;
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
