#include <Keypad.h>
#include <TM1637.h>

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
boolean doorIsOpenedFromOutside = false; // puerta abierta desde fuera
boolean doorIsOpenedFromInside = false; // puerta abierta desde dentro
// Almacena el instante en el que se abrió la puerta (en milisegundos)
double timeDoorWasOpened;
// Almacena el instante de la última vez que se intentó cerrar la puerta cuando hay alguien saliendo
double timeLastTryToCloseTheDoorWhenUserIsGoingOut;
// Almacena la contraseña
String password = "9876";

// Buffer de lectura en el que iremos almacenando todas las teclas que pulsa el usuario
String readingBuffer = "";

// Creamos un objeto Keypad que representa el teclado
Keypad _keypad = Keypad(makeKeymap(keys), rowsPins, columnsPins, numRows, numColumns);

// Variables para el sensor de ultrasonidos (sensor exterior)
const int pinTrig = 9;
const int pinEcho = 8;
const int distanceSomethingInFrontOfTheDoor = 10; // distancia en cm a partir de la cual se considera que hay algo delante de la puerta
const int delayBetweenUltrasonicSensorMeasurements = 2; // tiempo mínimo (en milisegundos) que se ha de esperar entre cada
// medición del sensor de ultrasonidos (recomendable más de 20 microsegundos)

// Variables para el sensor de luz (sensor interior)
const int lightSensorDetectionValue = 150;

// Número de usuarios que han entrado por la puerta
int numUsersHaveEntered = 0;

// Variables para la pantalla de 7 segmentos y 4 dígitos
int pinClk = 11; // ha de ser un pin digital PWM
int pinDio = 12;

// Creamos un objeto TM1637 que representa la pantalla
TM1637 screen(pinClk, pinDio);

void setup() {
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");

  // Salidas de los leds
  pinMode(ledDoorOpened, OUTPUT);
  pinMode(ledDoorClosed, OUTPUT);

  // Inicialmente la puerta está cerrada
  digitalWrite(ledDoorClosed, HIGH);

  // Se configuran los pines del sensor ultrasonidos
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  // Inicializamos la pantalla de 7 segmentos y 4 dígitos
  screen.init();
  screen.set(BRIGHT_TYPICAL); //BRIGHT_TYPICAL = 2; BRIGHT_DARKEST = 0; BRIGHTEST = 7;

  // Esperamos a que se inicie la pantalla
  delay(1500);

  // Lanzamos una primera medición para ignorar el 0cm
  getDistanceFromUltrasonicDistanceSensor();
}

void loop() {
  // Si la puerta no está abierta desde dentro (está cerrada o está abierta desde fuera)
  if (!doorIsOpenedFromInside) {
    // Comprobamos si la puerta está abierta y han pasado los 5 segundos, para intentar cerrarla
    checkIfTimeHasPassed();

    // Comprobamos si la puerta está abierta y alguien ha entrado, para intentar cerrarla
    checkIfSomeoneHasEntered();

    // Comprobamos si se pulsa el teclado, por si se pulsa C y la puerta está abierta, intentar cerrarla,
    // o almacenar contraseñas si está cerrada
    checkKeystrokes();

    // Comprobamos si la puerta está cerrada y hay alguien dentro, para entonces abrir la puerta
    checkIfSomeoneWantsToGoOutside();
  }
  // Si la puerta está abierta desde dentro
  else {
    // Comprobamos si ya no hay nadie dentro, para intentar cerrar la puerta
    checkIfSomeoneHasGoneOutside();
  }
}


void checkIfTimeHasPassed() {
  // Si la puerta está abierta y han pasado 5 segundos desde que se abrió
  if (doorIsOpenedFromOutside && millis() - timeDoorWasOpened >= 5000) {
    Serial.println("\n> Se acaban los 5 segundos de apertura");
    // Se intenta cerrar la puerta
    tryToCloseTheDoorWhenUserIsEntering(); // el usuario está entrando
  }
}

