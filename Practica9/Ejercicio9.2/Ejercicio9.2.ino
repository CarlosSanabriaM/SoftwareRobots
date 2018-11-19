#include <Servo.h>
int NO_LINEA = LOW; // También podría ponerse 0
int LINEA = HIGH; // También podría ponerse 1
Servo servoIzq;
Servo servoDer;
int pinIrDer = 3;
int pinIrIzq = 2;
int pinServoDer = 9;
int pinServoIzq = 8;

int alanteIzquierda = 0;
int alanteDerecha = 180;
int atrasIzquierda = 180;
int atrasDerecha = 0;
int alto = 90;

int iteracionGiro = 10;
unsigned long tiempoDesdeQueEmpezoABuscar = 0;
boolean empiezaABuscarLinea = false;
#define TIEMPO_AUMENTAR_GIRO 5000

void setup() {
  Serial.begin(9600); // Descomentar si queréis debuguear por consola
  pinMode(pinIrDer, INPUT);
  pinMode(pinIrIzq, INPUT);

  servoIzq.attach(pinServoIzq);
  servoDer.attach(pinServoDer);
}

void loop() {
  
  if (digitalRead(pinIrIzq) == LINEA && digitalRead(pinIrDer) == LINEA) {
    seguirDeFrente();
  } else if (digitalRead(pinIrIzq) == LINEA && digitalRead(pinIrDer) == NO_LINEA) {
    girarHaciaIzquierda();
  } else if (digitalRead(pinIrIzq) == NO_LINEA && digitalRead(pinIrDer) == LINEA) {
    girarHaciaDerecha();
  } else {
    buscarLinea();
  }
}

void seguirDeFrente() {
  servoIzq.write(alanteIzquierda);
  servoDer.write(alanteDerecha);
}

void girarHaciaDerecha() {
  servoIzq.write(alanteIzquierda);
  servoDer.write(alto);
}

void girarHaciaIzquierda() {

  servoIzq.write(alto);
  servoDer.write(alanteDerecha);
}

void marchaAtras() {
  servoIzq.write(atrasIzquierda);
  servoDer.write(atrasDerecha);
}

void buscarLinea() {
  if(!empiezaABuscarLinea){
    empiezaABuscarLinea = true;
    tiempoDesdeQueEmpezoABuscar = millis();
  }
  if(millis() - tiempoDesdeQueEmpezoABuscar > TIEMPO_AUMENTAR_GIRO && iteracionGiro < 50 ){
    tiempoDesdeQueEmpezoABuscar = millis();
    iteracionGiro += 3;
  }
  servoIzq.write(alanteIzquierda);
  servoDer.write(alto + iteracionGiro);
}
