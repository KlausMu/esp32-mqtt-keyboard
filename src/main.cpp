#include <Arduino.h>

#include "wifiCommunication.h"
#include "mqtt.h"
#include "keyboard.h"

#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup(){}
void loop(){}
#else

unsigned long previousMillis10000Cycle = 0;
unsigned long interval10000Cycle = 10000;

void setup() {
  Serial.begin(115200);

  wifi_setup();
  wifi_enable();

  mqtt_setup();

  keyboard_setup();
}

void loop() {

  mqtt_loop();

  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis10000Cycle) >= interval10000Cycle) {
    previousMillis10000Cycle = currentMillis;
    mqtt_publish_tele();
  }
}
#endif /* ARDUINO_USB_MODE */
