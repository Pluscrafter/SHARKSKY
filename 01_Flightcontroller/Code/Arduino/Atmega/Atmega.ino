#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_CS   (10)

Adafruit_BMP280 bmp(BMP_CS);

int16_t iAlt;
uint8_t hAlt, lAlt;
int c = 0;
void setup() {
  Serial.begin(9600);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.println(F("BMP280 test"));

  delay(2000);
  
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  iAlt = (int)bmp.readAltitude(1019.50);
  lAlt = iAlt & 0xFF;
  hAlt = iAlt >> 8;

  Serial.print(hAlt);
  Serial.print(" ");
  Serial.print(lAlt);
  Serial.print(" ");
  Serial.println(iAlt);
  Serial.println(c);

  delay(100);
  
}

void receiveEvent(int bytes){
   c = 1;
}

void requestEvent(){
  Wire.write(highByte(iAlt));
  Wire.write(lowByte(iAlt));
}
