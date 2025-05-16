// =================================================================================
//  File             : BTModeRX.ino
//  Description      : Receiver firmware for controlling the Pedro robot
//                     in Bluetooth mode with HC-05 module.
//  Supported Boards : Rev3
//  
//  Author           : Almoutazar SAANDI
//  Date             : May 16, 2025
//  Last Update      : v1.0.0
//
//  Robot Firmware Requirement:
//  TBD
// =================================================================================

#include <Servo.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSerif9pt7b.h>

#define nombreDePixelsEnLargeur   128        
#define nombreDePixelsEnHauteur   64        
#define brocheResetOLED           -1          
#define adresseI2CecranOLED       0x3C       
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);                                                                                                      

int msg[4];

Servo servo5;
Servo servo6;
Servo servo9;
Servo servo10;

const int buttonA0 = A0;
const int buttonA1 = A1;
const int buttonA2 = A2;

int buttonStateA0;
int buttonStateA1;
int buttonStateA2;

const int ledPins[] = {13, 11, 8, 7}; 
Servo servoList[] = {servo5, servo6, servo9, servo10};
const int servoPins[] = {5, 6, 9, 10};
int servoID,LEDID,rotation;

int currentLed,previousLed; 
int buttonState = LOW;
int lastButtonState = LOW;
int delayTime = 20;  

// Can be adjusted based on your Pedro Robot's movement. 
// Higher values result in faster movement, lower values result in slower movement.
int speed = 300; 
////////////

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial1.begin(9600);
  Serial.println("Communication avec HC-05 établie !");
  
  pinMode(buttonA0, INPUT);
  pinMode(buttonA1, INPUT);
  pinMode(buttonA2, INPUT);

  for (int i = 0; i < 4; i++) {
     pinMode(ledPins[i], OUTPUT);
     digitalWrite(ledPins[i], LOW);
     servoList[i].attach(servoPins[i]);
  }

  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                              

  ecranOLED.clearDisplay();                          
  ecranOLED.drawRect(0, 0, nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, SSD1306_WHITE);
  ecranOLED.setFont(&FreeSerif9pt7b);  
  ecranOLED.setTextColor(SSD1306_WHITE);  
  ecranOLED.setCursor(5, 15);           
  ecranOLED.print("Mode Bluetooth");   
  ecranOLED.setCursor(25, 40);            
  ecranOLED.print("Pedro RX");   
  ecranOLED.display();    
}

void loop() {

  String receivedData = "";
  
  while (Serial1.available()) {  
    char c = Serial1.read();  
    if (c == '\n') break; 
    receivedData += c; 
  }

  if (receivedData == "1") {
    currentLed = 0;
  } else if (receivedData == "2") {
    currentLed = 1;
  } else if (receivedData == "3") {
    currentLed = 2;
  } else if (receivedData == "4") {
    currentLed = 3;
  }
  
  for (int i = 0; i < 4; i++) {
   digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[currentLed], HIGH); 
 
  if (receivedData == "5") {
    rotation = 10;
  } else if (receivedData == "6") {
    rotation = 20;
  } else if (receivedData == "7") {
    rotation = 30;
  } 
  
  if (rotation == 10) {
    servoList[currentLed].writeMicroseconds(1500 + speed);
  } else if (rotation == 20) {
    servoList[currentLed].writeMicroseconds(1500 - speed);
  } else if (rotation == 30) {
    servoList[currentLed].writeMicroseconds(1500);
  }

}
