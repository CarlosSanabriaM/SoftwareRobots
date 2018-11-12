// Pins
int led1 = 2; 
int led2 = 3; 
int led3 = 4;

void setup() {
  // Inicializamos los pins digitales como salida 
  // Queremos que alimenten electricamente los leds 
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

// El loop es llamado una y otra vez sin parar 
void loop() {
  digitalWrite(led1, HIGH); // Voltaje a - HIGH para el led 1. 
  delay(500);
  
  digitalWrite(led1, LOW); // Voltaje a - LOW para el led 1. 
  digitalWrite(led2, HIGH); // Voltaje a - HIGH para el led 2. 
  delay(500);
  
  digitalWrite(led2, LOW); // Voltaje a - LOW para el led 1. 
  digitalWrite(led3, HIGH); // Voltaje a - HIGH para el led 2. 
  delay(500);
  
  digitalWrite(led3, LOW); // Voltaje a - LOW para el led 3. 
}
