#define VERSION "1.1"
#include <Wire.h>
// #include <inttypes.h>


#define TMP102 0x48
#define HMCaddress 0x1E //0011110b, I2C 7bit address of HMC5883
#define BMP085_ADDRESS 0x77  // I2C address of BMP085

const unsigned char OSS = 0;  // Oversampling Setting


// robot-electronics.co.uk 
#include <LCDi2cR.h>                      
LCDi2cR lcd = LCDi2cR(4,20,0x63,0);

uint8_t rows = 4;
uint8_t cols = 20;

int x, y, z;
int keyInput = 0;
double temperature;
int count = 0;


// Calibration values
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;
long b5; 


int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) msb<<8 | lsb;
}

unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}

long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}



void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

void getHMC () {
  Wire.beginTransmission(HMCaddress);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();

 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMCaddress, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
}

double getTemp102()
{
  // bytes used to store value from the TMP102 temperature registers
  byte firstByte, secondByte; 

  // an int is capable of storing two bytes
  int value; 
  float convertedTemp; 

  Wire.beginTransmission(TMP102); 
  Wire.write((byte)0x00);  // point to temperature register
  Wire.endTransmission();
  
  Wire.requestFrom(TMP102, 2);
  if( Wire.available() >= 2 )
  {
    // read the TMP102 temperature register
    firstByte  = (Wire.read());
    secondByte = (Wire.read());
  }
  
  /* The first byte contains the most significant bits,
  and the second the less significant */
  
  /* MSB */
  value = ((firstByte) << 4);
  /* LSB */
  value |= (secondByte >> 4);
  
  /* LSB is ORed into the second 4 bits of our byte.
  Bitwise maths is a bit funky, but there's a good
  tutorial on the playground*/
  convertedTemp = value * 0.0625; // need a proper number 
  return convertedTemp;
}

void setup() 
{ 
  lcd.init();                 // Init the display, clears the display
  lcd.print("Hello Chin Chin!");  
  Wire.begin(); // start the I2C library
  Serial.begin(9600); //Start serial comm at 9600 baud

  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(HMCaddress); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
  temperature = getTemp102(); // Get temp only when asked for

  //Calibrate BMP
  bmp085Calibration();
}

void loop()
{
  //statusNum = lcd.status();
  keyInput = lcd.keypad();
  lcd.clear();
  //lcd.setCursor(1,0); // Acts as a print new line on the LCD matrix 
  long pressure;
  count++; 
  if (count > 2000) {
    temperature = getTemp102(); // refresh temperature
    count = 0;
  }
  switch (keyInput) {
    case 50:
    lcd.print(temperature, DEC);
    break;
    case 53: 
    getHMC(); // Get HMC only when asked for
    lcd.print (x,DEC);
    lcd.setCursor(1,0);
    lcd.print (y, DEC);
    lcd.setCursor(2,0);
    lcd.print (z, DEC);
    break;
    case 56:
    pressure = bmp085GetPressure(bmp085ReadUP());
    lcd.print (pressure,DEC);
    // TODO: Implement the humidity sensor and xbee. 
    default:
    lcd.print(keyInput, DEC);
  }
  delay (100);
}


