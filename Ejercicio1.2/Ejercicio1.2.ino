// Pins
const int redButton = 2;
const int greenButton = 5;
const int redLed = 3;
const int greenLed = 4;

// Almacena si el botón está pulsado (1) o despulsado (0)
int redButtonPushed = 0;
int greenButtonPushed = 0;

// Indica el tamaño máximo de la secuencia
const int MAX_COLOR_SEQUENCE_SIZE = 10;
// Indica el tamaño inicial de la secuencia
const int INITIAL_COLOR_SEQUENCE_SIZE = 3;
// Almacena una secuencia de hasta 10 colores
int colorSequence[10];
// Indica el nº colores almacenados actualmente en el array anterior
int colorSequenceSize = INITIAL_COLOR_SEQUENCE_SIZE;

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
  if(haveToGenerateColorSequence){
    generateRandomColorSequence();
    haveToGenerateColorSequence = false;
  }
  
  if(haveToShowColorSequence){
    showColorSequence();
    haveToShowColorSequence = false;
  }

  // TODO- igual merece la pena sacar esta parte a otra funcion
  // Si el jugador ha acertado todos los colores de la secuencia actual
  if(numSuccessfulColor == colorSequenceSize){
    // Se incrementa la secuencia (en caso de ser posible)
    incrementColorSequence();
    // Se establece el numero de aciertos a 0
    numSuccessfulColors = 0;
    // Se indica que hay que volver a mostrar la secuencia
    haveToShowColorSequence = true;
  }

  if(jugadorAcierta){
    // Se incrementa el número de colores acertados
    numSuccessfulColors++;
  }

  if(jugadorFalla){
    haveToGenerateColorSequence = true;
    haveToShowColorSequence = true;
  }
  
}

void showColorSequence() {
  for (int i = 0; i < colorSequenceSize; i++) {
    switchOnLed(colorSequence[i]);
    delay(1000); // esperamos un seg entre cada encendido
    switchOffLed(colorSequence[i]);
  }
}

void switchOffLed(int ledColor) {
  switchLed(ledColor, LOW);
}

void switchOnLed(int ledColor) {
  switchLed(ledColor, HIGH);
}

void switchLed(int ledColor, int value) {
  switch (ledColor) {
    case 0: // rojo
      digitalWrite(redLed, value);
      break;
    case 1: // verde
      digitalWrite(greenLed, value);
      break;
  }
}

/* Devuelve un numero aleatorio: 0-rojo, 1-verde */
int randomColor() {
  // Generamos numero aleatorio entre 0 y 1
  int randomColor = random(2);
  Serial.println("Color aleatorio: " + randomColor == 0 ? "rojo" : "verde");
  return randomColor;
}

void generateRandomColorSequence() {
  // Generamos la secuencia inicial de forma aleatorio
  for (int i = 0; i < INITIAL_COLOR_SEQUENCE_SIZE; i++) {
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
  else{
    colorSequenceSize = INITIAL_COLOR_SEQUENCE_SIZE;
    // generateRandomColorSequence(); TODO - esto y la linea siguiente son equivalentes, pero creo que queda mejor dejar lo siguiente
    haveToGenerateColorSequence = true;
  }
}
