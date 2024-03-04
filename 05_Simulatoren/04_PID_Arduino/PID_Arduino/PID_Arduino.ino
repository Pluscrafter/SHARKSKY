#include "Servo.h"

Servo S;

float sollwert = 90;
float p,i,d;
float error = 0, preverror = 0;
float delaytime = 50;
void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  S.attach(9);
}

void loop() {
  int currentpos = map(analogRead(A2),813,10,0,180);
  S.attach(9);
  error = sollwert - currentpos;
  Serial.println(currentpos);
  p = error * 4;//5
  i += error * 0.01 * (delaytime/1000); //0.03
  d = 0.08 * ((error - preverror)/(delaytime/1000));

  float pid = p + i + d;
  preverror = error;

  S.write(currentpos + pid);
  delay(delaytime/2);

  S.detach();
  delay(delaytime/2);
}
