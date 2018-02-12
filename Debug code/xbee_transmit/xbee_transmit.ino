#include <XBee.h>

// create the XBee object
XBee xbee = XBee();

uint8_t payload[30] = "\0";

// SH + SL Address of receiving XBee                             // Remote XBee    
 XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4098DA08); // address of the receiver XBee
// Or 0x4098DA02


ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int value     = 0;
int statusLed = 10; // green
int errorLed  = 11; // red
int dataLed   = 12; // yellow

void flashLed(int pin, int times, int wait) 
{
  for (int i = 0; i < times; i++) 
  {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);
    if (i + 1 < times) delay(wait);
  }
}

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
   String myString = "";
   myString = Serial.readString();
   if (myString.length() > 0) {
    for (int i = 0; i < 30; i++)
      payload[i] = "\0";

    Serial.println ("Message sent!");
    myString[myString.length()] = "\n";
    myString.toCharArray(payload, 29);
   }
   
   xbee.send(zbTx);
   // flash TX indicator
   flashLed(dataLed, 1, 10);
    
   // after sending a tx request, we expect a status response
   // wait up to half a second for the status response
   if (xbee.readPacket(500))
   {
     // got a response!
     // should be a znet tx status              
     if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
     {
        xbee.getResponse().getZBTxStatusResponse(txStatus);
        // get the delivery status, the fifth byte
        if (txStatus.getDeliveryStatus() == SUCCESS) 
        {
          // success.  time to celebrate
          // flashLed(statusLed, 5, 10); 
      flashLed(statusLed, 1, 10); 
        } 
        else
        {
          // the remote XBee did not receive our packet. is it powered on?
          flashLed(errorLed, 1, 50);
        }
      }      
   } 
   else 
   {
     // local XBee did not provide a timely TX Status Response -- should not happen
     flashLed(errorLed, 5, 50);
   }
   delay(1000);
}
