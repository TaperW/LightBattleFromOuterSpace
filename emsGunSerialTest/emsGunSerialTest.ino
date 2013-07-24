// Serial Gun testbed

int inputGun1 = 0;
int inputGun2 = 0;
int gun1Fired = 0;
int gun2Fired = 0;

void setup() {
  Serial.begin(19200);
  Serial.println("Starting...");
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  if (Serial1.available()) {
      inputGun1 = Serial1.read();
  if ( inputGun1 < 20 ) { 
    gun1Fired = 1;
    Serial1.print("ACK");
  } else {
    gun1Fired = 0;
  }
//  while (Serial1.read()) {} 
  Serial1.end();
  delay(10);
  if ((gun1Fired == 1) && (inputGun1 > 0) ) {
    Serial.print(F("Gun 1 HIT! "));
    Serial.print((int) inputGun1);
    Serial.println("");
    // Do stuff with inputGun1
    // score++
    inputGun1 = 0;
    gun1Fired = 0;
  } else if ((gun1Fired == 1 ) && (inputGun1 == 0)) {
    Serial.println(F("Gun 1 MISS! "));
    inputGun1 = 0;
    gun1Fired = 0;
  }
  }
  if (Serial2.available()) {
      inputGun2 = Serial2.read();
    if ( inputGun2 < 20 ) { 
      gun2Fired = 1;
      Serial2.print("ACK");
    } else {
      gun2Fired = 0;
    }
    Serial2.end();
//    while (Serial2.read()) {} 
    delay(10);
    if ((gun2Fired == 1) && (inputGun2 > 0) ) {
      Serial.print(F("Gun 2 HIT! "));
      Serial.print((int) inputGun2);
      Serial.println("");
      // Do stuff
      inputGun2 = 0;
      gun2Fired = 0;
    } else if ((gun2Fired == 1) && (inputGun2 == 0)) {
      Serial.println(F("Gun 2 MISS! "));
      inputGun2 = 0;
      gun2Fired = 0;
    }

    
  }
  Serial1.begin(9600);
  Serial2.begin(9600);
}

//void serialEvent1() {
//}
//
//void serialEvent2() {
//}

