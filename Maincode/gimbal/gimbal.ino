// 2 servo planar stabilization system
// wp
// Jan 2016
//
// Based on Jeff Rowber's work found at
// https://github.com/jrowberg/i2cdevlib/blob/master/Arduino/MPU6050/MPU6050.cpp
//
// Use at your own risk.
//
// This code is placed under the MIT License (MIT)
//
// Copyright (c) 2016 woojay poynter

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//

// Servo Connection
// BROWN - gnd
// red - 5v
// yellow - d10 (pwm on Sero 1)
//        - d11 (servo 2)

// MPU Connection
//
// VCC - 5v
// GND - GND
// SCL - A5 (w/ 10k PuR)
// SDA - A4 (w/ 10k PuR)
// INT - D2 (not used)

#include <Servo.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define LED_PIN 13
bool blinkState = true;

Servo Servo1;   // First Servo off the chassis
Servo Servo2;   // Second Servo off the chassis

int Servo1Pos = 0;
int Servo2Pos = 0;


float mpuPitch = 0;
float mpuRoll = 0;
float mpuYaw = 0;


// define MPU instance
MPU6050 mpu;                    // class default I2C address is 0x68; specific I2C addresses may be passed as a parameter here

// MPU control/status vars
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


// TODO: Change the following to global variables and allow camera controls to control it. 


// relative ypr[x] usage based on sensor orientation when mounted, e.g. ypr[PITCH]
#define PITCH   1     // defines the position within ypr[x] variable for PITCH; may vary due to sensor orientation when mounted
#define ROLL  2     // defines the position within ypr[x] variable for ROLL; may vary due to sensor orientation when mounted
#define YAW   0     // defines the position within ypr[x] variable for YAW; may vary due to sensor orientation when mounted

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup()
{

  Servo1.attach(9);  // attaches the servo on D11 to the servo object
  Servo2.attach(10);  // Second servo on D11
  delay(50);
  /*
  Servo1.write(0);  // These are command checks to see if the servos work and
  Servo2.write(60); // to help w/ the initial installation.
  delay(500);   // Make sure these movements are clear from the rest of the chassis.
  Servo1.write(180);
  Servo2.write(120);
  delay(500);
  Servo1.write(0);
  Servo2.write(90);
  */
  delay(500);

  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial1.begin(9600);
  //Serial.begin(9600);
  //while (!Serial1);      // wait for Leonardo enumeration, others continue immediately

  // initialize device
  //Serial1.println(F("Initializing I2C devices..."));
  mpu.initialize();

  /*
  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP"));
  */
  
  devStatus = mpu.dmpInitialize();

  // TODO: 
  // INPUT CALIBRATED OFFSETS HERE; SPECIFIC FOR EACH UNIT AND EACH MOUNTING CONFIGURATION!!!!

  mpu.setXGyroOffset(16);
  mpu.setYGyroOffset(-53);
  mpu.setZGyroOffset(33);
  mpu.setXAccelOffset(-1127);
  mpu.setYAccelOffset(-5434);
  mpu.setZAccelOffset(609);

  // make sure it worked (returns 0 if so)
  if (devStatus == 0)
  {
    // turn on the DMP, now that it's ready
    //Serial.println(F("Enabling DMP"));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)"));
    mpuIntStatus = mpu.getIntStatus();

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  else
  {
    // ERROR!
    // 1 = initial memory load failed, 2 = DMP configuration updates failed (if it's going to break, usually the code will be 1)
    //Serial.print(F("DMP Initialization failed code = "));
    //Serial.println(devStatus);
  }

  // configure LED for output
  pinMode(LED_PIN, OUTPUT);

} // setup()



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop(void)
{
  // TODO: Make a receiver that receives command here 
  // Sender adaptation
  if (Serial1.available() > 0) {
      int current;
      current = Serial1.read();
      digitalWrite(17, HIGH); 
      Serial.println (current);
      switch (current) {
          case '0':
          // No movement 
          break; 
          case '1':
          // Move upwards
          Servo1Pos+= 5;
          Servo1Pos = (Servo1Pos >= 90) ? 0 : Servo1Pos;
          break;
          case '2':
          // Move downwards
          Servo1Pos-= 5;
          Servo1Pos = (Servo1Pos <= 0) ? 90 : Servo1Pos;
          break; 
          case '3':
          // Move Left
          Servo2Pos+= 5;
          Servo2Pos = (Servo2Pos >= 90) ? 0 : Servo2Pos;
          break; 
          case '4':
          // Move right
          Servo2Pos-= 5;
          Servo2Pos = (Servo2Pos <= 0) ? 90 : Servo2Pos;
      }
  }else {
      digitalWrite(17, LOW); 

  }

  
  processAccelGyro();
}   // loop()



// ================================================================
// ===                    PROCESS ACCEL/GYRO IF AVAILABLE       ===
// ================================================================

void processAccelGyro()
{

  // Get INT_STATUS byte
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024)
  {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    //Serial.println(F("FIFO overflow!"));
    return;
  }

  if (mpuIntStatus & 0x02)  // otherwise continue processing
  {
    // check for correct available data length
    if (fifoCount < packetSize)
      return; //  fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    fifoCount -= packetSize;

    // flush buffer to prevent overflow
    mpu.resetFIFO();

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    mpuPitch = ypr[PITCH] * 180 / M_PI;
    mpuRoll = ypr[ROLL] * 180 / M_PI;
    mpuYaw  = ypr[YAW] * 180 / M_PI;

    // flush buffer to prevent overflow
    mpu.resetFIFO();

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);

    // flush buffer to prevent overflow
    mpu.resetFIFO();
    
    // TODO: Implement a count feature here to count when the servo is in the right
    // position then stop

    
    //if (fabs(mpuPitch) > 1)
    //else 
      //Servo1.write (81); // Stop
      
    //if (fabs(mpuRoll) > 1)

    Servo2.write(90 + Servo2Pos);

    if (Servo2.read() >= 135 && Servo2.read() < 180) {
        Servo1.write(mpuRoll + 90 + Servo1Pos);
    }else if (Servo2.read() >= 90 && Servo2.read() < 135) {
        Servo1.write(mpuPitch + 90 + Servo1Pos);
    }else if (Servo2.read() >= 45 && Servo2.read() < 90) {
        Servo1.write(-mpuRoll + 90 + Servo1Pos);
    }else {
        Servo1.write(-mpuPitch + 90 + Servo1Pos);
    }
        


    //Serial.print (Servo1.read());
    //Serial.print (" ");
    //Serial.println (Servo2.read());
    //else 
      //Servo2.write(84); // stop

    /*
    Serial.print ("mpupitch: ");
    Serial.println (mpuPitch);
    Serial.print ("mpuroll: ");
    Serial.println (mpuRoll);
    //delay(10);
    */
    
    // flush buffer to prevent overflow
    mpu.resetFIFO();

  } // if (mpuIntStatus & 0x02)
}  // processAccelGyro()



