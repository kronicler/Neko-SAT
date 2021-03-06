// TODO: Make the receiver send data back from a sensor when it receives the right string.

#include <XBee.h>
#include <Wire.h>

#define TMP102 0x48
#define HMC_address 0x1E //0011110b, I2C 7bit address of HMC5883

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

const unsigned char OSS = 0;  // Oversampling Setting

// Calibration values - for Barometer
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

short temperature;
long pressure;

// Use these for altitude conversions
const float p0 = 101325;     // Pressure at sea level (Pa)
float altitude;


XBee xbee = XBee();

// create reusable response objects for responses we expect to handle
XBeeResponse response   = XBeeResponse();
ZBRxResponse        rx  = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


// Sending end

uint8_t payload[30] = "\0";
String myString = "nothing";
float temp = 0.0;

XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4098DA02);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();


float getTemp102 () {
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
    }
    //Serial.println (convertedTemp);
    return convertedTemp;
}


void getHMC (int *x, int *y, int *z) {
    
    //Tell the HMC5883L where to begin reading data
    Wire.beginTransmission(HMC_address);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
    
    //Read data from each axis, 2 registers per axis
    Wire.requestFrom(HMC_address, 6);
    if(6<=Wire.available()){
        *x = Wire.read()<<8; //X msb
        *x |= Wire.read(); //X lsb
        *z = Wire.read()<<8; //Z msb
        *z |= Wire.read(); //Z lsb
        *y = Wire.read()<<8; //Y msb
        *y |= Wire.read(); //Y lsb
    }
}

char bmp085Read(unsigned char address)
{
    unsigned char data;
    
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    
    Wire.requestFrom(BMP085_ADDRESS, 1);
    while(!Wire.available())
        ;
    
    return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
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


unsigned int bmp085ReadUT()
{
    unsigned int ut;
    
    // Write 0x2E into Register 0xF4
    // This requests a temperature reading
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(0xF4);
    Wire.write(0x2E);
    Wire.endTransmission();
    
    // Wait at least 4.5ms
    delay(5);
    
    // Read two bytes from registers 0xF6 and 0xF7
    ut = bmp085ReadInt(0xF6);
    return ut;
}

// Read the uncompensated pressure value
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


// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short bmp085GetTemperature(unsigned int ut)
{
    long x1, x2;
    
    x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
    x2 = ((long)mc << 11)/(x1 + md);
    b5 = x1 + x2;
    
    return ((b5 + 8)>>4);
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
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




void setup()
{
    // start xbee in serial 2
    Serial2.begin(9600);
    xbee.setSerial(Serial2);
    Serial.begin(9600);
    Wire.begin();
    
    temp = getTemp102();
    
    //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(HMC_address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();
    
    
    bmp085Calibration();

}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop()
{
    xbee.readPacket();
    //flashLed(dataLed, 1, 10);
    if (xbee.getResponse().isAvailable())
    {
        // Serial.println("Received");
        
        // got something
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
        {
            xbee.getResponse().getZBRxResponse(rx);
            
            // get data
            uint8_t* rxData = rx.getData();
            
            Serial.print (String((char *)rxData)); // The data already includes a new line
            String receiver = String((char *)rxData);
            
            if (receiver == "temp\n") {
                // Send back temperature data here.
                float temporary = getTemp102();
                if (temporary > 0) {
                    temp = temporary;
                }
                myString = String(temp);
                
                Serial.println ("Message replied!");
                
            }
            else if (receiver == "humidity\n") {
                // Fill in humidity response here.
                // Send back temperature data here.
                
                myString = "hello";
                
                Serial.println ("Message replied!_humidity");
                
            }
            else if (receiver == "hmc\n") {
                int x, y, z;
                getHMC(&x, &y, &z);
                myString = String(x);
                myString += " ";
                myString += String(y);
                myString += " ";
                myString += String (z);
                
                Serial.println ("Message replied!_HMC");
                
            }
            else if (receiver == "baro\n") {
                temperature = bmp085GetTemperature(bmp085ReadUT());
                pressure = bmp085GetPressure(bmp085ReadUP());
                
                myString = String(pressure);
                myString += " ";
                myString += String (temperature*0.1);
                Serial.println ("Message replied!_baro");

            }
            myString.toCharArray(payload, 29);
            xbee.send(zbTx);
        }
    }
    
}


