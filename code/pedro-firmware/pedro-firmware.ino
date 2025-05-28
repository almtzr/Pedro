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

#include <RF24.h>

#include <ManageDisplay.h>
#include <ManageButton.h>
#include <ManageState.h>
#include <ModeRadio.h>

ManageDisplay manageDisplay;
ManageButton manageButton;
ManageState manageState;
ModeRadio modeRadio;

RF24 radio(4, 12);
const uint64_t pipeA = 0xE8E8F0F0E1LL;

void setup() {
  Serial.begin(9600);  
  
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS);  //RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.openReadingPipe(1,pipeA);
  radio.setChannel(120); //2400 + 120 = 2520MHz 76 default
  radio.startListening();
  
  manageState.Init ();

}

void loop() {
  manageButton.updateBtnState ();
  manageDisplay.updateScreen ();
  manageState.updateState (&manageButton, &manageDisplay);
  modeRadio.communicationRadio (&manageState, &radio);
}
