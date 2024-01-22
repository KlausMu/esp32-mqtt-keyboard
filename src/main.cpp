#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "wifiCommunication.h"
#include "mqtt.h"
#include "keyboard.h"

#if defined(useOTAUpdate)
  // https://github.com/SensorsIot/ESP32-OTA
  #include "OTA.h"
  #if !defined(useOTA_RTOS)
    #include <ArduinoOTA.h>
  #endif
#endif

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

  // Sometimes initial WiFi connection fails and never recovers. In that case, restart ESP
  if ((currentMillis > 5000) && !WiFi.isConnected()) {
    ESP.restart();
  }

  #if defined(useOTAUpdate)
  if ((!OTAstarted) && WiFi.isConnected()) {
    OTA_setup("ESP32MQTTKEYBOARD");
    ArduinoOTA.begin();
    OTAstarted = true;
  }
  #endif
  #if defined(useOTAUpdate) && !defined(useOTA_RTOS)
  // If you do not use FreeRTOS, you have to regulary call the handle method
  ArduinoOTA.handle();
  #endif
}

#endif /* ARDUINO_USB_MODE */
