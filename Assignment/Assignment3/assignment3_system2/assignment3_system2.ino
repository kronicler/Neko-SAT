#include <XBee.h>
#include <Wire.h>
#include <math.h>
#define PI 3.141592653

#include <SD.h>
#define FILE "datalog.csv"
#define TMP102 0x48
#define HMC_address 0x1E //0011110b, I2C 7bit address of HMC5883

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

const unsigned char OSS = 0;  // Oversampling Setting

//Chip Select Pin on Arduino
const int chipSelect = 53;

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

// global variables for reading sensor values
int x, y, z;
String baroTempS,baroPresS,HMCstring,humidityS;
int fileOpened=0,SdRead=0;
unsigned long int lastWrite = 0;
File dataFile;

// Use these for altitude conversions
const float p0 = 101325;     // Pressure at sea level (Pa)
float altitude;

String curr_key;

XBee xbee = XBee();

// create reusable response objects for responses we expect to handle
XBeeResponse response   = XBeeResponse();
ZBRxResponse        rx  = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


// Sending end

uint8_t payload[30] = "\0";

XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4098DA02);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

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


float getHumidity () {
    return (float)analogRead(A0)*100 /1024;
}

String writeSD(String humidityS,String baroTempS,String baroPresS,String angle){
    File dataFile = SD.open(FILE, FILE_WRITE);

    dataFile.seek(lastWrite);
    //Serial.println("Writing...");
    // File available, write the bytes into it
    dataFile.print(humidityS);
    dataFile.print(",");
    dataFile.print(baroTempS);
    dataFile.print(",");
    dataFile.print(baroPresS);
    dataFile.print(",");
    dataFile.print(angle);
    dataFile.print("\n");

    lastWrite = dataFile.position();
    // Close file
    //Serial.println("Closing...");
    dataFile.close();
    //Serial.println("Closed");
    return "";
}

void xbee_respond () {
    xbee.readPacket();
    //flashLed(dataLed, 1, 10);
    if (xbee.getResponse().isAvailable())
    {
        //Serial.println("Received");
        
        // got something
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
        {
            xbee.getResponse().getZBRxResponse(rx);
            
            // get data
            uint8_t* rxData = rx.getData();
            String receiver = String((char *)rxData);
      
            Serial.println (receiver); 
            String myString = "hello"; 
            
            if (receiver == "sd\n" || receiver == "stopsd\n"){
              SdRead = 1 - SdRead;
              Serial.println("SD mode activated");
            }
            Serial.print("Check SD status: ");
            Serial.println(SdRead);
            if (!SdRead){
                // if the file is open for reading, close it and prepare to write into it.
                if (fileOpened){
                    dataFile.close();
                    fileOpened = 0;
                    Serial.println("SD Closed for writing");
                }
                
                //Serial.println("SD status check complete");
                // setup all data from sensors
                humidityS = String(getHumidity());
                temperature = bmp085GetTemperature(bmp085ReadUT())*0.1;
                baroTempS = String(temperature);
                pressure = bmp085GetPressure(bmp085ReadUP());
                baroPresS = String(pressure);
                getHMC(&x,&y,&z);
                String angle = String((atan2(double(y), double(x))/PI)*180 + 180);

                //Serial.println("Writing to SD");
                // Write data to SD card
                writeSD(humidityS,baroTempS,baroPresS,angle);
                
                // Set the curr_key only if receiver received correctly
                if (receiver == "humidity\n")  curr_key = "humidity";
                else if (receiver == "hmc\n") curr_key = "hmc";
                else if (receiver == "baro\n") curr_key = "baro";
        
                //Serial.println("Done Writing to SD");
                if (curr_key == "humidity") {
                    // Fill in humidity response here.
                    // Send back temperature data here.
                    
                    myString = humidityS;
                    Serial.println ("Message replied!_humidity");
                }
                else if (curr_key == "hmc") {
                    myString = angle;
                    myString += " degrees";
                    Serial.println ("Message replied!_HMC");
                }
                else if (curr_key == "baro") {
                  myString = baroPresS;
                  myString += " ";
                  myString += baroTempS;
                  Serial.println ("Message replied!_baro");
                }
            }
            else{
              //Get data from file. Open the file if not opened yet.
              Serial.println("Enabled SD read:");
              if (!fileOpened){
                Serial.println("Opening File for reading.");
                dataFile = SD.open(FILE, FILE_READ);
                //dataFile.seek(0);
                fileOpened = 1;
              }
              
              Serial.println("READING...");

              byte readByte = 0;
              String completeData = "";
              String humidityS = "";
              String baroTempS = "";
              String baroPresS = "";
              String angleS = "";
              int endline=0,i=0;
              Serial.println("File Data: ");
              
              // File available, read file until no data available
              while(dataFile.available() && !endline) 
              {
                readByte = dataFile.read();
                if ((char)readByte == ','){
                    switch(i){
                        case 0:
                        humidityS = completeData;
                        i++;
                        break;
                        case 1:
                        baroTempS = completeData;
                        i++;
                        break;
                        case 2:
                        baroPresS = completeData;
                        i++;
                        break;
                    }
                    completeData = "";
                    continue;
                }
                if ((char)readByte == '\n'){
                    angleS = completeData;
                    completeData = "";
                    endline = 1;
                }
                else
                    Serial.print((char)readByte);
                            completeData += (char)readByte;
                          }
              Serial.println("");
              // Print file data to screen
              Serial.println(humidityS);
              Serial.println(baroTempS);
              Serial.println(baroPresS);
              Serial.println(angleS);
              
              Serial.println("Done Processing!");
              
              // Set the curr_key only if receiver received correctly
              if (receiver == "humidity\n")  curr_key = "humidity";
              else if (receiver == "hmc\n") curr_key = "hmc";
              else if (receiver == "baro\n") curr_key = "baro";
      
              myString = "SD: ";
              
              if (curr_key == "humidity") {
                // Fill in humidity response here.
                // Send back temperature data here.
                
                myString += humidityS;
                Serial.println ("Message replied!SD_humidity");
                
              }
              else if (curr_key == "hmc") {
                myString += angleS;
                myString += " degrees";
                Serial.println ("Message replied!SD_HMC");
                
              }
              else if (curr_key == "baro") {
                myString += baroPresS;
                myString += " ";
                myString += baroTempS;
                Serial.println ("Message replied!SD_baro");
      
              }
            }

            // Send the payload out
            delay(500);
            myString.toCharArray(payload, 29);
            xbee.send(zbTx);
        }
    }
}

void setup()
{
    // start xbee in serial 
    Serial2.begin(9600);
    Serial.begin(9600);
    xbee.setSerial(Serial2);
    Wire.begin();
    
    //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(HMC_address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();
    
    // Initialise Card
    Serial.print("Initializing SD card...");
    pinMode(chipSelect, OUTPUT);
    if (!SD.begin(chipSelect)) 
    {
      Serial.println("Init failed, or not present");
      // Initialisation failed, do not continue
      return;
    }
    
    // Initialisation Complete
    Serial.println("Card initialised.");
    
    // Check if file already exist
    if (SD.exists(FILE)) 
    {
      // Delete existing file
      Serial.print(FILE);
      Serial.println(" exists, deleting");
      SD.remove(FILE);
      Serial.println();
    }
    else 
    {
      Serial.print(FILE);
      Serial.println(" not found!");
    }
    bmp085Calibration();
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop()
{
    xbee_respond();
}

