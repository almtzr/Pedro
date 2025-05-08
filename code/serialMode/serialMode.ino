#include <Servo.h>

Servo servos[4];
Servo servo5;
Servo servo6;
Servo servo9;
Servo servo10;

Servo servoList[] = {servo5, servo6, servo9, servo10};
const int servoPins[] = {5, 6, 9, 10};
int positions[4] = {90, 90, 90, 90};  // Position initiale au centre
const int ledPins[] = {13, 11, 8, 7}; 

int speed = 400; 

void setup() {
  Serial.begin(9600);
  servos[0].attach(3);
  servos[1].attach(5);
  servos[2].attach(6);
  servos[3].attach(9);
  for (int i = 0; i < 4; i++) {
     pinMode(ledPins[i], OUTPUT);
     digitalWrite(ledPins[i], LOW);
     servoList[i].attach(servoPins[i]);
  }
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    int servoNum = command.charAt(0) - '1'; // '1' à '4' => index 0 à 3
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
        //positions[servoNum] = max(0, positions[servoNum] - 5);
      } else if (direction == 'R') {
        //positions[servoNum] = min(180, positions[servoNum] + 5);
        servoList[servoNum].writeMicroseconds(1500 - speed);
      } else {
        servoList[servoNum].writeMicroseconds(1500);
        Serial.println ("Idle State");
      }
      //servos[servoNum].write(positions[servoNum]);
    }
  }
}
