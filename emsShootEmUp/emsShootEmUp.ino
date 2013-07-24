// emsShootEmUp
#include <Encabulator.h>
#include <Wire.h>
// #include <I2C.h>   // NOTE: For reasons buried in the Encabulator.cpp file, you might want to download and use this library instead. 
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

// Servos - invert every hit

float tempX = 0.0;
float tempY = 0.0;
float tempZ = 0.0;

float lastX = 0.0;
float lastY = 0.0;
float lastZ = 0.0;


int playerRed = 0;
int playerGreen = 1;

int playerGreenScore = 0;
int playerRedScore = 0;

int playerRedBell = 53;
int playerGreenBell = 51;

int inputGunRed = 0;
int inputGunGreen = 0;
int gunRedFired = 0;
int gunGreenFired = 0;

int numTargets = 10;

int displayMode = 0; // 0 -> in motion, 1 -> attract, 3 -> in-game ?

Servo target[11];

uint32_t colorRed;
uint32_t colorGreen;

uint32_t colorIndex[2];

int targetPin[] = {
  0, 
  23, 25, 27, 29, 
  31, 33, 35, 37, 
  39, 41
}; 

int targetOwner[] = {     // Check #9
  5, playerGreen, playerRed, playerRed, playerGreen, 
  playerRed, playerGreen, playerRed, playerRed, 
  playerGreen, playerRed
};

int targetPos[] = {
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 
  0, 0
};

int targetPosInit[] = {
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 
  0, 0
};


int targetIndex;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(11, 49, NEO_RGB + NEO_KHZ800);

int rpl = 0;

unsigned long savedTime; 

void setup() {
  Serial.begin(19200);
  Encabulator.upUpDownDownLeftRightLeftRightBA();

  for (int i = 1 ; i < 5 ; i++) {  
    Encabulator.stripBankA.jumpHeaderToRGB(i, 255, 255, 255); 
    Encabulator.stripBankB.jumpHeaderToRGB(5-i, 255, 255, 255);
    delay(250);
  }

  //  Serial.println(F("Would you like to play a game?"));
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  colorRed = strip.Color(255, 0, 0);
  colorGreen = strip.Color(0, 255, 0);
  colorIndex[playerRed] = colorRed;
  colorIndex[playerGreen] = colorGreen;
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial3.println("BOOT");
  for (int i = 1 ; i <= numTargets ; i++) {
    target[i].attach(targetPin[i]);
    target[i].write(targetPos[i]);
    strip.setPixelColor(i-1, colorIndex[targetOwner[i]]);
  }
  strip.show();

  pinMode(playerRedBell, OUTPUT);
  pinMode(playerGreenBell, OUTPUT);

  //newGame();
  displayMode = 1;  // put in attract mode
  savedTime = millis();
  randomSeed(analogRead(A0));
  delay(500);
  for (int i = 1 ; i < 5 ; i++) {  
    Encabulator.stripBankA.fadeHeaderToRGB(i, 0, 0, 0, 20); 
    Encabulator.stripBankB.fadeHeaderToRGB(5-i, 0, 0, 0, 20);
    delay(250);
  }

  delay(500);
}

