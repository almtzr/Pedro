#include <Servo.h>

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

const int ledPins[] = {13, 11, 8, 7}; // Broches des LEDs
Servo servoList[] = {servo5, servo6, servo9, servo10};

int currentLed = 0; // Indice de la LED actuellement allumée
int buttonState = LOW;
int lastButtonState = LOW;

int speed = 200; 
int delayTime = 20;  // Délai pour stabiliser le mouvement

void setup() {
  Serial.begin(9600);
  pinMode(buttonA0, INPUT);
  pinMode(buttonA1, INPUT);
  pinMode(buttonA2, INPUT);

  for (int i = 0; i < 4; i++) {
     pinMode(ledPins[i], OUTPUT);
     digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[0], HIGH);

  servo5.attach(5);
  servo6.attach(6);
  servo9.attach(9);
  servo10.attach(10);
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
    servoList[currentLed].writeMicroseconds(1500 + speed);
  } else if (buttonStateA2 == HIGH) {
    servoList[currentLed].writeMicroseconds(1500 - speed);
  } else {
    servoList[currentLed].writeMicroseconds(1500);
  }

  delay(delayTime);
}
