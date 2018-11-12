int led_red = 0; // the red LED is connected to Pin 0 of the Arduino
int led_yellow = 1; // the yellow LED is connected to Pin 1 of the Arduino
int led_green = 2; // the green LED is connected to Pin 2 of the Arduino

void switchOnLed(int ledPin){
  digitalWrite(ledPin, HIGH); 
}

void switchOffLed(int ledPin){
  digitalWrite(ledPin, LOW); 
}

void setup() {
  // set up all the LEDs as OUTPUT
  pinMode(led_red, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(led_green, OUTPUT);
}

void loop() {
  // turn the green LED on and the other LEDs off
  switchOffLed(led_red);
  switchOffLed(led_yellow);
  switchOnLed(led_green);
  delay(2000);    // wait 2 seconds
  
  // turn the yellow LED on and the other LEDs off
  switchOffLed(led_red);
  switchOffLed(led_green);
  switchOnLed(led_yellow);
    delay(1000);   // wait 1 second
  
  // turn the red LED on and the other LEDs off
  switchOffLed(led_yellow);
  switchOffLed(led_green);
  switchOnLed(led_red);
  delay(3000);  // wait 3 seconds        
}
