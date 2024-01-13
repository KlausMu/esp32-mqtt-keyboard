#include <Arduino.h>
#include <WiFi.h>

#include "wifiCommunication.h"
#include "config.h"

boolean connected = false;
bool wifiIsDisabled = true;

void printWiFiStatus(void){
  if (wifiIsDisabled) return;

  if (WiFi.isConnected()) {
    Serial.printf(MY_LOG_FORMAT("  WiFi.status() == connected\r\n"));
  } else {
    Serial.printf(MY_LOG_FORMAT("  WiFi.status() == DIS-connected\r\n"));
  }
  Serial.printf(MY_LOG_FORMAT("  IP address: %s\r\n"), WiFi.localIP().toString().c_str());

  if (WiFi.isConnected()) { //  && WiFi.localIP().isSet()) {
    Serial.printf(MY_LOG_FORMAT("  WiFi connected and IP is set :-)\r\n"));
  } else {
    Serial.printf(MY_LOG_FORMAT("  WiFi not completely available :-(\r\n"));
  }
}
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.printf(MY_LOG_FORMAT("  Callback \"StationConnected\"\r\n"));

  printWiFiStatus();
}
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.printf(MY_LOG_FORMAT("  Callback \"StationDisconnected\"\r\n"));
  connected = false;

  printWiFiStatus();

  // shouldn't even be here when wifiIsDisabled, but still happens ...
  if (!wifiIsDisabled) {
    // immediately try to reconnect WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
}
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.printf(MY_LOG_FORMAT("  Callback \"GotIP\"\r\n"));
  connected = true;

  printWiFiStatus();
}

void wifi_enable(void) {
  wifiIsDisabled = false;

  WiFi.onEvent(WiFiStationDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}
void wifi_disable(void){
  wifiIsDisabled = true;

  WiFi.removeEvent(ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.disconnect();
}

void wifi_setup(){
/*  
  WiFi can be started via wifi_enable(); Won't wait until WiFi has connected.
  Event connected:    Will only be logged, nothing else happens
  Event GotIP:        From here on WiFi can be used. Only from here on IP address is available
  Event Disconnected: Will automatically try to reconnect here. If reconnection happens, first event connected will be fired, after this event gotIP fires
*/
  WiFi.onEvent(WiFiStationConnected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);    
  WiFi.mode(WIFI_STA);

  wifi_disable();
}
