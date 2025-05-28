#ifndef __MANAGEBUTTON_H__
#define __MANAGEBUTTON_H__

#include <Arduino.h>
#include "Config.h"


class ManageButton {
  public:
    ManageButton ();
    bool getBtnRightState ();
    bool getBtnLeftState ();
    bool getBtnCenterPressed ();
    bool getBtnCenterReleased ();
    bool getBtnCenterLongPress ();
    void setBtnCenterReleased(bool released);
    void setBtnCenterPressed(bool pressed);
    void setBtnCenterLongPress(bool longPress);
    void updateBtnState ();
  private:
    bool m_isBtnRightOn, m_isBtnLeftOn;
    bool m_lastManageButton, m_btnCenterLongPress;
    bool m_currentManageButton;
    unsigned long m_lastDebounceTime, m_pressStartTime;
    bool m_stateBtnCenter, m_btnCenterPressed, m_btnCenterReleased, m_buttonHeld;
};



#endif