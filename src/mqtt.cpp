#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "config.h"
#include "wifiCommunication.h"
#include "mqtt.h"
#include "keyboard.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

// https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
// https://github.com/knolleary/pubsubclient
// https://gist.github.com/igrr/7f7e7973366fc01d6393

unsigned long reconnectInterval = 5000;
// in order to do reconnect immediately ...
unsigned long lastReconnectAttempt = millis() - reconnectInterval - 1;
unsigned long timerStartForHAdiscovery = 1;

void callback(char* topic, byte* payload, unsigned int length);

WiFiClient wifiClient;

PubSubClient mqttClient(MQTT_SERVER, MQTT_SERVER_PORT, callback, wifiClient);

bool checkMQTTconnection();

void mqtt_setup() {
  // Set bigger buffer size if needed
  // mqttClient.setBufferSize(1280);
}

void mqtt_loop(){
  if (!mqttClient.connected()) {
    unsigned long currentMillis = millis();
    if ((currentMillis - lastReconnectAttempt) > reconnectInterval) {
      lastReconnectAttempt = currentMillis;
      // Attempt to reconnect
      checkMQTTconnection();
    }
  }  

  if (mqttClient.connected()) {
    mqttClient.loop();
  }
}

bool checkMQTTconnection() {
  if (wifiIsDisabled) return false;

  if (WiFi.isConnected()) {
    if (mqttClient.connected()) {
      return true;
    } else {
      // try to connect to mqtt server
      if (mqttClient.connect(MQTT_CLIENTNAME, MQTT_USER, MQTT_PASS)) {
        Serial.printf("  Successfully connected to MQTT broker\r\n");
    
        // subscribes to messages with given topic.
        // Callback function will be called 1. in client.loop() 2. when sending a message
        mqttClient.subscribe(MQTTCMND_UP);
        mqttClient.subscribe(MQTTCMND_DOWN);
        mqttClient.subscribe(MQTTCMND_RIGHT);
        mqttClient.subscribe(MQTTCMND_LEFT);
        mqttClient.subscribe(MQTTCMND_SELECT);

        mqttClient.subscribe(MQTTCMND_BACK);
        mqttClient.subscribe(MQTTCMND_HOME);
        mqttClient.subscribe(MQTTCMND_MENU);

        mqttClient.subscribe(MQTTCMND_SCAN_PREVIOUS_TRACK);
        mqttClient.subscribe(MQTTCMND_REWIND_LONG);
        mqttClient.subscribe(MQTTCMND_REWIND);
        mqttClient.subscribe(MQTTCMND_PLAYPAUSE);
        mqttClient.subscribe(MQTTCMND_FASTFORWARD);
        mqttClient.subscribe(MQTTCMND_FASTFORWARD_LONG);
        mqttClient.subscribe(MQTTCMND_SCAN_NEXT_TRACK);
        mqttClient.subscribe(MQTTCMND_SENDSTRING);

        mqttClient.subscribe(MQTTCMND_RESTART_ESP32);
      } else {
        Serial.printf("  MQTT connection failed (but WiFi is available). Will try later ...\r\n");
      }
      return mqttClient.connected();
    }
  } else {
    Serial.printf("  No connection to MQTT server, because WiFi ist not connected.\r\n");
    return false;
  }  
}

bool publishMQTTMessage(const char *topic, const char *payload, boolean retained){
  if (wifiIsDisabled) return false;

  if (checkMQTTconnection()) {
    // Serial.printf("Sending mqtt payload to topic \"%s\": %s\r\n", topic, payload);
      
    if (mqttClient.publish(topic, payload, retained)) {
      // Serial.printf("Publish ok\r\n");
      return true;
    }
    else {
      Serial.printf("Publish failed\r\n");
    }
  } else {
    Serial.printf("  Cannot publish mqtt message, because checkMQTTconnection failed (WiFi or mqtt is not connected)\r\n");
  }
  return false;
}

bool publishMQTTMessage(const char *topic, const char *payload){
  return publishMQTTMessage(topic, payload, false);
}

