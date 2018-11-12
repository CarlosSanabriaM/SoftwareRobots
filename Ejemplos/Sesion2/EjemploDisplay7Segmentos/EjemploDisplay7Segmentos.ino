#include <TM1637.h>

long distance;
int pinClk = 3;
int pinDio = 4;

TM1637 screen(pinClk, pinDio);

void setup() {
  Serial.begin(9600);

  // Inicializamos la pantalla de 7 segmentos y 4 dígitos
  screen.init();
  screen.set(BRIGHT_TYPICAL); //BRIGHT_TYPICAL = 2; BRIGHT_DARKEST = 0; BRIGHTEST = 7;

  delay(1500);// Esperamos a que se inicie la pantalla
}

void loop() {
  distance = 1245;

  int digit0 = distance / 1000;
  Serial.println("Digit0 " + String(digit0));
  int digit1 = (distance - digit0 * 1000) / 100;
  Serial.println("Digit1 " + String(digit1));
  int digit2 = (distance - (digit0 * 1000 + digit1 * 100)) / 10;
  Serial.println("Digit2 " + String(digit2));
  int digit3 = distance - (digit0 * 1000 + digit1 * 100 + digit2 * 10);
  Serial.println("Digit3 " + String(digit3));

  screen.display(0, digit0);
  screen.display(1, digit1);
  screen.display(2, digit2);
  screen.display(3, digit3);

  delay(2000);

  screen.clearDisplay();
  screen.display(1, digit0);
  screen.display(2, digit1);

  delay(5000);
}
