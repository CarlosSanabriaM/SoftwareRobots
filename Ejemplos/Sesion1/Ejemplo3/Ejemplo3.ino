int pinbuzzer = 10;
int song[] = {261, 349, 392, 440, 392, 330, -10, 261, 349, 392, 440, 392,
              -10, -10, 261, 349, 392, 440, 392, 330, -10, 330, 349, 330, 261, 261
             };

void setup() {
  Serial.begin(9600);
  pinMode(pinbuzzer, OUTPUT);
}

void loop() {
  for (int i = 0; i < sizeof(song) / sizeof(song[0]); i++) {
    tone(pinbuzzer, song[i]);
    delay(500);
  }

  noTone(pinbuzzer);
  delay(5000);
}
