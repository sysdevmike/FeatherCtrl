#include "LED.h"


LED::LED(void) {
  
}

void LED::begin(void) {
  powerButtonOn();
}

void LED::powerButtonOn(void) {
  analogWrite(LED::POWER_BUTTON_PIN, 255 - LED::powerButtonBrightness);  
}
