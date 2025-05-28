#include "ModeRadio.h"
#include "Config.h"
//#include <RF24.h>

const uint64_t pipeA = 0xE8E8F0F0E1LL;

ModeRadio::ModeRadio() {
  Serial.println("INIT RADIO");
  /*
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(120);
  */

  m_radioType = 1;
}



void ModeRadio::updateRadioMode() {
  /*if (m_radioType == 1) {
      // Mode émetteur
      radio.stopListening();
      radio.openWritingPipe(pipeA);
  } else if (m_radioType == 2) {
      // Mode récepteur
      radio.openReadingPipe(1, pipeA);
      radio.startListening();
  }*/
  m_radioType = 3;
}



void ModeRadio::communicationRadio(ManageState * manageState, RF24 * radio) {
  m_radioType = manageState->getRadioType();
 // updateRadioMode();
  if (m_radioType == 1) {
    Serial.println("INIT TX");
      // Mode émetteur
      radio->stopListening();
      radio->openWritingPipe(pipeA);
  } else if (m_radioType == 2) {
    Serial.println("INIT RX");
      // Mode récepteur
      radio->openReadingPipe(1, pipeA);
      radio->startListening();
  }
  m_radioType = 3;
}