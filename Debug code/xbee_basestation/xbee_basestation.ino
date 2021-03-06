
#include <time.h>
#include <XBee.h>

// create the XBee object
XBee xbee = XBee();

uint8_t payload[30] = "\0";

// SH + SL Address of receiving XBee                             // Remote XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4098DA08); // address of the receiver XBee
// Or 0x4098DA02

ZBRxResponse        rx  = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int value     = 0;
int statusLed = 10; // green
int errorLed  = 11; // red
int dataLed   = 12; // yellow
String myString = "";

//void flashLed(int pin, int times, int wait)
//{
//    for (int i = 0; i < times; i++)
//    {
//        digitalWrite(pin, HIGH);
//        delay(wait);
//        digitalWrite(pin, LOW);
//        if (i + 1 < times) delay(wait);
//    }
//}

void setup()
{
    pinMode(statusLed, OUTPUT);
    pinMode(errorLed,  OUTPUT);
    pinMode(dataLed,   OUTPUT);
    
    // start xbee in serial 2
    Serial2.begin(9600);
    Serial.begin (9600);
    xbee.setSerial(Serial2);
    // xbee.begin(9600);
    
}

void loop()
{
    // Add a detector to see if arduino has reecived any serial inputs so that it wont spam
    // continuously
    myString = Serial.readString();
    if (myString.length() > 0) {
        myString.toCharArray(payload, 29);
        Serial.println ("Message updated!");
        Serial.println ("Message sent!");
        xbee.send(zbTx);
    }    
    // after sending a tx request, we expect a status response
    // wait up to half a second for the status response
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable())
    {
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
        {
            // got a zb rx packet
            // now fill our zb rx class
            xbee.getResponse().getZBRxResponse(rx);

            uint8_t* rxData = rx.getData();
            
            Serial.println((char *)rxData);
            String receiver = String((char *)rxData);
        }
    }
}

