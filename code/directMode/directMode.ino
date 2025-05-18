// =================================================================================
//  File             : directMode.ino
//  Description      : Firmware for controlling the Pedro robot
//                     in direct mode using the Pedro board's button.
//  Supported Boards : Rev2 and Rev3
//  Servo Type       : 360° SG90 or MG90
//
//  Author           : Almoutazar SAANDI
//  Date             : May 18, 2025
//  Last Update      : v1.0.1
// =================================================================================


#include <Servo.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Fonts/Org_01.h>
#include <Fonts/FreeSerif9pt7b.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define nombreDePixelsEnLargeur   128        
#define nombreDePixelsEnHauteur   64        
#define brocheResetOLED           -1          
#define adresseI2CecranOLED       0x3C       
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);                                                                                                      

const int buttonA0 = A0;
const int buttonA1 = A1;
const int buttonA2 = A2;

int buttonStateA0;
int buttonStateA1;
int buttonStateA2;

const int ledPins[] = {13, 11, 8, 7}; // Broches des LEDs
const int servoPins[] = {5, 6, 9, 10}; // Broches des LEDs
Servo servoList[4] ;

int currentLed = 0; // Indice de la LED actuellement allumée
int lastButtonState = LOW;
int delayTime = 20;  // Délai pour stabiliser le mouvement

// servoSpeed => Can be adjusted based on your Pedro Robot's movement. 
// Higher values result in faster movement, lower values result in slower movement.
const int servoSpeed[] = {150, 200, 150, 100};
////////////

void setup() {
  Serial.begin(9600);
  pinMode(buttonA0, INPUT);
  pinMode(buttonA1, INPUT);
  pinMode(buttonA2, INPUT);

  for (int i = 0; i < 4; i++) {
       pinMode(ledPins[i], OUTPUT);
       digitalWrite(ledPins[i], LOW);
       servoList[i].attach(servoPins[i]);
  }
  digitalWrite(ledPins[0], HIGH);

  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
  while(1);                              

  ecranOLED.clearDisplay();                          

  ecranOLED.drawRect(0, 0, nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, SSD1306_WHITE);
  ecranOLED.setFont(&FreeSerif9pt7b);  
  ecranOLED.setTextColor(SSD1306_WHITE);  
  ecranOLED.setCursor(18, 15);           
  ecranOLED.print("Pedro Robot");   
  ecranOLED.setCursor(18, 35);            
  ecranOLED.print("Direct Mode");  
  ecranOLED.display(); 
}

void loop() {
  buttonStateA0 = digitalRead(buttonA0);
  buttonStateA1 = digitalRead(buttonA1);
  buttonStateA2 = digitalRead(buttonA2);

  if (buttonStateA0 == LOW && lastButtonState == HIGH) { // Détection du front descendant
      digitalWrite(ledPins[currentLed], LOW);  // Éteint la LED actuelle
      currentLed = (currentLed + 1) % 4;  // Passe à la LED suivante
      digitalWrite(ledPins[currentLed], HIGH); // Allume la nouvelle LED
      delay(200); // Anti-rebond
  }

  lastButtonState = buttonStateA0; // Sauvegarde de l'état du bouton
    
  if (buttonStateA1 == HIGH) {
    // Vitesse dans une direction (par exemple, 1600 µs pour rotation dans un sens)
    servoList[currentLed].writeMicroseconds(1500 + servoSpeed[currentLed]);
  } else if (buttonStateA2 == HIGH) {
    // Vitesse dans l'autre direction (par exemple, 1400 µs pour rotation dans l'autre sens)
    servoList[currentLed].writeMicroseconds(1500 - servoSpeed[currentLed]);
  } else {
    // Arrêt du servo (1500 µs = position neutre, pas de rotation)
    servoList[currentLed].writeMicroseconds(1500);
  }
    
  // Contrôle des servos en fonction des LED allumées et des boutons
  Serial.print ("speed: ");
  Serial.println (servoSpeed[currentLed]);

  delay(delayTime);  // Attendre un court délai pour stabiliser le mouvement
}
