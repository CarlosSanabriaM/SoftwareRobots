// Pins
int led1 = 2;
int led2 = 3;
int button = 4;
int buttonState = 0;
int pushed = 0;
// 0: todo apagado
// 1: led1 encendido
// 2: led2 encendido

void setup() {
  Serial.begin(9600);
  // Iniciar el Serial
  Serial.println("Setup");
  // Inicializamos los pines digitales como salida
  // Queremos que alimenten electricamente los leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  // Queremos leer la entrada
  pinMode(button, INPUT);
}

// La funcion loop se llama una y otra vez hasta el infinito
void loop() {
  int read = digitalRead(button);
  // Si han pulsado el botón y estaba sin pulsar ->
  if (read == HIGH && pushed == 0) {
    // botón pulsado!
    pushed = 1;
    Serial.println("Valor HIGH");
    if (buttonState == 2) {
      buttonState = 0;
    } else {
      buttonState++;
    }
    switch (buttonState) {
      case 0:
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        break;
      case 1:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        break;
      case 2:
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        break;
    }
  }

  // Si han quitado el dedo del botón cambio su estado
  else if (read == LOW && pushed == 1) {
    Serial.println("Valor LOW");
    pushed = 0;
  }

}
