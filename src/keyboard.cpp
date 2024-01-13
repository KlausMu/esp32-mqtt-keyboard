#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

void keyboard_setup() {

  // initialize control over the keyboard:
  USB.begin();
  Keyboard.begin();
}

void keyboard_write(uint8_t c) {
  Keyboard.write(c);
}

void keyboard_home() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
}

void keyboard_sendString(const String &s) {
  Keyboard.print(s);
}

void consumerControl_write(uint8_t value) {
  ConsumerControl.press(value);
  ConsumerControl.release();
}

void consumerControl_longpress(uint8_t value) {
  ConsumerControl.press(value);
  delay(1000);
  ConsumerControl.release();
}

