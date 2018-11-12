// Pins
const int redLed = 6;
const int greenLed = 7;
const int blueLed = 8;

void setup() {
  // Salidas de los leds
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchOnLedForOneSecond(redLed);
  switchOnLedForOneSecond(greenLed);
  switchOnLedForOneSecond(blueLed);

  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, HIGH);
}

void switchOnLedForOneSecond(int ledColor) {
  digitalWrite(ledColor, HIGH);
  delay(1000);
  digitalWrite(ledColor, LOW);
}
