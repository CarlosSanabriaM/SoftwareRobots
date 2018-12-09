#include <Servo.h>

/*
  Se va a aplicar el algoritmo de la mano izquierda.
  En un cruce, la prioridad de decisiones es la siguiente:
    1. Girar a la izquierda.
    1. Seguir de frente.
    3. Girar a la derecha.
  Se toma la decisión disponible de mayor prioridad
*/

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

const int INITIAL_STOP_ROBOT_TIME = 5000; // Inicialmente, el robot espera 5 segundos antes de ejecutar el loop
const int TURN_AROUND_TIME = 1000; // Tiempo durante el cual el robot gira sobre sí mismo para dar la vuelta
const int GO_FORWARD_CHECK_CROSSING_TIME = 300; // Tiempo durante el cual el robot avanza hacia delante para comprobar el tipo de cruce
const int STOP_ROBOT_REACH_GOAL_TIME = 10000; // Tiempo durante el cual el robot se queda parado al alcanzar la meta

String robotDecitions = ""; // Almacena las decisiones que toma el robot: giros, avanzar hacia delante, retroceder, ... (solo en cruces o similar)



void setup() {
  //Serial.begin(9600); // Descomentar para debuguear por consola

  // Para cada uno de los 4 pines de los sensores IR, los inicializamos como pines de entrada
  for(int i=0; i < 4; i++) {
    pinMode(IRSensorsPins[i], INPUT);
  }

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  delay(INITIAL_STOP_ROBOT_TIME);
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

/* Le indica al robot que se de la vuelta. */
void turnAround() {
  // Gira sobre sí mismo en sentido anti-horario, 180º
  leftServo.write(leftServoBackward);
  rightServo.write(rightServoForward);
  delay(TURN_AROUND_TIME);

  robotDecitions += "R"; // almacenamos que el robot ha retrocedido (back)
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

/* Le indica al robot que gire hacia la izquierda. */
void turnLeft() {
  // Gira sobre sí mismo en sentido anti-horario, 90º
  leftServo.write(leftServoBackward);
  rightServo.write(rightServoForward);
  delay(TURN_AROUND_TIME/2);
}

/* Le indica al robot que gire hacia la derecha. */
void turnRight() {
  // Gira sobre sí mismo en sentido horario, 90º
  leftServo.write(leftServoForward);
  rightServo.write(rightServoBackward);
  delay(TURN_AROUND_TIME/2);
}

/* Comprueba si es un cruce a la derecha o a la derecha y de frente. */
void checkTypeOfCrossingRight() {
  goForwardAndUpdateAllIRSensors();

  // _ X X _
  if(assertIRSensorsValues(NO_LINE, LINE, LINE, NO_LINE)) {
    // Algo de línea delante, es una curva a la derecha y de frente
    goForward();
    robotDecitions += "A"; // almacenamos que el robot va hacia delante (forward)
  }
  // _ _ _ _
  else { // else if(assertIRSensorsValues(NO_LINE, NO_LINE, NO_LINE, NO_LINE)) {
    // Es una curva solo a la derecha
    turnRight();
    // No se almacena la decisión, porque está obligado a ir a la derecha. No es un cruce con varias opciones.
  }
}

/* Comprueba si es un cruce a la izquierda o a la izquierda y de frente. */
void checkTypeOfCrossingLeft() {
  goForwardAndUpdateAllIRSensors();

  // _ X X _
  if(assertIRSensorsValues(NO_LINE, LINE, LINE, NO_LINE)) {
    // Algo de línea delante, es una curva a la izquierda y de frente
    turnLeft();
    robotDecitions += "I"; // almacenamos que el robot va hacia la izquierda (left)
  }
  // _ _ _ _
  else { // else if(assertIRSensorsValues(NO_LINE, NO_LINE, NO_LINE, NO_LINE)) {
    // Es una curva solo a la izquierda
    turnLeft();
    // No se almacena la decisión, porque está obligado a ir a la izquierda. No es un cruce con varias opciones.
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
    turnLeft();
    robotDecitions += "I"; // almacenamos que el robot va hacia la izquierda (left)
  }
  // _ X X _
  else if(assertIRSensorsValues(NO_LINE, LINE, LINE, NO_LINE)) {
    // Es un cruce a la izquierda, a la derecha y de frente
    turnLeft();
    robotDecitions += "I"; // almacenamos que el robot va hacia la izquierda (left)
  }
  // X X X X
  else if(assertIRSensorsValues(LINE, LINE, LINE, LINE)) {
    // Es la meta
    robotReachesTheGoal();
    // No se almacena la decisión, porque está obligado a ir de frente. No es un cruce con varias opciones.
  }
}

/* Avanza ligeramente y actualiza las mediciones de los sensores IR. */
void goForwardAndUpdateAllIRSensors() {
  // Avanzamos un poco para comprobar qué tipo de cruce es
  goForward();
  delay(GO_FORWARD_CHECK_CROSSING_TIME);

  // Paramos un corto periodo de tiempo
  stopRobot();
  delay(GO_FORWARD_CHECK_CROSSING_TIME);

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

/* El robot ha alcanzado la meta, por lo que se detiene 10 segundos. */
void robotReachesTheGoal() {
  // TODO - quizas se deba dejar avanzar al robot x ms hacia delante para entrar del todo en la meta
  stopRobot();
  reduceDecitions();
  delay(STOP_ROBOT_REACH_GOAL_TIME);
}

/*
  Aplica reglas para reducir las decisiones que debe tomar el robot,
  y llegar así a la meta usando el camino óptimo.
*/
void reduceDecitions() {
  int i = 0;
  Serial.println(robotDecitions);

  while(i <= robotDecitions.length() - 3) {
    if(robotDecitions.substring(i, i + 3) == "ARI") { // coge [i, i+2] caracteres
      applyReduction(i, "D");
      return reduceDecitionsRecursive();
    }
    else if(robotDecitions.substring(i, i + 3) == "IRI") { // coge [i, i+2] caracteres
      applyReduction(i, "A");
      return reduceDecitionsRecursive();
    }
    else if(robotDecitions.substring(i, i + 3) == "IRA") { // coge [i, i+2] caracteres
      applyReduction(i, "D");
      return reduceDecitionsRecursive();
    }
    else if(robotDecitions.substring(i, i + 3) == "DRI") { // coge [i, i+2] caracteres
      applyReduction(i, "R");
      return reduceDecitionsRecursive();
    }

    i++;
    Serial.println(robotDecitions);
  }
}

/* Actualiza el valor de robotDecitions, aplicando la reducción indicada a la parte indicada. */
void applyReduction(int i, String reduction) {
  robotDecitions = robotDecitions.substring(0, i) + reduction + robotDecitions.substring(i+3);
}
