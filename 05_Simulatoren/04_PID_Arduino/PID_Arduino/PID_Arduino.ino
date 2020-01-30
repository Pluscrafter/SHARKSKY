#include "Servo.h"

Servo S;

float sollwert = 90;
float p,i,d,pp = 4,ii = 5,dd = 0.1;
float error = 0, preverror = 0;
float delaytime = 50;

String data = "";

void setup() {
  Serial.begin(115200);
  pinMode(9, OUTPUT);
  S.attach(9);
}

void loop() {
  if(Serial.available() > 0){
    char inChar = (char)Serial.read();
    data += inChar;
    if(inChar == '\n'){
      String str(data);
      char str_array[str.length()];
      str.toCharArray(str_array, str.length());
      char* token = strtok(str_array, " ");
      pp = atof(token);
      token = strtok(NULL, " ");
      ii = atof(token);
      token = strtok(NULL, " ");
      dd = atof(token);
      token = strtok(NULL, " ");
      sollwert = atof(token);
      data = "";
    }
  }
  int currentpos = map(analogRead(A2),813,10,0,180);
  S.attach(9);
  error = sollwert - currentpos;
  
  p = error * pp;//4
  i += error * ii * ((delaytime)/1000); //0.01
  d = dd * ((error - preverror)/((delaytime)/1000));//0.08

  float pid = p + i + d;
  preverror = error;
  Serial.println(currentpos);
  S.write(currentpos + pid);
  delay(delaytime/2);

  S.detach();
  delay(delaytime/2);
}
