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

const int NO_LINE = LOW; // valor que devuelve el sensor IR cuando no detecta linea
const int LINE = HIGH; // valor que devuelve el sensor IR cuando detecta linea

const int leftServoForward = 0;
const int rightServoForward = 180;
const int leftServoBackward = 180;
const int rightServoBackward = 0;
const int stopServo = 90;
const int leftServoForwardSlow = 20; // La rueda izquierda avanza hacia delante, pero más despacio que a la velocidad máxima.

// Variables para el sensor de ultrasonidos (sensor exterior)
const int pinTrig = 4;
const int pinEcho = 5;
const long DISTANCE_SOMETHING_IN_FRONT = 20; // distancia en cm a partir de la cual se considera que hay algo delante del robot
const int DELAY_BETWEEN_ULTRASONIC_SENSOR_MEASUREMENTS = 300; // tiempo mínimo (en milisegundos) que se ha de esperar entre cada medición del sensor de ultrasonidos (recomendable más de 20 microsegundos)
unsigned long timeLastCheckIfThereIsSomethingInFront; // Almacena el instante el que se comprobó por última vez si algo se movía (en milisegundos)

const int INITIAL_STOP_ROBOT_TIME = 5000;
const int GO_BACKWARD_TIME = 700; // Tiempo durante el cual el robot avanza hacia atrás al estar a punto de caerse del tatami.
const int TURN_AROUND_TIME = 1400; // Tiempo durante el cual el robot gira sobre sí mismo para dar la vuelta

boolean somethingInFrontOfTheRobot = false;



void setup() {
  Serial.begin(9600);
  
  // Para cada uno de los 4 pines de los sensores IR, los inicializamos como pines de entrada
  for(int i=0; i < 4; i++) {
    pinMode(IRSensorsPins[i], INPUT);
  }

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  // Lanzamos una primera medición para ignorar la primera medición del sensor de ultrasonidos, que suele dar 0cm
  getDistanceFromUltrasonicDistanceSensor();

  delay(INITIAL_STOP_ROBOT_TIME);
}

void loop() {
  updateAllIRSensors();

   //_ _ _ _
  if (assertIRSensorsValues(NO_LINE, NO_LINE, NO_LINE, NO_LINE)) {
    tryToCheckIfThereIsSomethingInFront();
    // Si el contricante está delante, lo empuja hasta que ya no esté, salvo si se va a caer del tatami
    if(somethingInFrontOfTheRobot)
      pushOpponent();
    else 
  }
  // Hay algun sensor que detecta línea
  else {
    goBackward();
    turnAround();
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
  delay(GO_BACKWARD_TIME);
  stopRobotAndDelayALittle();
}

/* Le indica al robot que se de la vuelta. */
void turnAround() {
  // Gira sobre sí mismo en sentido anti-horario, 180º
  leftServo.write(leftServoBackward);
  rightServo.write(rightServoForward);
  delay(TURN_AROUND_TIME);
  stopRobotAndDelayALittle();
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

/* Si ha pasado el tiempo minimo entre mediciones del sensor de ultrasonidos, comprueba si hay algo delante del robot. */
void tryToCheckIfThereIsSomethingInFront() {
  // Si ha pasado el tiempo mínimo entre mediciones con el sensor de ultrasonidos desde la última vez que se comprobó si había algo delante
  if (millis() - timeLastCheckIfThereIsSomethingInFront >= DELAY_BETWEEN_ULTRASONIC_SENSOR_MEASUREMENTS) {
    checkIfThereIsSomethingInFront();
  }
}

/* Comprueba si hay algo delante del robot, para en ese caso empujarlo. */
void checkIfThereIsSomethingInFront() {
  timeLastCheckIfThereIsSomethingInFront = millis(); // se establece el tiempo actual como el de la última comprobación

  // Si detecta algo a una distancia menor o igual que la indicada
  if (getDistanceFromUltrasonicDistanceSensor() <= DISTANCE_SOMETHING_IN_FRONT)
    somethingInFrontOfTheRobot = true;
  else 
    somethingInFrontOfTheRobot = false;
}

/* Empujar al robot que tiene delante. */
void pushOpponent() {
  Serial.println("Empujar contrincante.");

  // Avanza hasta que no tenga nada delante (siempre que no esté a punto de salirse del tatami).
  goForward();
  while (somethingInFrontOfTheRobot && assertIRSensorsValues(LINE, LINE, LINE, LINE)){
    tryToCheckIfThereIsSomethingInFront();
    updateAllIRSensors();
  }
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

/* Actualiza las mediciones de los sensores de infrarrojos. */
void updateAllIRSensors() {
  // Para cada uno de los 4 sensores IR, actualiza su valor
  for(int i=0; i < 4; i++) {
    IRSensorsValues[i] = digitalRead(IRSensorsPins[i]);
  }
  // Se realizan 4 lecturas adicionales, para estar seguros del valor de cada sensor
  // Se guardan las que detectan línea en cualquiera de los 4 barridos
  for(int i=0; i < 1; i++) { // 4 barridos
    delay(20); // Delay con el movimiento actual

    for(int i=0; i < 4; i++) { // para cada 1 de los 4 sensores IR
      if (digitalRead(IRSensorsPins[i]) == LINE )
        IRSensorsValues[i] = LINE;
    }
  }
}

void stopRobotAndDelayALittle(){
  stopRobot();
  delay(300);
}
