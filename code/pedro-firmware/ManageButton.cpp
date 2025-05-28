#include "ManageButton.h"
#include "Config.h"

ManageButton::ManageButton() {
    m_isBtnRightOn = false;
    m_isBtnLeftOn = false;
    m_btnCenterReleased = false;
    m_lastManageButton = LOW;
    m_currentManageButton = LOW;
    m_lastDebounceTime = 0;
    m_pressStartTime = 0;
    m_stateBtnCenter = LOW;
    m_btnCenterLongPress = false;
    m_btnCenterPressed = false;
    m_buttonHeld = false;
}

bool ManageButton::getBtnRightState() { return m_isBtnRightOn; }
bool ManageButton::getBtnLeftState() { return m_isBtnLeftOn; }
bool ManageButton::getBtnCenterReleased() { return m_btnCenterReleased; }
bool ManageButton::getBtnCenterPressed() { return m_btnCenterPressed; }
bool ManageButton::getBtnCenterLongPress() { return m_btnCenterLongPress; }

void ManageButton::setBtnCenterReleased(bool released) { m_btnCenterReleased = released; }
void ManageButton::setBtnCenterLongPress(bool long_press) { m_btnCenterLongPress = long_press; }
void ManageButton::setBtnCenterPressed(bool pressed) { m_btnCenterPressed = pressed; }

void ManageButton::updateBtnState () { 
  m_isBtnRightOn = digitalRead(BTNRIGHT) == HIGH; 
  m_isBtnLeftOn = digitalRead(BTNLEFT) == HIGH; 

  m_stateBtnCenter = digitalRead(BTNCENTER);

  // Si l'état change, on reset le timer de debounce
  if (m_stateBtnCenter != m_lastManageButton) {
    m_lastDebounceTime = millis();
  }

// Après le délai de rebond
if ((millis() - m_lastDebounceTime) > DEBOUNCE_DELAY) {
  // Si l'état réel du bouton a changé
  if (m_stateBtnCenter != m_currentManageButton) {
    m_currentManageButton = m_stateBtnCenter;

    if (m_stateBtnCenter == HIGH) {
      Serial.println("Appui détecté");
      m_btnCenterPressed = true;
      m_buttonHeld = true;
      m_pressStartTime = millis();  // Start measuring hold time
    } else {
      Serial.println("Relâchement détecté");
      m_btnCenterReleased = true;
      // Reset states
      m_buttonHeld = false;
    }
  }

  // Vérification d'un appui long pendant que le bouton est maintenu appuyé
  if (m_buttonHeld && m_stateBtnCenter == HIGH) {
    if ((millis() - m_pressStartTime) >= 3000 && !m_btnCenterLongPress) {
      m_btnCenterLongPress = true;
      Serial.println("Appui long détecté");
      m_buttonHeld = false;
    }
  }
}

m_lastManageButton = m_stateBtnCenter;

}