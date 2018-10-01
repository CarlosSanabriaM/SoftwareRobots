/*
   Circuito disponible en Tinkercad en el siguiente enlace:
   https://www.tinkercad.com/things/7p5eHTP9m4u

   La sección "Código" está vacía. Hay que copiar el código de
   este fichero e introducirlo ahí para probar el funcionamiento.
*/

// Pins
const int redButton = 2;
const int greenButton = 5;
const int redLed = 6;
const int greenLed = 7;
const int blueLed = 8;
const int pinBuzzer = 11;

// Sonido para cada color
const int redSound = 698;
const int greenSound = 523;
const int failSound = 200;

// Almacena si el botón está pulsado (1) o despulsado (0)
int redButtonPushed = 0;
int greenButtonPushed = 0;

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

void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");

  // Entradas de los botones
  pinMode(redButton, INPUT);
  pinMode(greenButton, INPUT);

  // Salidas de los leds
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  // Leemos un pin analógico vacío para que el ruido analógico cause que la función
  // randomSeed() genere diferentes semillas cada vez que se inicia el sketch, para la función random()
  randomSeed(analogRead(0));

  generateRandomColorSequence();
  showColorSequence();
}

void loop() {

  // Comprobamos la pulsación de los botones
  if (checkIfRedButtonIsPushed()) {
    // Si se pulsa el botón rojo, es que el jugador quiere indicar que en la secuencia se debe encender el led rojo
    checkUserTry(redLed); // comprobamos esa suposición del usuario
  }
  // Si se ha pulsado el botón rojo, no se comprueba si se está pulsando el verde en ese mismo instante
  else if (checkIfGreenButtonIsPushed()) {
    // Si se pulsa el botón verde, es que el jugador quiere indicar que en la secuencia se debe encender el led verde
    checkUserTry(greenLed); // comprobamos esa suposición del usuario
  }

}

void checkUserTry(int userLedColor) {
  // Comparamos el color introducido por el usuario con el color que debía haber introducido
  int expectedLedColor = colorSequence[numSuccessfulColors];

  // Si acierta
  if (userLedColor == expectedLedColor) {
    Serial.println("\n-El jugador ha acertado el color\n");

    // Se incrementa el número de colores acertados
    numSuccessfulColors++;

    // Comprobamos si el jugador ha acertado toda la secuencia
    checkIfPlayerHasCompletedColorSequence();
  }
  // Si falla
  else {
    Serial.println("\n-El jugador ha fallado el color\n");

    // Encedemos el led azul para indicar que el jugador ha fallado
    switchOnLedForOneSecondAndPlaySound(blueLed);

    // El juego se debe resetear (se establece el nº aciertos a 0, se genera una nueva secuencia, y se muestra)
    numSuccessfulColors = 0;
    generateRandomColorSequence();
    showColorSequence();
  }
}

void checkIfPlayerHasCompletedColorSequence() {
  // Si el jugador ha acertado todos los colores de la secuencia actual
  if (numSuccessfulColors == colorSequenceSize) {
    Serial.println("--El jugador ha acertado todos los colores de la secuencia actual\n");

    // Se incrementa la secuencia (en caso de ser posible)
    incrementColorSequence();
    // Se establece el numero de aciertos a 0
    numSuccessfulColors = 0;
    // Se vuelve a mostrar la secuencia
    showColorSequence();
  }
}

/* Enciende el led sólo cuando se suelte el botón */
boolean checkIfRedButtonIsPushed() {
  // Comprobamos si el botón está pulsado o despulsado actualmente
  int redButtonActualState = digitalRead(redButton);

  // Si han pulsado el botón rojo y estaba sin pulsar
  if (redButtonActualState == HIGH && redButtonPushed == 0) {
    Serial.println("Boton rojo pulsado");
    // Marcamos el botón como pulsado
    redButtonPushed = 1;
  }

  // Si han quitado el dedo del botón rojo
  else if (redButtonActualState == LOW && redButtonPushed == 1) {
    // Lo marcamos como despulsado
    Serial.println("Boton rojo despulsado");
    redButtonPushed = 0;

    // Encendemos el led rojo durante 1 seg
    switchOnLedForOneSecondAndPlaySound(redLed);

    Serial.println("Led termina de encenderse despues de pulsar boton");

    // Devolvemos cierto, porque el jugador ha introducido un color
    return true;
  }

  // Devolvemos falso, porque el jugador NO ha introducido un color
  return false;
}