void checkIfSomeoneHasEntered() {
  // Si la puerta está abierta y hay alguien dentro
  if (doorIsOpenedFromOutside && somethingInsideTheDoor()) {
    // Intentamos cerrar la puerta (dentro se comprueba que no hay nada delante)
    tryToCloseTheDoorWhenUserIsEntering(); // el usuario está entrando
  }
}

/*
   Este método sólo es llamado cuando la puerta no está abierta desde dentro.
   Por tanto,si la puerta tampoco está abierta desde fuera, es que está cerrada.
*/
void checkIfSomeoneWantsToGoOutside() {
  // Si está cerrada y hay alguien dentro
  if (!doorIsOpenedFromOutside && somethingInsideTheDoor()) {
    // Abrimos la puerta desde dentro
    openTheDoorFromInside();
  }
}

/*
   Este método sólo es llamado cuando la puerta está abierta desde dentro.
*/
void checkIfSomeoneHasGoneOutside() {
  // Si la puerta está abierta desde dentro y ya no hay nadie dentro
  if (!somethingInsideTheDoor()) {
    // Si ha pasado el tiempo mínimo entre mediciones con el sensor de ultrasonidos desde la
    // última vez que se intentó cerrar la puerta cuando alguien estaba saliendo
    if (millis() - timeLastTryToCloseTheDoorWhenUserIsGoingOut >= delayBetweenUltrasonicSensorMeasurements) {
      // Intentamos cerrar la puerta (dentro se comprueba que no hay nada delante)
      tryToCloseTheDoorWhenUserIsGoingOut(); // el usuario está saliendo
    }
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

/*
   Este método sólo es llamado cuando la puerta no está abierta desde dentro.
   Por tanto,si la puerta tampoco está abierta desde fuera, es que está cerrada.
*/
void userHasPressedAKey(char key) {
  Serial.println("Tecla pulsada: " + String(key));

  // Si la puerta está cerrada
  if (!doorIsOpenedFromOutside) {
    userHasPressedAKeyWhileDoorWasClosed(key);
  }
  // Si la puerta está abierta y se pulsa la tecla C
  else if (doorIsOpenedFromOutside && key == 'C') {
    Serial.println("\n> Se fuerza al cierre de la puerta");
    // Se intenta cerrar la puerta
    tryToCloseTheDoorWhenUserIsEntering(); // el usuario está entrando
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

void userHasIntroducedAPassword() {
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
  openTheDoor();

  // Se indica que la puerta está abierta y se almacena el momento en el que se abrió
  doorIsOpenedFromOutside = true;
  timeDoorWasOpened = millis();
}

void openTheDoorFromInside() {
  Serial.println("\n# Se abre la puerta desde dentro");
  openTheDoor();

  // Se indica que la puerta está abierta desde dentro
  doorIsOpenedFromInside = true;

  // Se inicia el valor del tiempo del último intento de cerrar la puerta,
  // para que así haya un cierto espacio entre las llamadas al sensor de ultrasonidos
  timeLastTryToCloseTheDoorWhenUserIsGoingOut = millis();
}

void openTheDoor() {
  // Se encienden/apagan los leds correspondientes
  digitalWrite(ledDoorClosed, LOW);
  digitalWrite(ledDoorOpened, HIGH);
}

void tryToCloseTheDoorWhenUserIsEntering() {
  //Si hay alguien delante de la puerta, no la podemos cerrar!
  if (somethingInFrontOfTheDoor()) {
    // Dejamos la puerta abierta otros 5 segundos
    Serial.println("# No se puede cerrar la puerta. Alguien está entrando. Se deja la puerta abierta otros 5 segundos");
    timeDoorWasOpened = millis(); // se vuelve a tomar el tiempo en el que se abrió la puerta
  }
  // Si no hay nadie delante de la puerta, la cerramos
  else closeTheDoorWhenUserIsEntering();
}

void tryToCloseTheDoorWhenUserIsGoingOut() {
  //Si hay alguien delante de la puerta, no la podemos cerrar!
  if (somethingInFrontOfTheDoor()) {
    Serial.println("# No se puede cerrar la puerta. Alguien está saliendo");

    // Se toma el tiempo de la última vez que se intentó cerrar la puerta cuando alguien estaba saliendo
    timeLastTryToCloseTheDoorWhenUserIsGoingOut = millis();
  }
  // Si no hay nadie delante de la puerta, la cerramos
  else closeTheDoorWhenUserIsGoingOut();
}

void closeTheDoorWhenUserIsEntering() {
  Serial.println("# Se cierra la puerta después de que alguien ha entrado\n");
  closeTheDoor();

  // Se indica que la puerta está cerrada
  doorIsOpenedFromOutside = false;

  // Contabilizamos que acaba de entrar un usuario
  newUserHasEntered();
}

void closeTheDoorWhenUserIsGoingOut() {
  Serial.println("# Se cierra la puerta después de que alguien ha salido\n");
  closeTheDoor();

  // Se indica que la puerta está cerrada
  doorIsOpenedFromInside = false;
}

void closeTheDoor() {
  // Se encienden/apagan los leds correspondientes
  digitalWrite(ledDoorClosed, HIGH);
  digitalWrite(ledDoorOpened, LOW);
}

void newUserHasEntered() {
  // Aumentamos el número de usuarios que han entrado
  numUsersHaveEntered++;
  Serial.println("$ Un usuario acaba de entrar. Num usuarios han entrado: " + String(numUsersHaveEntered) + "\n");

  // Actualizamos en el display el número de usuarios que han entrado
  displayNumUsersHaveEntered();
}

/* No se tiene en cuenta que el número de usuarios que han entrado sea mayor de 9999 */
void displayNumUsersHaveEntered() {
  // Se obtiene cada uno de los 4 dígitos del número
  int digit0 = numUsersHaveEntered / 1000;
  int digit1 = (numUsersHaveEntered - digit0 * 1000) / 100;
  int digit2 = (numUsersHaveEntered - (digit0 * 1000 + digit1 * 100)) / 10;
  int digit3 = numUsersHaveEntered - (digit0 * 1000 + digit1 * 100 + digit2 * 10);

  screen.display(0, digit0);
  screen.display(1, digit1);
  screen.display(2, digit2);
  screen.display(3, digit3);
}

void blinkGreenLed3Times() {
  // Durante este tiempo no se procesan las teclas pulsadas, porque no se ejecuta el método loop
  blinkGreenLed();
  delay(1000);
  blinkGreenLed();
  delay(1000);
  blinkGreenLed();
}

void blinkGreenLed() {
  switchOffGreenLedFor1Second();
  digitalWrite(ledDoorClosed, HIGH);
}

void switchOffGreenLedFor1Second() {
  // Durante este tiempo no se procesan las teclas pulsadas, porque no se ejecuta el método loop
  digitalWrite(ledDoorClosed, LOW);
  delay(1000);
}

boolean somethingInFrontOfTheDoor() {
  // Si la distancia al sensor de ultrasonidos es menor o igual que la
  // distancia a partir de la cual se considera que hay algo delante de la puerta
  if (getDistanceFromUltrasonicDistanceSensor() <= distanceSomethingInFrontOfTheDoor) {
    // Hay algo delante de la puerta
    Serial.println("\n> Hay algo delante de la puerta");
    return true;
  }
  else {
    Serial.println("\n> No hay nada delante de la puerta");
    return false;
  }
}

boolean somethingInsideTheDoor() {
  // Leer lectura analógica
  int lightValue = analogRead(A4);

  // Si el valor de la luz es menor o igual que el valor a partir del cual se considera que no hay luz
  if (lightValue <= lightSensorDetectionValue) {
    // Hay alguien dentro
    Serial.println("\n> Hay algo dentro");
    return true;
  }
  else {
    //Serial.println("> No hay nada dentro");
    return false;
  }
}

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
