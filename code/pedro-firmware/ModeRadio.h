#ifndef __MODERADIO_H__
#define __MODERADIO_H__

#include <Arduino.h>
#include "Config.h"
#include <RF24.h>
#include "ManageState.h"

class ModeRadio {
  public:
    ModeRadio ();
    void updateRadioMode();
    void communicationRadio(ManageState * manageState, RF24 * radio);

  private:
   // RF24 radio = RF24(4, 12); // CE, CSN
    uint8_t m_radioType;
    
};



#endif