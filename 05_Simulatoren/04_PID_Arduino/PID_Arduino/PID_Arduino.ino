#include "Servo.h"

Servo S;

float sollwert = 90;
float p,i,d,pp = 4,ii = 0.01,dd = 0.08;
float error = 0, preverror = 0;
float delaytime = 50;

String data = "";

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  S.attach(9);
}

void loop() {
  if(Serial.available() > 0){
    char inChar = (char)Serial.read();
    data += inChar;
    if(inChar == '\n'){
      pp = getValue(data,' ',0).toFloat();
      ii = getValue(data,' ',1).toFloat();
      dd = getValue(data,' ',2).toFloat();
      sollwert = getValue(data,' ',3).toFloat();
      data = "";
    }
    
  }
  int currentpos = map(analogRead(A0),813,10,0,180);
  S.attach(9);
  error = sollwert - currentpos;
  Serial.println(currentpos);
  p = error * pp;//4
  i += error * ii * (delaytime/1000); //0.01
  d = dd * ((error - preverror)/(delaytime/1000));//0.08

  float pid = p + i + d;
  preverror = error;

  S.write(currentpos + pid);
  delay(delaytime/2);

  S.detach();
  delay(delaytime/2);
}

 String getValue(String data, char separator, int index) //https://stackoverflow.com/questions/9072320/split-string-into-string-array to busy to wirite own code 
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
