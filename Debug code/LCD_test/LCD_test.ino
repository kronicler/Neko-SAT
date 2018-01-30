#include <Wire.h>
#include <LCDi2cR.h>


LCDi2cR lcd = LCDi2cR(4, 20, 0x63, 0); 
uint8_t rows = 4;
uint8_t cols = 20;

void setup()
{
  lcd.init(); // Init & clear display lcd.print("Hello World!"); delay(1000);
}

void lcdtest_basic() {
  lcd.clear();
  //LCDi2cR Lib;
  
  lcd.print ("Hello world");
  lcd.setCursor(1,0); // Acts as a print new line on the LCD matrix 
  
  delay(1000);
 }

void loop() {
  lcdtest_basic();
}

