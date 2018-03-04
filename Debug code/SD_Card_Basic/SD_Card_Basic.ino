// SD_Card_Basic
// The circuit: Arduino mega 2560 -> microSD Shield 
//  MOSI - pin 51 -> 11
//  MISO - pin 50 -> 12
//  CLK  - pin 52 -> 13
//  CS   - pin 53 -> 8

#include <SD.h>
#define FILE "datalog.csv"

//Chip Select Pin on Arduino
const int chipSelect = 53;

// Array to write into card
byte byteArr[5] = {0x41,0x42,0x43,0x44,0x45};

void setup()
{
  // Open serial communication
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  // Initialise Card
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("Init failed, or not present");
    // Initialisation failed, do not continue
    return;
  }
  
  // Initialisation Complete
  Serial.println("Card initialised.");
  
//  // Check if file already exist
//  if (SD.exists(FILE)) 
//  {
//    // Delete existing file
//    Serial.print(FILE);
//    Serial.println(" exists, deleting");
//    SD.remove(FILE);
//    Serial.println();
//  }
//  else 
//  {
//    Serial.print(FILE);
//    Serial.println(" not found!");
//  }
}

void loop()
{
  // Open or create new file for writing 
  File dataFile;// = SD.open(FILE, FILE_WRITE);
//
//  // Check if file is available 
//  if (dataFile) 
//  {
//    // File available, write the bytes into it
//    for(int i=0 ; i<5; i++) 
//    {
//      for(int j=0; j <5; j++) 
//	  {
//        dataFile.print("S");
//        if ( j<4) dataFile.print(",");
//      }
//      dataFile.println();
//    }
//    // Close file
//    dataFile.close();
//  }  
  // If the file isn't open, show error
//  else 
//  {
//    Serial.print("Error - ");
//    Serial.print(FILE);
//    Serial.println(" not opened");
//  }
   
  // Opening file for reading
  dataFile = SD.open(FILE, FILE_READ);
    
  // Check if file is available 
  if (dataFile) 
  {
    // Byte containing last read data
	byte readByte = 0;
	String completeData = "";
  String humidityS = "";
  String baroTempS = "";
  String baroPresS = "";
  String angleS = "";
  int endline=0,i=0;
	Serial.println("File Data: ");
  
	// File available, read file until no data available
	while(dataFile.available()) 
	{
	  readByte = dataFile.read();
//    if ((char)readByte == ','){
//        switch(i){
//            case 0:
//            humidityS = completeData;
//            i++;
//            break;
//            case 1:
//            baroTempS = completeData;
//            i++;
//            break;
//            case 2:
//            baroPresS = completeData;
//            i++;
//            break;
//        }
//        completeData = "";
//        continue;
//    }
//    if ((char)readByte == '\n'){
//        angleS = completeData;
//        completeData = "";
//        endline = 1;
//    }
//    else
    Serial.print((char)readByte);
	  completeData += (char)readByte;
	}
	Serial.println("");
	// Print file data to screen
//	Serial.println(humidityS);
//  Serial.println(baroTempS);
//  Serial.println(baroPresS);
//  Serial.println(angleS);
	
	// Clear Variables
	readByte = 0;
	completeData = "";
  }
  // If the file isn't open, show error
  else 
  {
    Serial.print("Error - ");
    Serial.print(FILE);
    Serial.println(" not opened");
  }
  // Close file
  dataFile.close();
  
  while(1);
}

