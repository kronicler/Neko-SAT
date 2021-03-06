#include <XBee.h>

XBee xbee = XBee();

// create reusable response objects for responses we expect to handle 
XBeeResponse response   = XBeeResponse();
ZBRxResponse        rx  = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

int statusLed = 10; // green
int errorLed  = 11; // red
int dataLed   = 12; // yellow
int led       = 13;
int value     = 0;
float voltage = 0.0;

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
  pinMode(led,       OUTPUT);
  
  // start xbee in serial 2
  Serial2.begin(9600);
  xbee.setSerial(Serial2);
  Serial.begin(9600);
  flashLed(statusLed, 1, 10);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() 
{
  digitalWrite(led, HIGH); // turn the led pin on
    xbee.readPacket();
    flashLed(dataLed, 1, 10);  
    if (xbee.getResponse().isAvailable())
    {
      // Serial.println("Received");

      // got something
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) 
      {
        // got a zb rx packet
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
     //   if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) 
        if (rx.getOption() == 65) 
        {
           // the sender got an ACK
           // flashLed(statusLed, 5, 10); 
           flashLed(statusLed, 1, 10); 
        } 
        else 
        {
           // we got it (obviously) but sender didn't get an ACK
                     
           // flashLed(errorLed, 2, 50);
             flashLed(statusLed, 2, 10); 
           // returned 65 
           // Serial.print("rx.getOption: ");
           // Serial.println(rx.getOption());         
        }
        // get data
        String *inData;
        uint8_t* rxData = rx.getData();

        Serial.print (String((char *)rxData)); // The data already includes a new line
      } 
      else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE)
      {
        xbee.getResponse().getModemStatusResponse(msr);
        // the local XBee sends this response on certain events, 
        // like association/dissociation
        if (msr.getStatus() == ASSOCIATED) 
        {
          // yay this is great.  flash led
          flashLed(statusLed, 5, 100);
        } 
        else if (msr.getStatus() == DISASSOCIATED) 
        {
          // this is awful.. flash led to show our discontent
          flashLed(errorLed, 10, 50);
        }
        else 
        {
          // another status
          flashLed(statusLed, 10, 100);
        }
      }
      else
      {
        // not something we were expecting
        flashLed(errorLed, 3, 100);    
      }
    }
}
