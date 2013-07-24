// Serial Gun testbed
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX


byte inputGun1 = 0;
int gun1Fired = 0;  

void setup() {
  Serial.begin(19200);
  Serial.println("Starting...");
  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available()) {
      inputGun1 = mySerial.read();
    if ( inputGun1 < 20 ) { 
      gun1Fired = 1;
      mySerial.print("ACK");
    } else {
      gun1Fired = 0;
    }
  while (mySerial.read()) {} 
  delay(10);
  }
  if (gun1Fired && (inputGun1 > 0) ) {
    Serial.print(F("Gun 1 HIT! "));
    Serial.print((int) inputGun1);
    Serial.println("");
    inputGun1 = 0;
    gun1Fired = 0;
  } else if (gun1Fired && (inputGun1 == 0)) {
    Serial.println(F("Gun 1 MISS! "));
    inputGun1 = 0;
    gun1Fired = 0;
  }
}


