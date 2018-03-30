#include <Servo.h>


Servo Servo1;


void setup() {
    Servo1.attach(10);
    Servo1.write (0);

    //Servo1.write (0);
    Serial.begin (9600);

}


void loop () {
     Servo1.write(45);      // Turn SG90 servo Left to 45 degrees
     delay(1000);          // Wait 1 second
     //Servo1.write(90);      // Turn SG90 servo back to 90 degrees (center position)
     delay(1000);          // Wait 1 second
     //Servo1.write(135);     // Turn SG90 servo Right to 135 degrees
     delay(1000);          // Wait 1 second
     Servo1.write(90);      // Turn SG90 servo back to 90 degrees (center position)
     delay(1000);

}
