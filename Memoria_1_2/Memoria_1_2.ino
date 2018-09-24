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
