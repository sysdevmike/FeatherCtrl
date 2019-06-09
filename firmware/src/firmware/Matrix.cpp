#include "Matrix.h"
#include "MCP23S17.h"

#include <Arduino.h>

const uint8_t Matrix::rowPins[] = {
  4, 3, 2, 16, 15, 7, 11
};

const uint8_t Matrix::colPins[] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

MCP mcp(0, 27); 

Matrix::Matrix(void) {
  memset(keys, 0, sizeof(keys));
}

void Matrix::begin(void) {
  mcp.begin();
  
  for (auto c = 0; c < (int)Matrix::Dim::Col; c++) {
    mcp.pinMode(colPins[c], OUTPUT);
    mcp.digitalWrite(colPins[c], HIGH);
  }
  
  for (auto r = 0; r < (int)Matrix::Dim::Row; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }

}

bool Matrix::debounce_tick(const int row, const int col, const bool pressed) {
  debounce_status_t *status = &(debounce[row][col]);
  switch (status->state) {
    case STATE_NOT_PRESSED:
    if (!pressed) {
      return false;
    }
    // state transition from not pressed to pressed
    status->state = STATE_PRESSED_BOUNCING;
    status->last_millis = millis();
    return true;

    case STATE_PRESSED_BOUNCING:
      if ((millis() - status->last_millis) < debounceTime) {
        return false; // key bouncing, ignore
      }
      status->state = STATE_PRESSED;
      return false;

    case STATE_PRESSED:
    if (pressed) {
      return false;
    }
    // state transition from pressed to not pressed
    status->state = STATE_RELEASING_BOUNCING;
    status->last_millis = millis();
    return true;

    case STATE_RELEASING_BOUNCING:
    if ((millis() - status->last_millis) < debounceTime) {
      return false; // key bouncing, ignore
    }
    status->state = STATE_NOT_PRESSED;
    return false;
  }

  return false; // unreached
}


bool Matrix::scan(void) {
  auto scanTime = millis();

  bool update = false;

  for (auto c = 0; c < (int)Matrix::Dim::Col; c++) {

    mcp.digitalWrite(colPins[c], LOW);
  
    for (auto r = 0; r < (int)Matrix::Dim::Row; r++) {
      auto pressed = digitalRead(rowPins[r]) == LOW;

//      if (!debounce_tick(r, c, pressed)) {
//        continue; // not pressed
//      }

      bool updated = debounce_tick(r, c, pressed);
      if(!update) {
        update = updated;
      }
      
//      auto *keyOld = &keys[r][c];
//      KeyState keyNew = { .pressTime = scanTime, .pressed = pressed };
//
//      if (keyNew.pressed && !keyOld->pressed) {
//        *keyOld = keyNew;
//        update = true;
//      } else if (!keyNew.pressed && keyOld->pressed) {
//        if (keyNew.pressTime - keyOld->pressTime > debounceTime) {
//          keyOld->pressed = false;
//          update = true;
//        }
//      }

    }
  
    mcp.digitalWrite(colPins[c], HIGH);
  }

  return update;
}

bool Matrix::pressed(const Matrix::Key k) const {
  bool isPressed = debounce[k.r][k.c].state == STATE_PRESSED || debounce[k.r][k.c].state == STATE_PRESSED_BOUNCING;
//  if(isPressed) {
//    Serial.println("row:");
//    Serial.println(k.r);
//    Serial.println("col:");
//    Serial.println(k.c);
//  }
  return isPressed;
//  return debounce[k.r][k.c].state == STATE_PRESSED || debounce[k.r][k.c].state == STATE_PRESSED_BOUNCING;
//  return keys[k.r][k.c].pressed;
}

void Matrix::sleep(void) {
  for (auto c = 0; c < (int)Matrix::Dim::Col; c++) {
    mcp.digitalWrite(colPins[c], LOW);
  }

  for (auto r = 0; r < (int)Matrix::Dim::Row; r++) {
      nrf_gpio_cfg_sense_input(g_ADigitalPinMap[rowPins[r]], NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
  }
}
