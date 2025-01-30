#include <RF24.h>
#include <Servo.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSerif9pt7b.h>

#define nombreDePixelsEnLargeur   128        
#define nombreDePixelsEnHauteur   64        
#define brocheResetOLED           -1          
#define adresseI2CecranOLED       0x3C       
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);                                                                                                      

RF24 radio(4, 12);
const uint64_t pipeA = 0xE8E8F0F0E1LL;     
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
Servo servoList[] = {servo5, servo6, servo10, servo9};
const int servoPins[] = {5, 6, 10, 9};

int currentLed = 0; 
int buttonState = LOW;
int lastButtonState = LOW;

int speed = 200; 
int delayTime = 20;  

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS); //RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.openWritingPipe(pipeA); 
  radio.setChannel(120); //2400 + 120 = 2520MHz 76 default
  
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
  ecranOLED.setCursor(20, 15);            
  ecranOLED.print("Mode Radio");  

  ecranOLED.setCursor(25, 40);            
  ecranOLED.print("Pedro TX");   

  ecranOLED.display();   
}

void loop() {
  buttonStateA0 = digitalRead(buttonA0);
  buttonStateA1 = digitalRead(buttonA1);
  buttonStateA2 = digitalRead(buttonA2);

  if (buttonStateA0 == LOW && lastButtonState == HIGH) {
    msg[1] = currentLed;
    digitalWrite(ledPins[currentLed], LOW);
    currentLed = (currentLed + 1) % 4; 
    digitalWrite(ledPins[currentLed], HIGH);
    msg[2] = currentLed;
    delay(200);
  }

  lastButtonState = buttonStateA0;
      
  if (buttonStateA1 == HIGH) {
    servoList[currentLed].writeMicroseconds(1500 + speed);
    msg[3] = 10;
  } else if (buttonStateA2 == HIGH) {
    servoList[currentLed].writeMicroseconds(1500 - speed);
    msg[3] = 20;
  } else {
    servoList[currentLed].writeMicroseconds(1500);
    msg[3] = 2;
  }

  radio.write(&msg, sizeof(msg));

  delay(delayTime);
}
