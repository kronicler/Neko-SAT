// The one with the LCD screen
#include <XBee.h>
#include <Wire.h>
#include <LCDi2cR.h>


LCDi2cR lcd = LCDi2cR(4, 20, 0x63, 0);
uint8_t rows = 4;
uint8_t cols = 20;



#define TMP102 0x48

const unsigned char OSS = 0;  // Oversampling Setting

int count = 0;
float temp = 0.0;
String curr_key;
XBee xbee = XBee();

// create reusable response objects for responses we expect to handle
XBeeResponse response   = XBeeResponse();
ZBRxResponse        rx  = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


// Sending end

uint8_t payload[30] = "\0";

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

void xbee_respond () {
    xbee.readPacket();
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

            // Print only when it gets a response 
            
            lcd.print(temp);
            lcd.setCursor(1,0);
            lcd.print(receiver);

            // Send only when it gets the response
            
            xbee.send(zbTx);
        }
    }
}


void setup()
{
    // start xbee in serial 2
    Serial2.begin(9600);
    Serial.begin(9600);
    xbee.setSerial(Serial2);
    Wire.begin();

    delay (1000);
    temp = getTemp102();
    // Initiate the first handshake
    String(temp).toCharArray(payload, 29);
    
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
    
    int keyInput = lcd.keypad();
    if (keyInput != 32) {
        switch (keyInput) {
            case 53:
            curr_key = "humidity";
            curr_key.toCharArray(payload, 29);
            xbee.send(zbTx);
            break; 
            case 57:
            curr_key = "hmc";
            curr_key.toCharArray(payload, 29);
            xbee.send(zbTx);
            break; 
            case 55:
            curr_key = "baro";
            curr_key.toCharArray(payload, 29);
            xbee.send(zbTx);
            break;
        }
    }else {
        float temp_temperature = getTemp102();
        if (temp_temperature < 50 && count >= 1000) {
            temp = temp_temperature;
            count = 0;
        }
        String (temp).toCharArray(payload, 29);
    }
    
    count++;
    xbee_respond();
}

