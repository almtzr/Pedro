#ifndef __MANAGESTATE_H__
#define __MANAGESTATE_H__

#include <Arduino.h>
#include "Config.h"
#include "ManageDisplay.h"
#include "ManageButton.h"

extern const uint8_t ledPins[4];
extern const uint8_t servoPins[4];
extern const int servoSpeed[4];
extern Servo servoList[4];

class ManageState {
  public:
    ManageState ();
    void Init ();
    void updateState (ManageButton * manageButton, ManageDisplay * manageDisplay);
    void screenIntro ();
    void screenControl ();
    void screenSelectMode ();
    void screenRadio ();
    void replayMovements ();
    uint8_t getRadioType ();

  private:
    uint8_t m_currentScreen, m_selectMode, m_currentLed, m_movementIndex, m_radioType;
    int m_intervalIntro, m_lastPulse;
    unsigned long m_previousMillis, m_lastChange;
    bool m_isPressBtnRight, m_isBtnRightOn;
    bool m_isPressBtnLeft, m_isBtnLeftOn;
    bool m_isBtnCenterPressed, m_isBtnCenterLongPress, m_isBtnCenterReleased;
    bool m_startRecord, m_fromSelectMode;
};



#endif