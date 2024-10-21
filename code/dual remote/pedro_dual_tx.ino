#include <RF24.h>
#include <Servo.h>

RF24 radio(4, 12);
const uint64_t pipeA = 0xE8E8F0F0E1LL;     //Remember that this code is the same as in the transmitter
int msg[4];

Servo servo5;
Servo servo6;
Servo servo9;
Servo servo10;

const int buttonA5 = A5;
const int buttonA4 = A4;
const int buttonA0 = A0;
const int buttonA1 = A1;
const int buttonA2 = A2;
const int buttonA3 = A3;

int val1,val2;

int buttonStateA5;
int buttonStateA4;
int buttonStateA0;
int buttonStateA1;
int buttonStateA2;
int buttonStateA3;

const int ledD2 = 2;
const int ledD3 = 3;
const int ledD7 = 7;
const int ledD8 = 8;
const int ledD11 = 11;
int speed = 40; 
int delayTime = 20;  // Délai pour stabiliser le mouvement
int ledState = LOW; 

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS); //RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.openWritingPipe(pipeA); 
  radio.setChannel(120); //2400 + 120 = 2520MHz 76 default
  
  pinMode(buttonA5, INPUT);
  pinMode(buttonA4, INPUT);
  pinMode(buttonA0, INPUT);
  pinMode(buttonA1, INPUT);
  pinMode(buttonA2, INPUT);
  pinMode(buttonA3, INPUT);

  pinMode(ledD2, OUTPUT);
  pinMode(ledD3, OUTPUT);
  pinMode(ledD7, OUTPUT);
  pinMode(ledD8, OUTPUT);
  digitalWrite(ledD7, HIGH);

  servo5.attach(5);
  servo6.attach(6);
  servo9.attach(9);
  servo10.attach(10);
}

// Fonction de contrôle des servos avec vitesse ajustable
void controlServo(Servo& servo, int buttonState1, int buttonState2, int speed) {
  if (buttonState1 == HIGH) {
    // Vitesse dans une direction (par exemple, 1600 µs pour rotation dans un sens)
    servo.writeMicroseconds(1500 + speed);
  } else if (buttonState2 == HIGH) {
    // Vitesse dans l'autre direction (par exemple, 1400 µs pour rotation dans l'autre sens)
    servo.writeMicroseconds(1500 - speed);
  } else {
    // Arrêt du servo (1500 µs = position neutre, pas de rotation)
    servo.writeMicroseconds(1500);
  }
}

void loop() {
  
  buttonStateA5 = digitalRead(buttonA5);
  buttonStateA4 = digitalRead(buttonA4);
  buttonStateA0 = digitalRead(buttonA0);
  buttonStateA1 = digitalRead(buttonA1);
  buttonStateA2 = digitalRead(buttonA2);
  buttonStateA3 = digitalRead(buttonA3);

  if (buttonStateA0 == HIGH) {
     digitalWrite(ledD2, LOW);
     digitalWrite(ledD3, LOW);
     digitalWrite(ledD7, HIGH);
     digitalWrite(ledD8, LOW);
     msg[1] = 5;
     msg[2] = 7;
  } else if (buttonStateA1 == HIGH) {
     digitalWrite(ledD2, LOW);
     digitalWrite(ledD3, LOW);
     digitalWrite(ledD7, LOW);
     digitalWrite(ledD8, HIGH);
     msg[1] = 6;
     msg[2] = 8;
  } else if (buttonStateA2 == HIGH) {
     digitalWrite(ledD2, LOW);
     digitalWrite(ledD3, HIGH);
     digitalWrite(ledD7, LOW);
     digitalWrite(ledD8, LOW);
     msg[1] = 10;
     msg[2] = 3;
  } else if (buttonStateA3 == HIGH) {
     digitalWrite(ledD2, HIGH);
     digitalWrite(ledD3, LOW);
     digitalWrite(ledD7, LOW);
     digitalWrite(ledD8, LOW);
     msg[1] = 9;
     msg[2] = 2;
  }

  if (buttonStateA4 == HIGH) {
     msg[3] = 0;
  } else if (buttonStateA5 == HIGH) {
     msg[3] = 1;
  } else {
     msg[3] = 2;
  }  

  // Contrôle des servos en fonction des LED allumées et des boutons
  if (digitalRead(ledD7) == HIGH) {
    controlServo(servo5, buttonStateA5, buttonStateA4, 500);
    radio.write(&msg, sizeof(msg));
  } else if (digitalRead(ledD8) == HIGH) {
    controlServo(servo6, buttonStateA5, buttonStateA4, 500);
    radio.write(&msg, sizeof(msg));
  } else if (digitalRead(ledD3) == HIGH) {
    controlServo(servo10, buttonStateA5, buttonStateA4, 500);
    radio.write(&msg, sizeof(msg));
  } else if (digitalRead(ledD2) == HIGH) {
    controlServo(servo9, buttonStateA5, buttonStateA4, 100);
    radio.write(&msg, sizeof(msg));
  }

  delay(delayTime);  // Attendre un court délai pour stabiliser le mouvement
}