#include "ManageState.h"
#include "Config.h"

ManageDisplay * m_manageDisplay;
ManageButton * m_manageButton;

struct Movement {
  int pulse;
  unsigned long duration;
  uint8_t servo;
};

Movement movements[100];

const uint8_t ledPins[4] = {13, 11, 8, 7};
const uint8_t servoPins[4] = {5, 6, 9, 10}; 

// servoSpeed => Can be adjusted based on your Pedro Robot's movement. 
// Higher values result in faster movement, lower values result in slower movement.
const int servoSpeed[4] = {-150, -150, 150, 100};
////////////

Servo servoList[4];

ManageState::ManageState() {
    m_currentScreen = 0;
    m_previousMillis = 0;
    m_intervalIntro = 3000;
    m_lastPulse = 1500;
    m_selectMode = 1;
    m_currentLed = 0;
    m_lastChange = 0;
    m_radioType = 1;
    m_movementIndex = 0;
    m_isPressBtnRight = false;
    m_isPressBtnLeft = false;
    m_isBtnCenterReleased = false;
    m_isBtnCenterPressed = false;
    m_isBtnCenterLongPress = false;
    m_isBtnRightOn = false; 
    m_isBtnLeftOn = false; 
    m_startRecord = true;
    m_fromSelectMode = false;
}

void ManageState::Init() {  
    pinMode(BTNLEFT, INPUT);
    pinMode(BTNCENTER, INPUT);
    pinMode(BTNRIGHT, INPUT);
    for (uint8_t i = 0; i < 4; i++) {
       pinMode(ledPins[i], OUTPUT);
       digitalWrite(ledPins[i], LOW);
       servoList[i].attach(servoPins[i]);
    }
    digitalWrite(ledPins[0], HIGH);
}

uint8_t ManageState::getRadioType() { return m_radioType; }

void ManageState::updateState(ManageButton * manageButton, ManageDisplay * manageDisplay) {  
    m_manageButton = manageButton;
    m_manageDisplay = manageDisplay;
    m_isPressBtnRight = m_manageButton->getBtnRightState ();
    m_isPressBtnLeft = m_manageButton->getBtnLeftState ();
    m_isBtnCenterReleased = m_manageButton->getBtnCenterReleased ();
    m_isBtnCenterPressed = m_manageButton->getBtnCenterPressed ();
    m_isBtnCenterLongPress = m_manageButton->getBtnCenterLongPress ();
    m_currentScreen = m_manageDisplay->getScreen ();
    m_radioType = m_manageDisplay->getRadioType();

    if (m_currentScreen == 0) {
        screenIntro();
    } else if (m_currentScreen == 1) {
        if (m_fromSelectMode) {
            m_manageButton->setBtnCenterReleased(false);
            m_currentLed = -1;
            digitalWrite(ledPins[m_currentLed], HIGH);
        }
        screenControl ();
    } else if (m_currentScreen == 2) {
        screenSelectMode ();
    } else if (m_currentScreen == 3) {
        screenRadio ();
    }     
}

void ManageState::screenRadio() {  
    if (m_isBtnCenterReleased) {
        m_manageButton->setBtnCenterReleased(false);
        if (m_radioType == 1){
            m_manageDisplay->setRadioType(2);
        } else if (m_radioType == 2){
            m_manageDisplay->setRadioType(1);
        } 
    }
}

void ManageState::screenIntro() {  
    unsigned long currentMillis = millis();
    if (currentMillis - m_previousMillis >= m_intervalIntro) {
        m_previousMillis = currentMillis;
        m_manageDisplay->setDisplayScreen (SCREEN_NORMAL);
    }  
}

