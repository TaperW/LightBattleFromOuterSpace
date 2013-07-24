// emsRoboSerialTest

int i = 0;
int j = 0;
int scoreR = 0; 
int scoreG = 0;

void setup() {
  randomSeed(analogRead(A0));
  Serial.begin(19200);
  Serial.println("Starting...");
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
    delay(100);

  Serial3.print("BOOT\n");
  Serial.println("Sent BOOT");    delay(100);

}

void loop() {
  if (i < 2) {
    Serial3.print("TICKLE\n");
    Serial.println("Sent TICKLE");
    i++;
    delay(30000);
  } else if (i==2) {
    Serial3.print("NEWGAME\n");
    Serial.println("sent NEWGAME");
    delay(1000);
    i++;
  } else if (scoreR < 5 && scoreG < 5) {
    int r = random(10);
    if (r < 3) { 
      Serial3.print("REDMISS\n");
      Serial.println("sent REDMISS");
    } else if (r >= 3 && r < 6 ) {
      Serial3.print("GREENMISS\n");
      Serial.println("sent GREENMISS");
    } else if (r == 6 || r == 7) {
      scoreR++;
      Serial3.print("RED S");
      Serial3.print(scoreR);
      Serial3.print(",");
      Serial3.print(scoreG);
      Serial3.print("\n");
      Serial.print("sent RED S");
      Serial.print(scoreR);
      Serial.print(",");
      Serial.println(scoreG);
    } else if (r == 8 || r == 9) {
      scoreG++;
      Serial3.print("GREEN S");
      Serial3.print(scoreR);
      Serial3.print(",");
      Serial3.print(scoreG);
      Serial3.print("\n");
      Serial.print("sent GREEN S");
      Serial.print(scoreR);
      Serial.print(",");
      Serial.println(scoreG);
    }      
    delay(10000);
  } else {
    Serial3.print("GAMEOVER S");
    Serial3.print(scoreR);
    Serial3.print(",");
    Serial3.print(scoreG);
    Serial3.print("\n");
    Serial.print("sent GAMEOVER S");
    Serial.print(scoreR);
    Serial.print(",");
    Serial.println(scoreG);
    i = 0;scoreG= 0;scoreR= 0;
    delay(10000);
  }
}
