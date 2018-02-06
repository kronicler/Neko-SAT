#include <XBee.h>

/*
This example is for Series 2 XBee
Receives a ZB RX packet and sends the packet data to PC.
Error led is flashed if an unexpected packet is received
*/

XBee xbee               = XBee();

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
// xbee.begin(9600);
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
      Serial.println("Received");

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
        /**
        value = rx.getData(0) << 8 | rx.getData(1);
        voltage = value * 5.0 / 1024.0;
        Serial.print("Voltage: ");
        Serial.print(voltage, 4);
        Serial.println(" V ");
        delay(500);
        digitalWrite(led, LOW); // turn the led pin off
        delay(500);
        */
        char returnData = rx.getData(0);
        Serial.println (returnData);
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
