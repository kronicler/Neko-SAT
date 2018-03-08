const byte interruptPin = 19;//In arduino MEGA RX 19. TX 18
String msg = "";//Incomming message

void setup() {
  Serial.begin(9600);//Serial Monitor
  Serial1.begin(9600);//Incomming serial messages
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), serial_read, HIGH); // Use serial1 as the interrupt serial
}


void serial_read(){
  char _bite;
  sei();//Disable hardware interrupts for a moment
  while(Serial1.available()>0){
    delay(1);//Do not delet this delay
    if(Serial1.available()>0){
      _bite = (char)Serial1.read();
      msg += _bite;
      if(_bite == '\n'){
        Serial.print(msg); //Do what you want with your message
        msg = "";//Clean message for new one
        break;
      }
    }
  }
  cli();//reenabling hardware interrupts
}

void loop() {
  // put your main code here, to run repeatedly:
  

  
}


