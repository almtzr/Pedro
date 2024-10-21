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

int servoID,LEDID,rotation;

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
  radio.setDataRate(RF24_2MBPS);  //RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.openReadingPipe(1,pipeA);
  radio.setChannel(120); //2400 + 120 = 2520MHz 76 default
  radio.startListening();
  
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

void loop() {

   if (radio.available() > 0) {
      radio.read(msg, sizeof(msg));
      ledState = HIGH;
      digitalWrite(ledD11, ledState);
      servoID = msg[1];  // servo number 5,6,9,10
      LEDID = msg[2];  // LED 7,8,3,2
      rotation = msg[3];  // btn left or right 0 or 1
      Serial.print ("servoID: ");
      Serial.print (servoID);
      Serial.print (" LED: ");
      Serial.print (LEDID); 
      Serial.print (" rotation: ");
      Serial.println (rotation);
      pedro (servoID, rotation, LEDID);
    } else {
      ledState = LOW;
      digitalWrite(ledD11, ledState);
      Serial.println ("no signal");   
    }
}
    
void pedro(int servoID, int rotDir, int led) {
  
  buttonStateA5 = digitalRead(buttonA5);
  buttonStateA4 = digitalRead(buttonA4);
  buttonStateA0 = digitalRead(buttonA0);
  buttonStateA1 = digitalRead(buttonA1);
  buttonStateA2 = digitalRead(buttonA2);
  buttonStateA3 = digitalRead(buttonA3);

  if (led == 7) {
     digitalWrite(ledD2, LOW);
     digitalWrite(ledD3, LOW);
     digitalWrite(ledD7, HIGH);
     digitalWrite(ledD8, LOW);
  } else if (led == 8) {
     digitalWrite(ledD2, LOW);
     digitalWrite(ledD3, LOW);
     digitalWrite(ledD7, LOW);
     digitalWrite(ledD8, HIGH);
  } else if (led == 3) {
     digitalWrite(ledD2, LOW);
     digitalWrite(ledD3, HIGH);
     digitalWrite(ledD7, LOW);
     digitalWrite(ledD8, LOW);
  } else if (led == 2) {
     digitalWrite(ledD2, HIGH);
     digitalWrite(ledD3, LOW);
     digitalWrite(ledD7, LOW);
     digitalWrite(ledD8, LOW);
  }

  // Contrôle des servos en fonction des LED allumées et des boutons
  if (servoID == 5) {
    speed = 40;
    controlServo(servo5, rotDir, speed);
  } else if (servoID == 6) {
    speed = 500;
    controlServo(servo6, rotDir, speed);
  } else if (servoID == 10) {
    speed = 500;
    controlServo(servo10, rotDir, speed);
  } else if (servoID == 9) {
    speed = 100;
    controlServo(servo9, rotDir, speed);
  }

  delay(delayTime);  // Attendre un court délai pour stabiliser le mouvement
}

// Fonction de contrôle des servos avec vitesse ajustable
void controlServo(Servo& servo, int val, int speed) {
  if (val == 0) {
    servo.writeMicroseconds(1500 + speed);
  } else if (val == 1) {
    servo.writeMicroseconds(1500 - speed);
  } else if (val == 2) {
    servo.writeMicroseconds(1500);
  }
}