#include <Servo.h>


Servo Servo1;


void setup() {
    Servo1.attach(11);
}


void loop () {
    for (int i = 0; i < 90; i++) {
        Servo1.write (i);
    }


}
