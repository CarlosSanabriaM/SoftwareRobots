#include <Servo.h>

#define boton_pin 8 // Pin digital para el botón
#define X_pin A0 // Pin analógico para leer eje X
#define Y_pin A1 // Pin analógico para leer eje Y

void setup() {
  Serial.begin(9600);
  /*
     INPUT_PULLUP agrega una resistencia interna en pull-up de entre 20-50k-ohm,
     depende de la placa, simplificando la utilización de botones y pulsadores.
     De esta forma podemos utilizar directamente el pulsador que incorpora el Joystick.
     Revisad la placa que tenéis para comprobar la resistencia, por si acaso.
  */
  pinMode(boton_pin, INPUT_PULLUP);
}

void loop() {
  /*
      El Joystick permite obtener la medición de los ejes X e Y de forma analógica.
      Los valores se obtienen en el rango de valores 0 - 1023.
      En la posición por defecto el joystick devuelve 500,500.

      Valores devueltos (X, Y) según la posición del joystick,
      teniendo sus pines a nuestra izquierda,
      y siendo la posición central, (500, 500), la posición sin movimiento:
      ----------------------------------------
         0, 0   |    500, 0    |   1024, 0
      ----------------------------------------
        0, 500  |   500, 500   |   1024, 500
      ----------------------------------------
        0, 1024 |   500, 1024  |   1024, 1024
      ---------------------------------------

      Registra la pulsación del botón central mediante una entrada digital,
      1 cuando el botón no está pulsado y 0 cuando lo está.
  */
  
  Serial.println("Boton pulsado: " + String(digitalRead(boton_pin))); 
  
  int valorX = analogRead(X_pin);
  Serial.println("X: " + String(valorX));
  
  int valorY = analogRead(Y_pin); 
  Serial.println("Y: " + String(valorY)); 
  
  delay(1000);
}