bool mqtt_publish_tele() {
  bool error = false;
  // maximum message length 128 Byte
  String payload = "";

  // WiFi
  payload = "";
  payload += "{\"wifiRSSI\":";
  payload += WiFi.RSSI();
  payload += ",\"wifiChan\":";
  payload += WiFi.channel();
  payload += ",\"wifiSSID\":";
  payload += WiFi.SSID();
  payload += ",\"wifiBSSID\":";
  payload += WiFi.BSSIDstr();
  payload += ",\"IP\":";
  payload += WiFi.localIP().toString();
  payload += "}";
  error = error || !publishMQTTMessage(MQTTTELESTATE1, payload.c_str());

  // ESP32 stats
  payload = "";
  payload += "{\"up\":";
  payload += String(millis());
  payload += ",\"heapSize\":";
  payload += String(ESP.getHeapSize());
  payload += ",\"heapFree\":";
  payload += String(ESP.getFreeHeap());
  payload += ",\"heapMin\":";
  payload += String(ESP.getMinFreeHeap());
  payload += ",\"heapMax\":";
  payload += String(ESP.getMaxAllocHeap());
  payload += "}";
  error = error || !publishMQTTMessage(MQTTTELESTATE2, payload.c_str());

  return !error;
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  std::string strPayload(reinterpret_cast<const char *>(payload), length);

  // Serial.printf("MQTT message arrived [%s] %s\r\n", topic, strPayload.c_str());

  bool doLog = false;

  // for more keycodes see
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/USB/src/USBHIDKeyboard.h
  // or here (please consider offset of values! For this please examine keys which are both in the arduino-esp32 library and the Adafruit CircuitPython library)
  // https://github.com/adafruit/Adafruit_CircuitPython_HID/blob/main/adafruit_hid/keycode.py
  if (strcmp(MQTTCMND_UP, topic) == 0) {
    if (doLog) {Serial.printf("UP received\r\n");}
    keyboard_write(KEY_UP_ARROW);

  } else if (strcmp(MQTTCMND_DOWN, topic) == 0) {
    if (doLog) {Serial.printf("DOWN received\r\n");}
    keyboard_write(KEY_DOWN_ARROW);

  } else if (strcmp(MQTTCMND_RIGHT, topic) == 0) {
    if (doLog) {Serial.printf("RIGHT received\r\n");}
    keyboard_write(KEY_RIGHT_ARROW);

  } else if (strcmp(MQTTCMND_LEFT, topic) == 0) {
    if (doLog) {Serial.printf("LEFT received\r\n");}
    keyboard_write(KEY_LEFT_ARROW);

  } else if (strcmp(MQTTCMND_SELECT, topic) == 0) {
    if (doLog) {Serial.printf("SELECT received\r\n");}
    keyboard_write(KEY_RETURN);



  } else if (strcmp(MQTTCMND_BACK, topic) == 0) {
    if (doLog) {Serial.printf("BACK received\r\n");}
    keyboard_write(KEY_ESC);

  } else if (strcmp(MQTTCMND_HOME, topic) == 0) {
    if (doLog) {Serial.printf("HOME received\r\n");}
    keyboard_home();

  } else if (strcmp(MQTTCMND_MENU, topic) == 0) {
    if (doLog) {Serial.printf("MENU received\r\n");}
    keyboard_write(0xED); // 0xDA + 13 = 0xED



  // for more consumerControl codes see
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/USB/src/USBHIDConsumerControl.h
  // https://github.com/adafruit/Adafruit_CircuitPython_HID/blob/main/adafruit_hid/consumer_control_code.py
  } else if (strcmp(MQTTCMND_SCAN_PREVIOUS_TRACK, topic) == 0) {
    if (doLog) {Serial.printf("SCAN_PREVIOUS_TRACK received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_SCAN_PREVIOUS);

  } else if (strcmp(MQTTCMND_REWIND_LONG, topic) == 0) {
    if (doLog) {Serial.printf("REWIND_LONG received\r\n");}
    consumerControl_longpress(0x00B4);

  } else if (strcmp(MQTTCMND_REWIND, topic) == 0) {
    if (doLog) {Serial.printf("REWIND received\r\n");}
    consumerControl_write(0x00B4);

  } else if (strcmp(MQTTCMND_PLAYPAUSE, topic) == 0) {
    if (doLog) {Serial.printf("PLAYPAUSE received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_PLAY_PAUSE);

  } else if (strcmp(MQTTCMND_FASTFORWARD, topic) == 0) {
    if (doLog) {Serial.printf("FASTFORWARD received\r\n");}
    consumerControl_write(0x00B3);
    
  } else if (strcmp(MQTTCMND_FASTFORWARD_LONG, topic) == 0) {
    if (doLog) {Serial.printf("FASTFORWARD_LONG received\r\n");}
    consumerControl_longpress(0x00B3);
    
  } else if (strcmp(MQTTCMND_SCAN_NEXT_TRACK, topic) == 0) {
    if (doLog) {Serial.printf("SCAN_NEXT_TRACK received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_SCAN_NEXT);



  } else if (strcmp(MQTTCMND_SENDSTRING, topic) == 0) {
    if (doLog) {Serial.printf("SENDSTRING received\r\n");}
    if (strPayload != "") {
      keyboard_sendString(strPayload.c_str());
    }



  } else if (strcmp(MQTTCMND_RESTART_ESP32, topic) == 0) {
    if (doLog) {Serial.printf("RESTART_ESP32 received\r\n");}
    ESP.restart();

  }

}
