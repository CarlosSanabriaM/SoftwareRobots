/*
   Circuito disponible en Tinkercad en el siguiente enlace:
   https://www.tinkercad.com/things/hlJc0ERJcDu

   La sección "Código" está vacía. Hay que copiar el código de
   este fichero e introducirlo ahí para probar el funcionamiento.
*/

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

// Indica el estado de la puerta (abierta/cerrada)
boolean doorIsOpened = false;
// Almacena el instante en el que se abrió la puerta en milisegundos
double timeDoorWasOpened;
// Almacena la contraseña
String password = "9876";

// Buffer de lectura en el que iremos almacenando todas las teclas que pulsa el usuario
String readingBuffer = "";

// Creamos un objeto Keypad que representa el teclado
Keypad _keypad = Keypad(makeKeymap(keys), rowsPins, columnsPins, numRows, numColumns);

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
  // Comprobamos si la puerta está abierta y han pasado los 5 segundos
  checkIfTimeHasPassed();

  // Comprobamos si se pulsa el teclado
  checkKeystrokes();
}

void checkIfTimeHasPassed() {
  // Si la puerta está abierta y han pasado 5 segundos desde que se abrió
  if (doorIsOpened && millis() - timeDoorWasOpened >= 5000) {
    Serial.println("\n> Se acaban los 5 segundos de apertura");
    // Se cierra la puerta
    closeTheDoor();
  }
}

void checkKeystrokes() {
  // Obtenemos la tecla pulsada (devuelve '\0' si no se pulsó ninguna)
  char key = _keypad.getKey();

  // Si se pulsó alguna tecla
  if (key != '\0') {
    userHasPressedAKey(key);
  }
}

void userHasPressedAKey(char key) {
  Serial.println("Tecla pulsada: " + String(key));

  // Si la puerta está cerrada
  if (!doorIsOpened) {
    userHasPressedAKeyWhileDoorWasClosed(key);
  }
  // Si la puerta está abierta y se pulsa la tecla C
  else if (doorIsOpened && key == 'C') {
    Serial.println("\n> Se fuerza al cierre de la puerta");
    // Se cierra la puerta
    closeTheDoor();
  }
}

void userHasPressedAKeyWhileDoorWasClosed(char key) {
  // Se añade la tecla al buffer
  readingBuffer = readingBuffer + key;
  Serial.println("Contenido buffer lectura: " + readingBuffer);

  // Si se han introducido 4 teclas
  if (readingBuffer.length() == 4) {
    userHasIntroducedAPassword();
  }
}

void userHasIntroducedAPassword(){
  Serial.println("\n@ Contrasenia introducida: " + readingBuffer);
  
  // Si la contraseña introducida coincide con la correcta
  if (readingBuffer == password) {
    userHasIntroducedCorrectPassword();
  }
  // Si la contraseña introducida NO coincide con la correcta
  else {
    userHasIntroducedWrongPassword();
  }
}

void userHasIntroducedCorrectPassword() {
  Serial.println("\n$ Contrasenia correcta");
  
  // Se resetea el buffer
  readingBuffer = "";
  
  // Se apaga el led verde un segundo
  switchOffGreenLedFor1Second();

  // Abrimos la puerta durante 5 segundos
  openTheDoorFor5Seconds();
}

void userHasIntroducedWrongPassword() {
  Serial.println("\n$ Contrasenia incorrecta\n");
  
  // Se resetea el buffer
  readingBuffer = "";
  
  // El led verde parpadea 3 veces y se queda encendido
  blinkGreenLed3Times();
}

void openTheDoorFor5Seconds() {
  Serial.println("\n# Se abre la puerta");

  // Se encienden/apagan los leds correspondientes
  digitalWrite(ledDoorClosed, LOW);
  digitalWrite(ledDoorOpened, HIGH);

  // Se indica que la puerta está abierta y se almacena el momento en el que se abrió
  doorIsOpened = true;
  timeDoorWasOpened = millis();
}

void closeTheDoor() {
  Serial.println("# Se cierra la puerta\n");

  // Se encienden/apagan los leds correspondientes
  digitalWrite(ledDoorClosed, HIGH);
  digitalWrite(ledDoorOpened, LOW);

  // Se indica que la puerta está cerrada
  doorIsOpened = false;
}

void blinkGreenLed3Times(){
  // Durante este tiempo no se procesan las teclas pulsadas, porque no se ejecuta el método loop
  blinkGreenLed();
  delay(1000);
  blinkGreenLed();
  delay(1000);
  blinkGreenLed();
}

void blinkGreenLed(){
  switchOffGreenLedFor1Second();
  digitalWrite(ledDoorClosed, HIGH);
}

void switchOffGreenLedFor1Second(){
  // Durante este tiempo no se procesan las teclas pulsadas, porque no se ejecuta el método loop
  digitalWrite(ledDoorClosed, LOW);
  delay(1000);
}
