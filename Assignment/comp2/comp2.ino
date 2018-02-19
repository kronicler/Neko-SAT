#include <XBee.h>
#include <Wire.h> //I2C Arduino Library


#define HMC_address 0x1E //0011110b, I2C 7bit address of HMC5883

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

float getHumidity () {
    return (float)analogRead(A0)*100 /1024;
}

void setup()
{
    
    Serial2.begin(9600);
    xbee.setSerial(Serial2);
    Serial.begin(9600);
    
    Wire.begin();
    
    //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(HMC_address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop()
{
    xbee.readPacket();
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
            }
            else
            {
            }
            
            
            // get data
            String *inData;
            uint8_t* rxData = rx.getData();
            String receiver = String((char *)rxData);

            Serial.print (receiver); // The data already includes a new line
            if (receiver == "humidity\n") {
                String myString = String(getHumidity());
                //myString = Serial.readString();
                if (myString.length() > 0) {
                    for (int i = 0; i < 30; i++)
                        payload[i] = "\0";
                    
                    Serial.println ("Message replied!_humidty");
                    myString[myString.length()] = "\n";
                    myString.toCharArray(payload, 29);
                }
                
                xbee.send(zbTx);
            }
            else if (receiver == "hmc\n") {
                // Fill in humidity response here.
                // Send back temperature data here.
                int x, y, z;
                getHMC (&x, &y, &z);
                String myString = String(x);
                myString += " ";
                myString += String (y);
                myString += " ";
                myString += String (z);
                
                if (myString.length() > 0) {
                    for (int i = 0; i < 30; i++)
                        payload[i] = "\0";
                    
                    Serial.println ("Message replied!_hmc");
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
            }
            else if (msr.getStatus() == DISASSOCIATED)
            {
                // this is awful.. flash led to show our discontent
            }
            else
            {
                // another status
            }
        }
        else
        {
            // not something we were expecting
        }
    }
}

