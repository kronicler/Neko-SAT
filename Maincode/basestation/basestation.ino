// The one with the LCD screen
#include <XBee.h>
#include <Wire.h>
#include <LCDi2cR.h>


LCDi2cR lcd = LCDi2cR(4, 20, 0x63, 0);
uint8_t rows = 4;
uint8_t cols = 20;

#define TMP102 0x48

const unsigned char OSS = 0;  // Oversampling Setting

int count = 5000;
float temp = 0.0;

int heading_camera = '0'; 

String receiver;

XBee xbee = XBee();

// create reusable response objects for responses we expect to handle
XBeeResponse response   = XBeeResponse();
ZBRxResponse        rx  = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


// Sending end

uint8_t payload[51] = "\0";

XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40A9DB24);
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

void xbee_respond () {
    xbee.readPacket();
    if (xbee.getResponse().isAvailable())
    {
        //Serial.println("Received");
        
        // got something
        //if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
        //{
            xbee.getResponse().getZBRxResponse(rx);
            
            // get data
            lcd.clear();
            uint8_t* rxData = rx.getData();
            
            receiver = String((char *)rxData);
            
            lcd.print(receiver);
            
            // Send only when it gets the response
            delay(500);
            xbee.send(zbTx);
            //if (heading_camera != '0') heading_camera = '0';
        //}
    }
}


void setup()
{
    // start xbee in serial 2
    Serial2.begin(9600);
    Serial.begin(9600);
    //Serial1.begin(9600);
    xbee.setSerial(Serial2);
    Wire.begin();

    delay (1000);
    temp = getTemp102();
    // Initiate the first handshake
    String(temp).toCharArray(payload, 50);
    
    xbee.send(zbTx);
    
    lcd.init(); 
    lcd.print (String(temp));
    lcd.setCursor(1,0);
    lcd.print("Initiated\n");
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop()
{
    // Sender adaptation
    //heading = Serial1.read();
    if (Serial.available() > 0) {
      // Use heading_camera as a means of pinging too. 0 means nothing 
      heading_camera = Serial.read();
      Serial.println (heading_camera);
      Serial.println (receiver); // Only serial print on request 
      // 0 by default - Also used for pinging 
    }
    
    // Update temperature 
    if (count >= 5000) {
        float temp_temperature = getTemp102();
        if (temp_temperature < 50 && temp_temperature > 5) {
            temp = temp_temperature;
            count = 0;
        } 
    }
    String (heading_camera).toCharArray(payload, 29);
    count++;
    xbee_respond();
}

