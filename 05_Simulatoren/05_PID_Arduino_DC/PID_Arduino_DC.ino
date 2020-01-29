
float soll = 500;
float error, preverror;
float p = 0,i = 0,d = 0;
float delaytime = 1;

float throttle = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  
}

void loop() {
  int pos = analogRead(A0);
  error = soll - pos;

  p = error * 10;
  i += 0 * error * (delaytime/1000);
  d = 0 * ((error - preverror)/(delaytime/1000));

  float pid = p+i+d;

  throttle += pid;
  
  if(throttle < 0){ //-
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
  }else{
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
  }

  if(throttle > 256){
    throttle = 255;  
  }

  if(throttle < -256){
    throttle = -255;  
  }

  analogWrite(11, abs(throttle));

  Serial.println(pos);
  
  delay(delaytime);
  
}
