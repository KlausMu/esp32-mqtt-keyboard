/*
  Before changing anything in this file, consider to copy file "config_override_example.h" to file "config_override.h" and to do your changes there.
  Doing so, you will
  - keep your credentials secret
  - most likely never have conflicts with new versions of this file
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define MY_LOG_FORMAT(format) "%lu ms: " format, millis()

// --- wifi ---------------------------------------------------------------------------------------------------------------------------------

#define WIFI_SSID                    "YourWifiSSID"             // override it in file "config_override.h"
#define WIFI_PASSWORD                "YourWifiPassword"         // override it in file "config_override.h"

// --- mqtt ---------------------------------------------------------------------------------------------------------------------------------
#define UNIQUE_DEVICE_NAME           "esp32_keyboard"           // override it in file "config_override.h"
#define MQTT_SERVER                  "IPAddressOfYourBroker"    // override it in file "config_override.h"
#define MQTT_SERVER_PORT             1883                       // override it in file "config_override.h"
#define MQTT_USER                    ""                         // override it in file "config_override.h"
#define MQTT_PASS                    ""                         // override it in file "config_override.h"
#define MQTT_CLIENTNAME              UNIQUE_DEVICE_NAME

#define MQTTCMND_RESTART_ESP32       UNIQUE_DEVICE_NAME "/cmnd/RESTART_ESP32"

#define MQTTTELESTATE1               UNIQUE_DEVICE_NAME "/tele/STATE1"
#define MQTTTELESTATE2               UNIQUE_DEVICE_NAME "/tele/STATE2"

// --- include override settings from seperate file ---------------------------------------------------------------------------------------------------------------
#if __has_include("config_override.h")
  #include "config_override.h"
#endif

#endif /*__CONFIG_H__*/
