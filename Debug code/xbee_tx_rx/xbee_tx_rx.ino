// TODO: Make the receiver send data back from a sensor when it receives the right string. 

#include <XBee.h>

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
             flashLed(statusLed, 2, 10); 
        }

        
        // get data
        String *inData;
        uint8_t* rxData = rx.getData();

        Serial.print (String((char *)rxData)); // The data already includes a new line
        String receiver = String((char *)rxData); 
        if (receiver == "temp\n") {
          // Send back temperature data here. 
          String myString = "hello chin chin";
          //myString = Serial.readString();
          if (myString.length() > 0) {
            for (int i = 0; i < 30; i++)
            payload[i] = "\0";

            Serial.println ("Message replied!");
            myString[myString.length()] = "\n";
            myString.toCharArray(payload, 29);
          }
   
          xbee.send(zbTx);
        }
        else if (receiver == "humidity\n") {
          // Fill in humidity response here. 
          // Send back temperature data here. 
          String myString = "hello chin chin humidity here";
          //myString = Serial.readString();
          if (myString.length() > 0) {
            for (int i = 0; i < 30; i++)
            payload[i] = "\0";

            Serial.println ("Message replied!_humidity");
            myString[myString.length()] = "\n";
            myString.toCharArray(payload, 29);
          }
   
          xbee.send(zbTx);
        }
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
