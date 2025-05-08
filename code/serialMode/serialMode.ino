// =================================================================================
//  File             : serialMode.ino
//  Description      : Firmware for controlling the Pedro robot
//                     in serial mode. Enables communication 
//                     with the Pedro Serial Controller app.
//  Supported Boards : Rev2 and Rev3
//
//  Author           : Almoutazar SAANDI
//  Date             : May 7, 2025
//  Last Update      : v1.0.0
//
//  Robot Firmware Requirement:
//  --------------------------------------------------------------------------------
//  This sketch must be uploaded to the Pedro robot board
//  for proper communication with the Pedro Serial Controller app.
//
//  Communication: Serial (baudrate 9600)
//  Compatible with Python application "Pedro Serial Controller"
//  Make sure to connect via a USB cable for programming and control.
//
//  Repository/Source: https://github.com/almtzr/Pedro/tree/main/app/pedro_app.py
// =================================================================================

#include <Servo.h>

Servo servo5;
Servo servo6;
Servo servo9;
Servo servo10;

Servo servoList[] = {servo5, servo6, servo9, servo10};
const int servoPins[] = {5, 6, 9, 10};
const int ledPins[] = {13, 11, 8, 7}; 

// Can be adjusted based on your Pedro Robot's movement. 
// Higher values result in faster movement, lower values result in slower movement.
int speed = 400; 
////////////

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
     pinMode(ledPins[i], OUTPUT);
     digitalWrite(ledPins[i], LOW);
     servoList[i].attach(servoPins[i]);
  }
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    int servoNum = command.charAt(0) - '1';
    char direction = command.charAt(1);
    Serial.print ("Dir: ");
    Serial.print (direction);
    Serial.print (" Servo: ");
    Serial.println (servoNum);

    for (int i = 0; i < 4; i++) {
     digitalWrite(ledPins[i], LOW);
    }
    digitalWrite(ledPins[servoNum], HIGH); 
    
    if (servoNum >= 0 && servoNum < 4) {
      if (direction == 'L') {
        servoList[servoNum].writeMicroseconds(1500 + speed);
      } else if (direction == 'R') {
        servoList[servoNum].writeMicroseconds(1500 - speed);
      } else {
        servoList[servoNum].writeMicroseconds(1500);
        Serial.println ("Idle State");
      }
    }
  }
}