/* Enciende el led sólo cuando se suelte el botón */
boolean checkIfGreenButtonIsPushed() {
  // Comprobamos si el botón está pulsado o despulsado actualmente
  int greenButtonActualState = digitalRead(greenButton);

  // Si han pulsado el botón rojo y estaba sin pulsar
  if (greenButtonActualState == HIGH && greenButtonPushed == 0) {
    Serial.println("Boton verde pulsado");
    // Marcamos el botón como pulsado
    greenButtonPushed = 1;
  }

  // Si han quitado el dedo del botón rojo
  else if (greenButtonActualState == LOW && greenButtonPushed == 1) {
    // Lo marcamos como despulsado
    Serial.println("Boton verde despulsado");
    greenButtonPushed = 0;

    // Encendemos el led rojo durante 1 seg
    switchOnLedForOneSecondAndPlaySound(greenLed);

    Serial.println("Led termina de encenderse despues de pulsar boton");

    // Devolvemos cierto, porque el jugador ha introducido un color
    return true;
  }

  // Devolvemos falso, porque el jugador NO ha introducido un color
  return false;
}

void showColorSequence() {
  Serial.println("\n>Se muestra la secuencia de colores\n");

  for (int i = 0; i < colorSequenceSize; i++) {
    switchOnLedForOneSecondAndPlaySound(colorSequence[i]);
    delay(1000); // se espera un segundo entre cada parpadeo
  }
}

void switchOnLedForOneSecondAndPlaySound(int ledColor) {
  playSound(ledColor);

  digitalWrite(ledColor, HIGH);
  delay(500);
  stopSound();  
  delay(500);
  digitalWrite(ledColor, LOW);
}

void playSound(int ledColor){
  // En función del color del led, reproducimos un sonido u otro
  switch (ledColor) {
    case redLed:
      tone(pinBuzzer, redSound);
      break;
    case greenLed:
      tone(pinBuzzer, greenSound);
      break;
    case blueLed:
      tone(pinBuzzer, failSound);
      break;
  }
}

void stopSound(){
  noTone(pinBuzzer);
}

int randomColor() {
  // Generamos numero aleatorio que solo pueda tomar dos valores: 0 ó 1
  long randomColor = random(2);
  // El 0 representa el rojo y el 1 el verde
  Serial.println(String("Color generado aleatoriamente: ") + String(randomColor == 0L ? "rojo" : "verde"));
  // Si el número aleatorio generado es 0, retornamos el pin donde se encuentra el led rojo
  // y si no (si es 1), retornamos el pin donde se encuentra el led verde
  return randomColor == 0L ? redLed : greenLed;
}

void generateRandomColorSequence() {
  Serial.println("@Se genera una secuencia aleatoria de tamanio inicial: "
                 + String(INITIAL_COLOR_SEQUENCE_SIZE) + String("\n"));

  // Se establece el tamaño de la secuencia con el valor que debe tener al inicio de un nuevo juego
  colorSequenceSize = INITIAL_COLOR_SEQUENCE_SIZE;

  // Generamos la secuencia inicial de forma aleatorio
  for (int i = 0; i < colorSequenceSize; i++) {
    colorSequence[i] = randomColor();
  }
  serialPrintColorSequence();
}

void incrementColorSequence() {
  // Si no se ha alcanzado el tamaño máximo de la sequencia:
  if (colorSequenceSize < MAX_COLOR_SEQUENCE_SIZE) {
    // Añadimos un nuevo color a la sequencia
    colorSequence[colorSequenceSize] = randomColor();
    colorSequenceSize++;

    Serial.println("\n#Se aniade un nuevo color a la secuencia");
    serialPrintColorSequence();
  }
  // Si se ha alcanzado, se genera una nueva secuencia aleatoria con el tamaño inicial
  else {
    colorSequenceSize = INITIAL_COLOR_SEQUENCE_SIZE;
    generateRandomColorSequence();

    Serial.println("\n#Se ha llegado al maximo tamanio posible de la secuencia. "
                   "Se va a generar una nueva secuencia del tamanio inicial\n");
  }
}

/* Muestra por la consola los valores actuales de la secuencia */
void serialPrintColorSequence() {
  String stringColorSequence = "\n$Secuencia de colores: [";

  for (int i = 0; i < colorSequenceSize; i++) {
    stringColorSequence += colorSequence[i] == redLed ? "rojo" : "verde";
    if (i < colorSequenceSize - 1)
      stringColorSequence += ", ";
  }
  stringColorSequence += "]";

  Serial.println(stringColorSequence);
}
