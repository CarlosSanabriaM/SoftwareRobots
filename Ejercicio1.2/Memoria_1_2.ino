// Pins
const int redButton = 2;
const int greenButton = 5;
const int redLed = 3;
const int greenLed = 4;

// Almacena si el botón está pulsado (1) o despulsado (0)
int pushed = 0;

// Almacena una secuencia de hasta 10 colores
int colorSequence[10];
// Indica el tamaño inicial de la secuencia
int INITIAL_COLOR_SEQUENCE_SIZE = 3;
// Indica el nº colores almacenados actualmente en el array anterior
int colorSequenceSize = INITIAL_COLOR_SEQUENCE_SIZE;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");

  // Entradas de los botones
  pinMode(redButton, INPUT);
  pinMode(greenButton, INPUT);

  // Salidas de los leds
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  initializeColorSequence();
}

void loop() {
  drawSequence(); // TODO
  checkSequence(); // TODO
}

void drawSequence() {
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

void initializeColorSequence() {
  // Generamos la secuencia inicial de forma aleatorio
  for (int i = 0; i < INITIAL_COLOR_SEQUENCE_SIZE; i++) {
    colorSequence[i] = randomColor();
  }
}
