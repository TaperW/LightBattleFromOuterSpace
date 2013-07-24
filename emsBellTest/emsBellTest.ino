int bell1 = 51;
int bell2 = 53;

void setup() {
  pinMode(51, OUTPUT);
  pinMode(53, OUTPUT);
}

void loop() {
  ringBell(51);
  delay(1000);
  ringBell(53);
  delay(1000);
}

void ringBell(int bell) {
  digitalWrite(bell, HIGH);
  delay(20);
  digitalWrite(bell, LOW);
}

