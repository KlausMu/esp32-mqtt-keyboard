# esp32-mqtt-keyboard
## Overview
This project turns an ESP32 <b>S3</b> into a HID keyboard. The keyboard emulates a <b>hardware keyboard</b>, not a bluetooth keyboard.
* only works with an ESP32 S3 or ESP32 S2 (not tested), because the original ESP32 does not support native USB OTG
* Keyboard strokes can be send with MQTT commands (e.g. topic "esp32_keyboard/cmnd/UP", payload doesn't matter)

What is this good for?
* if you can't or don't want to connect an ESP32 as a bluetooth keyboard to your device, you could still connect an ESP32 S3 via USB cable
* in my case the bluetooth connection between the ESP32 and an Amazon Fire TV did not work, so I used a USB cable connection to connect the ESP32 S3 as a keyboard to control the Amazon Fire TV

Features are
* robust WiFi and MQTT connection. If disconnected, device will automatically reconnect
* easily expandable if more keys or special combinations of keys are needed

Media control keys
* support of so called "consumer controller" commands like PLAY/PAUSE, REWIND, FORWARD, ...
* support of special keys needed for controlling a Fire TV, like HOME, BACK and MENU

## ESP32 S3 connections
The ESP32 S3 normally has two USB connectors.
* one labeled "COM"
* one labeled "USB"

Via the port labeled "COM" you will upload the firmware. Via the port "USB" you will connect the ESP32 S3 as a keyboard to your device.
You can have both ports connected at the same time, even to the same device.
Before uploading, I sometimes had some trouble when both ports were connected before. In this case, disconnect both ports and reconnect only "COM". After uploading, you can add the connection to "USB".
![TempControllerModeArduiTouch](https://github.com/KlausMu/esp32-mqtt-keyboard/blob/main/images/ESP32_S3_Back.jpg)
![TempControllerModeArduiTouch](https://github.com/KlausMu/esp32-mqtt-keyboard/blob/main/images/ESP32_S3_Front.jpg)

## Installation
I used PlatformIO IDE for installation: <a href="https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation">guide</a>. It is as simple as:
* install VSCode (Visual Studio Code)
* install PlatformIO as an VSCode extension
* clone this repository or download it
* use "open folder" in VSCode to open this repository
* check settings in "config.h"
* upload to ESP32
With PlatformIO selection of the board and the USB mode is automatically done in file "platformio.ini".

You can also use the ArduinoIDE (not tested). Here you have to select the correct board and settings for an ESP32 S3 by yourself.
* choose an ESP32 S3 board
* "Upload Mode": "UART0 / Hardware CDC"
* "USB Mode": "USB OTG"
* "USB CDC On Boot": "Disabled"

Please give me feedback if you were successfull with these settings.
  