void loop() {
  // in-motion handler  - 
  //  tempX = Encabulator.accelerometer.x() / 1024.0;
  //  tempY = Encabulator.accelerometer.y() / 1024.0;
  //  tempZ = Encabulator.accelerometer.x() / 1024.0;
  //
  //  if ()

  // Checking for 
  if (displayMode == 3) {
//    for (int i = 0 ; i < numTargets ; i++) {
//      strip.setPixelColor(i-1, colorIndex[targetOwner[i]]);
//    }
//    strip.show();

    // listen on Serial1 (== gun 1/Red)
    // Reading for gunRed
    if (Serial1.available()) {
      inputGunRed = Serial1.read();
      if ( inputGunRed < 20 ) { 
        gunRedFired = 1;
        Serial1.print("ACK");
        delay(10);
        Serial1.print("ACK");
      } 
      else {
        gunRedFired = 0;
      }
      //  while (Serial1.read()) {} 
      Serial1.end();
      //  delay(10);
      if ((gunRedFired == 1) && (inputGunRed > 0) ) {
        Serial.print(F("Gun Red HIT! "));
        Serial.print((int) inputGunRed);
        Serial.println("");
        if (playerRed == targetOwner[inputGunRed] ^ (targetPos[inputGunRed] / 180)) {
          scoreRed(inputGunRed);
        } 
        else {
          misfireRed(inputGunRed);
        }
        // score++
        inputGunRed = 0;
        gunRedFired = 0;
      } 
      else if ((gunRedFired == 1 ) && (inputGunRed == 0)) {
        Serial.println(F("Gun Red MISS! "));
        inputGunRed = 0;
        gunRedFired = 0;
      }
        Serial1.begin(9600);
    }
    // listen on Serial2 (== gun 2/Green)
    if (Serial2.available()) {
      inputGunGreen = Serial2.read();
      if ( inputGunGreen < 20 ) { 
        gunGreenFired = 1;
        Serial2.print("ACK");
        delay(10);
        Serial2.print("ACK");
      } 
      else {
        gunGreenFired = 0;
      }
      Serial2.end();
      //    while (Serial2.read()) {} 
      delay(10);
      if ((gunGreenFired == 1) && (inputGunGreen > 0) ) {
        Serial.print(F("Gun Green HIT! "));
        Serial.print((int) inputGunGreen);
        Serial.println("");
        if (playerGreen == targetOwner[inputGunGreen] ^ (targetPos[inputGunGreen] / 180)) {
          scoreGreen(inputGunGreen);
        } 
        else {
          misfireGreen(inputGunGreen);
        }
        // Do stuff
        inputGunGreen = 0;
        gunGreenFired = 0;
      } 
      else if ((gunGreenFired == 1) && (inputGunGreen == 0)) {
        Serial.println(F("Gun Green MISS! "));
        inputGunGreen = 0;
        gunGreenFired = 0;
      }
      Serial2.begin(9600);
    }
    if ((millis() - savedTime) > 60000) {
      gameOver();
    }
    Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 1, 0, 0, 200); 
    Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 4, 0,0, 200); 

    Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 1, 0,200, 0); 
    Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 4, 0,0, 200); 

  } 
  else {
    if (displayMode == 0) {
      doInMotionLoop();
    } 
    else if (displayMode == 1) {
      doAttractModeLoop();
    }
    if ((Serial1.available() || Serial2.available())) {
      Serial1.print("ACK");
      Serial2.print("ACK");
      Serial1.end();
      Serial2.end();
      delay(100);
      Serial1.begin(9600);
      Serial2.begin(9600);
      Serial.println("Entering game!");
      newGame();
    }
  }
}

void newGame() {
  playerGreenScore = 0;
  playerRedScore = 0;


  for (int i = 1 ; i < 5 ; i++) {  
    Encabulator.stripBankA.jumpHeaderToRGB(i, 255, 255, 255); 
    Encabulator.stripBankB.jumpHeaderToRGB(5-i, 255, 255, 255);
    delay(200);
  }
  delay(500);

  // quickly fade all 12V RGB headers down to black

  for (int i = 1 ; i < 5 ; i++) {  
    Encabulator.stripBankA.fadeHeaderToRGB(i, 0, 0, 0, 20); 
    Encabulator.stripBankB.fadeHeaderToRGB(5-i, 0, 0, 0, 20);
    delay(500);
  }
  startGame();
  displayMode = 3;
}


void scoreRed(int index) {
  playerRedScore++;
  Serial3.print("RED S");
  print3Score();
  celebrateRed(index);
  // flip target
  // redscore++
}

void misfireRed(int index) {
  playerGreenScore++;
  Serial3.print("REDOOPS S");
  print3Score();
  celebrateGreen(index);
}

