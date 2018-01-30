#include <Wire.h>
#define TMP102 0x48


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void getTemp102 () {
  // put your main code here, to run repeatedly:
  byte firstByte, secondByte;
  int value;
  float convertedTemp;
  Wire.beginTransmission(TMP102);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom (TMP102, 2);

  
  if ( Wire.available() >= 2 ) {
    firstByte = (Wire.read()); 
    secondByte = (Wire.read());
    value = ((firstByte) << 4); // MSB
    value |= (secondByte >> 4); // LSB
    convertedTemp = value * 0.0625;
    Serial.println (convertedTemp);
  }


}

void loop() {
  getTemp102();
  delay(5000);
}
