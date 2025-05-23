// =================================================================================
//  File             : replayMode.ino
//  Description      : Firmware for controlling the Pedro robot
//                     in direct mode and replayMode with recording movements.
//  Supported Boards : Rev2 and Rev3
//  Servo Type       : 360° SG90 or MG90
//
//  Author           : Almoutazar SAANDI
//  Date             : May 18, 2025
//  Last Update      : v1.0.0
// =================================================================================

#include <Servo.h>

int currentLed, lastServo = 0;
int lastButtonState = LOW;

const int buttonA0 = A0;
const int buttonA1 = A1;
const int buttonA2 = A2;

int buttonStateA0;
int buttonStateA1;
int buttonStateA2;

const int ledPins[] = {13, 11, 8, 7};
const int servoPins[] = {5, 6, 9, 10}; 
// servoSpeed => Can be adjusted based on your Pedro Robot's movement. 
// Higher values result in faster movement, lower values result in slower movement.
const int servoSpeed[] = {-150, -150, 150, 100};
////////////
Servo servoList[4];

struct Movement {
  int pulse;
  unsigned long duration;
  int servo;
};

Movement movements[100];
int movementIndex = 0;

unsigned long buttonPressStart = 0;

bool recording, buttonHeld, hasRecorded, button3sec = false;

int lastPulse = 1500;
unsigned long lastChange = 0;

unsigned long lastPressTime = 0;
int pressCount = 0;
const unsigned long doubleClickDelay = 500;

void setup() {
  Serial.begin(9600);
  servoList[0].attach(6);
  pinMode(buttonA0, INPUT);
  pinMode(buttonA1, INPUT);
  pinMode(buttonA2, INPUT);

  for (int i = 0; i < 4; i++) {
       pinMode(ledPins[i], OUTPUT);
       digitalWrite(ledPins[i], LOW);
       servoList[i].attach(servoPins[i]);
  }
  digitalWrite(ledPins[0], HIGH);
}

void loop() {
  buttonStateA0 = digitalRead(buttonA0);
  buttonStateA1 = digitalRead(buttonA1);
  buttonStateA2 = digitalRead(buttonA2);

  if (buttonStateA0 == HIGH) {
    if (!buttonHeld) {
      buttonPressStart = millis();
      buttonHeld = true;
      button3sec = false;
    } else if (millis() - buttonPressStart >= 3000) {
      if (recording) {
        Serial.println("Starting replay...");
        hasRecorded = true;
        recording = false;
        replayMovements(); 
        button3sec = false;
        Serial.println("🎥End of Raplay...");
      } else {
        Serial.println("Starting recording...");
        hasRecorded = false;
        recording = true;
        movementIndex = 0;
        lastChange = millis();
      }
      button3sec = true;
      while (digitalRead(buttonA0) == HIGH);
      buttonHeld = false;
      delay(1000);
    }
  } else {
    buttonHeld = false;
  }
 
  if (!button3sec) {
    if (buttonStateA0 == LOW && lastButtonState == HIGH) {
        digitalWrite(ledPins[currentLed], LOW);
        currentLed = (currentLed + 1) % 4;
        digitalWrite(ledPins[currentLed], HIGH);
        delay(200);
    }
  }
 
  lastButtonState = buttonStateA0; 
  
  int pulse = 1500; // neutre
  if (buttonStateA1 == HIGH) {
    pulse = 1500 + servoSpeed[currentLed];
  } else if (buttonStateA2 == HIGH) {
    pulse = 1500 - servoSpeed[currentLed];
  } 

  servoList[currentLed].writeMicroseconds(pulse);

  if (recording) {
    if (abs(pulse - lastPulse) > 5) {
      unsigned long now = millis();
      if (movementIndex < 100) {
        movements[movementIndex++] = {lastPulse, now - lastChange, currentLed};
        Serial.print("Servo: ");
        Serial.print(currentLed);
        Serial.print(" movementIndex: ");
        Serial.println(movementIndex);
      }
      lastChange = now;
      lastPulse = pulse;
    }
  }
}

void replayMovements() {
  while (true) {

      // Mettre tous les servos à neutre avant de commencer
      for (int i = 0; i < 4; i++) {
        servoList[i].writeMicroseconds(1500);
        digitalWrite(ledPins[i], LOW);
      }
    
      // Lecture séquentielle des mouvements
      for (int i = 0; i < movementIndex; i++) {
        int currentServo = movements[i].servo;
    
        // Activer uniquement le servo concerné
        for (int j = 0; j < 4; j++) {
          if (j == currentServo) {
            digitalWrite(ledPins[j], HIGH); // Allumer la LED correspondante
          } else {
            digitalWrite(ledPins[j], LOW);  // Éteindre les autres
            servoList[j].writeMicroseconds(1500); // Stopper les autres servos
          }
        }
    
        // Exécuter le mouvement
        servoList[currentServo].writeMicroseconds(movements[i].pulse);
        delay(movements[i].duration);
    
        // Remettre le servo à neutre après le mouvement
        servoList[currentServo].writeMicroseconds(1500);
      }
    
      // Éteindre toutes les LEDs à la fin
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
        servoList[i].writeMicroseconds(1500);
      }
    
      Serial.println("🎥 Relecture terminée.");
  }
}
