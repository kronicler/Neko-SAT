// The one with the LCD screen
#include <XBee.h>
#include <Wire.h>
#include <LCDi2cR.h>


LCDi2cR lcd = LCDi2cR(4, 20, 0x63, 0); 
uint8_t rows = 4;
uint8_t cols = 20;



#define TMP102 0x48
#define HMC_address 0x1E //0011110b, I2C 7bit address of HMC5883

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

const unsigned char OSS = 0;  // Oversampling Setting

String seString = "";

float temp = 0.0;

XBee xbee = XBee();

// create reusable response objects for responses we expect to handle
XBeeResponse response   = XBeeResponse();
ZBRxResponse        rx  = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


// Sending end

uint8_t payload[30] = "\0";
String myString = "nothing";



XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4098DA08);
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


void setup()
{
    // start xbee in serial 2
    Serial2.begin(9600);
    Serial.begin(9600);
    xbee.setSerial(Serial2);
    Wire.begin();
    
    temp = getTemp102();

    //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(HMC_address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();
       
    lcd.init(); // Init & clear display lcd.print("Hello World!"); delay(1000);

}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop()
{
    // Sender adaptation
    seString = Serial.readString();
    if (seString.length() > 0) {
        seString.toCharArray(payload, 29);
        Serial.println ("Message updated!");
        Serial.println ("Message sent!");
        xbee.send(zbTx);
    }
    
    
    
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
            lcd.clear();
            uint8_t* rxData = rx.getData();
            String receiver = String((char *)rxData);
            Serial.print (receiver); // The data already includes a new line
            lcd.print(receiver);

            
            if (receiver == "temp\n") {
                // Send back temperature data here.
                float temporary = getTemp102();
                if (temporary > 0) {
                    temp = temporary;
                }
                myString = String(temp);
                
                Serial.println ("Message replied!");
                
            }
            myString.toCharArray(payload, 29);
            xbee.send(zbTx);
        }
    }
}

