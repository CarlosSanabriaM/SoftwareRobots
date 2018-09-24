// Pins
const int button = 2;
const int led1 = 3;
const int led2 = 4;
const int led3 = 5;

// Almacena si el botón está pulsado (1) o despulsado (0)
int pushed = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");

  // Entrada del boton a leer
  pinMode(button, INPUT);

  // Salidas de los leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  // Comprobamos si el botón está pulsado o despulsado
  int actualButtonState = digitalRead(button);

  // Si han pulsado el botón y estaba sin pulsar
  if (actualButtonState == HIGH && pushed == 0) {
    // Lo marcamos como pulsado
    pushed = 1;

    // Generamos numero aleatorio de 1 a 3
    int randomNumber = random(1, 4);
    Serial.println("Valor aleatorio: " + String(randomNumber));

    // En función del número generado, encendemos/apagamos los leds correspondientes
    switchOnLeds(randomNumber);
  }

  // Si han quitado el dedo del botón
  else if (actualButtonState == LOW && pushed == 1) {
    // Lo marcamos como despulsado
    Serial.println();
    pushed = 0;
  }

}

void switchOnLeds(int randomNumber) {
  switch (randomNumber) {
    case 1:
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      break;
    case 2:
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      break;
    case 3:
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      break;
  }
}
