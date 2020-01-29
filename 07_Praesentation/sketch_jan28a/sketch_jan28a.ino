#include "Servo.h"
Servo S;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  S.attach(9);

}

void loop() {
  Serial.println(analogRead(A0));
  S.write(180);
  delay(100);

}
