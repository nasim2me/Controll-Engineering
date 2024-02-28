int buzzer = 10;
void setup() {
  
}

void loop() {
tone(buzzer, 500);
delay(500);
noTone(buzzer);
delay(700);

}
