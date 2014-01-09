/*
 * BLEbee Test Sketch for Arduino 1.0.5
 * v1.0.0 2013-09-01
 * 
 * This Sketch is used to test the BLEBees communication features
 * with an Arduino FIO
 *
 * Copyright (c) 2013 Dr. Michael Kroll
 *
 * http://www.mkroll.mobi
 * http://forum.mkroll.mobi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons 
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <SPI.h>
#include <ble.h>
#include <boards.h>
#include <ble_shield.h>
#include <ble_mini.h>
#include <services.h>
// Pin 13 has an LED connected on most Arduino boards.
int led = 13;

long previousMillis = 0; 
long interval = 1000; 

long cnt = 0;

#define I_InA1            7                  
#define I_InB1            8   

#define I_InC1            9                  
#define I_InD1            10  

#define LED_I            A2                 
#define LED_II           A3 


int speedValue = 150;
 char fb;
 char lr;
  int I_PWM_val = 0;
  int II_PWM_val =0;
  int steeringActive = 0;
  int throttleActive = 0;

byte runXTimes = 1;
   byte valForMotor1;
    byte dirForMotor1;
 
    //byte motor2;
    byte valForMotor2;
    byte dirForMotor2;
    
     int data;
     
     unsigned char buf[4] = {0};
     unsigned char len = 0;
 
void setup() {
  
  pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);

   pinMode(I_InA1, OUTPUT);
  pinMode(I_InB1, OUTPUT);
   pinMode(I_InC1, OUTPUT);
  pinMode(I_InD1, OUTPUT);
  
  
  // Set the data rate for the Serial where the BLEbee is connected to
  // The BLEBee's baudrate can be controlled by characteristic: 
  // 2FBC0F31-726A-4014-B9FE-C8BE0652E982
  // 0x00 = 9600 (default)
  // 0x01 = 14400
  // 0x02 = 19200
  // 0x03 = 28800
  // 0x04 = 38400
  Serial.begin(9600);
  Serial.print("BLEbee v1.0.0 Sketch setup...");
}
 
void loop() { // run over and over
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;       
    //Serial.print("Hello from Arduino FIO: ");      
    //Serial.println(cnt++);
  }
  
  if (Serial.available()) {

     // turn the LED on (HIGH is the voltage level)
      //data = Serial.read();
   
     //data =  Serial.read();
     //digitalWrite(led, HIGH);  
     //digitalWrite(LED_I, HIGH);
   ckhSteering();
chkThrottle();


   
    byte b = Serial.read();
    
      if (b == 0x01){
        fb = '1';
          lr = 's';
          ckhSteering();
          steeringActive = 1;
        throttleActive = 1;
      }
     
      else if (b == 0x02){
       fb = '2';
         lr = 's';
         ckhSteering();
          steeringActive = 1;
    throttleActive = 1;
    }
     
    else if (b == 0x03){
    fb  = '1';
    lr = '4';
    ckhSteering();
     throttleActive = 1;
       steeringActive = 1;
    }
    else if (b == 0x04){
    fb  = '1';
    lr = '6';
    ckhSteering();
        throttleActive = 1;
          steeringActive = 1;
    }
    else if (b == 0x05){
    fb  = '2';
    lr = '4';
    ckhSteering();
    throttleActive = 1;
      steeringActive = 1;
    }
    else if (b == 0x06){
    fb  = '2';
    lr = '6';
    ckhSteering();
   throttleActive = 1;
     steeringActive = 1;
    }
    else if (b == 0x07){
    fb  = 'x';
    lr = '4';
    ckhSteering();
   steeringActive = 1;
    throttleActive = 1;
    }
    else if (b == 0x08){
    fb  = 'x';
    lr = '6';
   steeringActive = 1;
    throttleActive = 1;
   
    }
    else if(b == 0x09){
    fb = 'x';
    lr = 's';
  
    }
    
    


  
  
 



     
// FORWARD BACKWARD //

    //Serial.println(fb);
    if (fb >= '0' && fb <= '1')
    {
     motorForward();         
     
    }
    if (fb >= '2' && fb <= '3')
    {
     motorBackward();
   
   
    }
    if (fb == 'x')
    {
    motorStop();
    
    //Serial.println("throttlestop");
    }
   }
    
    
    // LEFT RIGHT //
     //Serial.println(lr);
     if (lr >= '4' && lr <= '5')
    {
     motorIIBackward();

    }
      if (lr >= '6' && lr <= '7')
    {
   motorIIForward();
    
    }
  if (lr == 's')
    {
    motorIIStop();
 
     // Serial.println("steering stop");
    }
  
 

 

 
  }
  
  void chkThrottle(){
     if (throttleActive == 1){
     fb = 'x';
     throttleActive = 0;
    }
  }
  
  void ckhSteering(){
         if(steeringActive == 1){
    lr='s';
    steeringActive = 0;
    }
  }

void motorIIStop(){
 digitalWrite(led, LOW);  
   digitalWrite(I_InC1, LOW);
  digitalWrite(I_InD1, LOW); 
}

void motorStop(){
  digitalWrite(led, LOW); 
  
  digitalWrite(I_InA1, LOW);
  digitalWrite(I_InB1, LOW);
 
  
}

void motorForward(){
  digitalWrite(led, HIGH);  
 digitalWrite(I_InA1, HIGH);
  digitalWrite(I_InB1, LOW);
}


void motorBackward(){
  digitalWrite(led, HIGH);  
 digitalWrite(I_InA1, LOW);
  digitalWrite(I_InB1, HIGH);
}

void motorIIBackward(){
  digitalWrite(led, HIGH);  
 digitalWrite(I_InC1, HIGH);
  digitalWrite(I_InD1, LOW);
}

void motorIIForward(){
  digitalWrite(led, HIGH);  
 digitalWrite(I_InC1, LOW);
  digitalWrite(I_InD1, HIGH);
}