void ManageState::screenControl() {  

    int pulse = 1500; // neutre

    if (m_isBtnCenterLongPress) {
        m_manageButton->setBtnCenterLongPress(false);
        m_manageButton->setBtnCenterPressed(false);
        m_manageDisplay->setDisplayScreen (SCREEN_SETTINGS);
        
    } else {
        if (m_isBtnCenterReleased and not m_fromSelectMode) {
            digitalWrite(ledPins[m_currentLed], LOW);
            m_currentLed = (m_currentLed + 1) % 4;
            digitalWrite(ledPins[m_currentLed], HIGH);
            m_manageButton->setBtnCenterReleased(false);
            delay(200);
        }

        if (m_fromSelectMode) {
            m_fromSelectMode = false;
        }

        if (m_isPressBtnRight) {
            pulse = 1500 + servoSpeed[m_currentLed];
        } else if (m_isPressBtnLeft) {
            pulse = 1500 - servoSpeed[m_currentLed];
        } 
    
        if (m_selectMode == 2) {   
          if (m_startRecord) {
            m_movementIndex = 0;
            m_lastChange = millis();
            m_startRecord = false;
          }
          if (abs(pulse - m_lastPulse) > 5) {
            unsigned long now = millis();
            if (m_movementIndex < 100) {
              movements[m_movementIndex++] = {m_lastPulse, now - m_lastChange, m_currentLed};
            }
            m_lastChange = now;
            m_lastPulse = pulse;
          }
        } else if (m_selectMode == 3) {
          do {
            replayMovements();
          } while (not digitalRead(BTNCENTER));
        } else if (m_selectMode == 4) {
          
            
        }
    }

    servoList[m_currentLed].writeMicroseconds(pulse);

}


void ManageState::replayMovements() {
  

    // Mettre tous les servos à neutre avant de commencer
    for (uint8_t i = 0; i < 4; i++) {
      servoList[i].writeMicroseconds(1500);
      digitalWrite(ledPins[i], LOW);
    }
  
    // Lecture séquentielle des mouvements
    for (uint8_t i = 0; i < m_movementIndex; i++) {
      uint8_t currentServo = movements[i].servo;
  
      // Activer uniquement le servo concerné
      for (uint8_t j = 0; j < 4; j++) {
        if (j == currentServo) {
          digitalWrite(ledPins[j], HIGH); // Allumer la LED correspondante
        } else {
          digitalWrite(ledPins[j], LOW);  // Éteindre les autres
          servoList[j].writeMicroseconds(1500); // Stopper les autres servos
        }
      }
  
      // Exécuter le mouvement
      servoList[currentServo].writeMicroseconds(movements[i].pulse);
      delay(movements[i].duration);
  
      // Remettre le servo à neutre après le mouvement
      servoList[currentServo].writeMicroseconds(1500);
    }
  
    // Éteindre toutes les LEDs à la fin
    for (uint8_t i = 0; i < 4; i++) {
      digitalWrite(ledPins[i], LOW);
      servoList[i].writeMicroseconds(1500);
    }
}

void ManageState::screenSelectMode() {  
    for (uint8_t i = 0; i < 4; i++) {
       digitalWrite(ledPins[i], LOW);
    }

    if (m_isBtnCenterPressed) {
        if (m_selectMode == 1) {
            m_manageDisplay->setModeActivated(MODE_CONTROL);
            m_manageDisplay->setDisplayScreen (SCREEN_NORMAL);
        } else if (m_selectMode == 2) {
            m_startRecord = true;
            m_manageDisplay->setModeActivated(MODE_RECORD);
            m_manageDisplay->setDisplayScreen (SCREEN_NORMAL);
        } else if (m_selectMode == 3) {
            m_manageDisplay->setModeActivated(MODE_REPEAT);
            m_manageDisplay->setDisplayScreen (SCREEN_NORMAL);
        } else if (m_selectMode == 4) {
            m_manageDisplay->setDisplayScreen (SCREEN_RADIO);
        } else if (m_selectMode == 5) {
            m_manageDisplay->setModeActivated(MODE_USB);
        } else if (m_selectMode == 6) {
            m_manageDisplay->setModeActivated(MODE_BLUETOOTH);
        }
        m_fromSelectMode = true;
        m_manageButton->setBtnCenterPressed(false);
        m_manageButton->setBtnCenterReleased(false);
    }

    if (m_isPressBtnRight) {
        if (not m_isBtnRightOn) {
            if (m_selectMode < 6) {
                m_selectMode++;
            }
            m_isBtnRightOn = true;
        }   
    } else {
        m_isBtnRightOn = false;
    }
    if (m_isPressBtnLeft) {
        if (not m_isBtnLeftOn) {
            if (m_selectMode > 1) {
                m_selectMode--;
            }
            m_isBtnLeftOn = true;
        }   
    } else {
        m_isBtnLeftOn = false;
    }

    m_manageDisplay->setModeSelected (m_selectMode);

}