void celebrateRed(int index) {
//  Serial.println(index);
  // bell
  ringBell(playerRedBell);
  // flip
  if (targetPos[index] == 0) {
    targetPos[index] = 180;
  } 
  else {
    targetPos[index] = 0;
  }
  target[index].write(targetPos[index]);
  // lights
  strip.setPixelColor(index-1, colorRed);
  strip.show();
}

void celebrateGreen(int index) {
  ringBell(playerGreenBell);
  // flip
  if (targetPos[index] == 0) {
    targetPos[index] = 180;
  } 
  else {
    targetPos[index] = 0;
  }
  target[index].write(targetPos[index]);
  // lights
  strip.setPixelColor(index-1, colorGreen);
  strip.show();
}


void scoreGreen(int index) {
   playerGreenScore++;
  Serial3.print("GREEN S");
  print3Score();
  celebrateGreen(index);
  // flip target
  // redscore++
}

void misfireGreen(int index) {
  playerRedScore++;
  Serial3.print("GREENOOPS S");
  print3Score();
  celebrateRed(index);
}


void ringBell(int bell) {
  digitalWrite(bell, HIGH);
  delay(27);
  digitalWrite(bell, LOW);
}

void doAttractModeLoop() {
  // lights
  byte g = random(256);
  byte b = random(256);
  uint16_t i;
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, b, g));
  }
  strip.show();
  delay(20);
  rpl++;
  if (rpl > 255) {
    rpl = 0;
  }
  
  Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 1, 0,g, b); 
  Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 4, 255,255-g, b); 

  Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 1, 0,255-g, 255-b); 
  Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 4, 255,g, 255-b); 

  
  // random flipping?
  // timeout for tickle
  if ((millis() - savedTime) > 60000) {
    savedTime = millis();
    Serial3.print("TICKLE\n");
    Serial.println("TICKLE");
  }
  delay(5000);
}

void doInMotionLoop() {
}

void print3Score() {
  Serial3.print(playerRedScore);
  Serial3.print(",");
  Serial3.print(playerGreenScore);
  Serial3.print("\n");
  Serial.print(playerRedScore);
  Serial.print(",");
  Serial.println(playerGreenScore);
}

void startGame() {
  // lights
  savedTime = millis();
  // initalize stuff
  playerGreenScore = 0;
  playerRedScore = 0;
  for (int i = 1 ; i <= numTargets ; i++) {
    targetPos[i] = targetPosInit[i];
    target[i].write(targetPos[i]);
    strip.setPixelColor(i-1, colorIndex[targetOwner[i]]);
  }
  strip.show();

  
  // tell robot
  
  Serial3.print("NEWGAME\n");
}

void gameOver() {
  Serial3.print("GAMEOVER S");
  Serial.println("GAMEOVER S");  
  print3Score();
  if (playerGreenScore > playerRedScore) {
    Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 1, 0, 255, 0); 
    Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 4, 0,255, 0); 

    Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 1, 0,255, 0); 
    Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 4, 0,255, 0); 
    for (int i = 1 ; i <= numTargets ; i++) {
      strip.setPixelColor(i-1, colorGreen);
    }
    strip.show();
  } else if (playerRedScore > playerGreenScore) {
    Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 1, 255, 0, 0); 
    Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 4, 255,0, 0); 

    Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 1, 255,0, 0); 
    Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 4, 255,0, 0); 
    for (int i = 1 ; i <= numTargets ; i++) {
      strip.setPixelColor(i-1, colorRed);
    }
    strip.show();
  } else {
    Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 1, 0, 255, 0); 
    Encabulator.stripBankA.jumpHeaderToRGB((uint8_t) 4, 255,0, 0); 

    Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 1, 255,0, 0); 
    Encabulator.stripBankB.jumpHeaderToRGB((uint8_t) 4, 0,255, 0); 
    for (int i = 1 ; i <= numTargets ; i++) {
      strip.setPixelColor(i-1, colorIndex[i%2]);
    }
    strip.show();

  }
  delay(10000);
  playerGreenScore = 0;
  playerRedScore = 0;
  savedTime=millis();
  
  displayMode = 1;
  
}


