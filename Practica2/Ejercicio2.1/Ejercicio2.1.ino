#include <Keypad.h>

// Pines utilizados para los estados abierto y cerrado de la puerta
const int ledDoorOpened = 6; // es un led rojo, que cuando está encendido indica puerta abierta/abriéndose
const int ledDoorClosed = 7; // es un led verde, que cuando está encendido indica puerta cerrada/cerrándose

// Teclado
const byte numRows = 4; // numero de filas del teclado
const byte numColumns = 4; // numero de columnas del teclado
// Matriz con todas las teclas del teclado
char keys[numRows][numColumns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowsPins[numRows] = {2, 3, 4, 5}; // Pines utilizados para las filas
byte columnsPins[numColumns] = {A0, A1, A2, A3}; // Pines utilizados para las columnas

// Creamos un objeto Keypad que representa el teclado
Keypad _keypad = Keypad(makeKeymap(keys), rowsPins, columnsPins, numRows, numColumns);

// Buffer de lectura en el que iremos almacenando todas las teclas que pulsa el usuario
String readingBuffer = "";


void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");

  // Salidas de los leds
  pinMode(ledDoorOpened, OUTPUT);
  pinMode(ledDoorClosed, OUTPUT);

  // Inicialmente la puerta está cerrada
  digitalWrite(ledDoorClosed, HIGH);
}

void loop() {
  // Obtenemos la tecla pulsada (devuelve '\0' si no se pulsó ninguna)
  char key = _keypad.getKey();

  // Si se pulsó alguna tecla
  if (key != '\0') {
    Serial.println("Tecla pulsada: " + String(key));

    // Si la tecla pulsada es la A
    if (key != 'A') {
      // Abrimos la puerta durante 5 segundos
      openTheDoorFor5Seconds();
    }
  }
}


void openTheDoorFor5Seconds() {
  openTheDoor();
  delay(5000);
  closeTheDoor();
}

void openTheDoor() {
  digitalWrite(ledDoorClosed, LOW);
  digitalWrite(ledDoorOpened, HIGH);
}

void closeTheDoor() {
  digitalWrite(ledDoorClosed, HIGH);
  digitalWrite(ledDoorOpened, LOW);
}
