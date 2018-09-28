// Pins
const int redButton = 2;
const int greenButton = 5;
const int redLed = 3;
const int greenLed = 4;

// Indica el tamaño máximo de la secuencia
const int MAX_COLOR_SEQUENCE_SIZE = 10;
// Indica el tamaño inicial de la secuencia
const int INITIAL_COLOR_SEQUENCE_SIZE = 3;
// Almacena una secuencia de hasta 10 colores
int colorSequence[10]; // Cada color está representado por el pin del led de dicho color
// Indica el nº colores almacenados actualmente en el array anterior
int colorSequenceSize;

// Indica el número de colores de la secuencia acertados por el jugador
int numSuccessfulColors = 0;
// Indica si se ha de generar de nuevo una secuencia aleatoria del tamaño inicial
boolean haveToGenerateColorSequence = true;
// Indica si se ha de mostrar la secuencia al jugador
boolean haveToShowColorSequence = true;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");

  // Entradas de los botones
  pinMode(redButton, INPUT);
  pinMode(greenButton, INPUT);

  // Salidas de los leds
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

void loop() {
  if (haveToGenerateColorSequence) {
    generateRandomColorSequence();
    haveToGenerateColorSequence = false;
  }

  if (haveToShowColorSequence) {
    showColorSequence();
    haveToShowColorSequence = false;
  }

  // Comprobamos la pulsación de los botones
  if (checkIfButtonIsPushed(redButton)) {
    // Si se pulsa el botón rojo, es que el jugador quiere indicar que en la secuencia se debe encender el led rojo
    checkUserTry(redLed); // comprobamos esa suposición del usuario
  }
  // Si se ha pulsado el botón rojo, no se comprueba si se está pulsando el verde en ese mismo instante
  else if (checkIfButtonIsPushed(greenButton)) {
    // Si se pulsa el botón verde, es que el jugador quiere indicar que en la secuencia se debe encender el led verde
    checkUserTry(greenLed); // comprobamos esa suposición del usuario
  }

}

void checkUserTry(int userLedColor) { // TODO - darle otro nombre mejor a este método??
  // Comparamos el color introducido por el usuario con el color que debía haber introducido
  int expectedLedColor = colorSequence[numSuccessfulColors];

  // Si acierta
  if (userLedColor == expectedLedColor) {
    // Se incrementa el número de colores acertados
    numSuccessfulColors++;

    // Comprobamos si el jugador ha acertado toda la secuencia
    checkIfPlayerHasCompletedColorSequence();
  }
  // Si falla
  else {
    // El juego se debe resetear (se establece el nº aciertos a 0, se genera una nueva secuencia, y se muestra)
    numSuccessfulColors = 0; 
    haveToGenerateColorSequence = true;
    haveToShowColorSequence = true;
  }
}

void checkIfPlayerHasCompletedColorSequence() {
  // Si el jugador ha acertado todos los colores de la secuencia actual
  if (numSuccessfulColors == colorSequenceSize) {
    // Se incrementa la secuencia (en caso de ser posible)
    incrementColorSequence();
    // Se establece el numero de aciertos a 0
    numSuccessfulColors = 0;
    // Se indica que hay que volver a mostrar la secuencia
    haveToShowColorSequence = true;
  }
}

boolean checkIfButtonIsPushed(int buttonColor) {
  // Comprobamos si el botón está pulsado o despulsado
  int buttonActualState = digitalRead(buttonColor);

  // Si está pulsado
  if (buttonActualState == HIGH) {
    // Encendemos el led correspondiente al botón pulsado durante 1 seg
    int ledColor = buttonColor == redButton ? redLed : greenLed;
    switchOnLedForOneSecond(ledColor);
    // Devolvemos cierto, porque el botón está pulsado
    return true;
  }
  // Devolvemos falso, porque el botón no está pulsado
  return false;
}

void showColorSequence() {
  for (int i = 0; i < colorSequenceSize; i++) {
    switchOnLedForOneSecond(colorSequence[i]);
  }
}

void switchOnLedForOneSecond(int ledColor) {
  digitalWrite(ledColor, HIGH);
  delay(1000);
  digitalWrite(ledColor, LOW);
}

int randomColor() {
  // Generamos numero aleatorio que solo pueda tomar dos valores: 0 ó 1
  int randomColor = random(2);
  // El 0 representa el rojo y el 1 el verde
  Serial.println("Color aleatorio: " + randomColor == 0 ? "rojo" : "verde");
  // Si el número aleatorio generado es 0, retornamos el pin donde se encuentra el led rojo
  // y si no (si es 1), retornamos el pin donde se encuentra el led verde
  return randomColor == 0 ? redLed : greenLed;
}

void generateRandomColorSequence() {
  // Se establece el tamaño de la secuencia con el valor que debe tener al inicio de un nuevo juego
  colorSequenceSize = INITIAL_COLOR_SEQUENCE_SIZE;
  
  // Generamos la secuencia inicial de forma aleatorio
  for (int i = 0; i < colorSequenceSize; i++) {
    colorSequence[i] = randomColor();
  }
}

void incrementColorSequence() {
  // Si no se ha alcanzado el tamaño máximo de la sequencia:
  if (colorSequenceSize < MAX_COLOR_SEQUENCE_SIZE) {
    // Añadimos un nuevo color a la sequencia
    colorSequence[colorSequenceSize] = randomColor();
    colorSequenceSize++;
  }
  // Si se ha alcanzado, se genera una nueva secuencia aleatoria con el tamaño inicial
  else {
    colorSequenceSize = INITIAL_COLOR_SEQUENCE_SIZE;
    haveToGenerateColorSequence = true;
  }
}
