//emsShootyRunServos

#include <Servo.h>

Servo myservo;

int currentpin;
int first=23;
int last = 41;

void setup() 
{ 
  currentpin = first;
} 

void loop() {
  myservo.attach(currentpin);
  delay(20);
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  myservo.detach();
  currentpin += 2;
  if (currentpin > last) {
     currentpin = first;
  }
  delay(5000);
} 

