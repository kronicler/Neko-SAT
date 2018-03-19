int heading_camera = 0;

void setup () {
    Serial2.begin(9600);
    Serial.begin(9600);
}


void loop()
{
    // Sender adaptation
    if (Serial2.available() > 0) {
      heading_camera = Serial2.read();
      Serial.println (heading_camera);
      // 0 by default
    }
    
}

