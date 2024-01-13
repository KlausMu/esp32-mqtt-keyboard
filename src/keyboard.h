#define MQTTCMND_UP                  UNIQUE_DEVICE_NAME "/cmnd/UP"
#define MQTTCMND_DOWN                UNIQUE_DEVICE_NAME "/cmnd/DOWN"
#define MQTTCMND_RIGHT               UNIQUE_DEVICE_NAME "/cmnd/RIGHT"
#define MQTTCMND_LEFT                UNIQUE_DEVICE_NAME "/cmnd/LEFT"
#define MQTTCMND_SELECT              UNIQUE_DEVICE_NAME "/cmnd/SELECT"
#define MQTTCMND_SENDSTRING          UNIQUE_DEVICE_NAME "/cmnd/SENDSTRING"
#define MQTTCMND_BACK                UNIQUE_DEVICE_NAME "/cmnd/BACK"
#define MQTTCMND_HOME                UNIQUE_DEVICE_NAME "/cmnd/HOME"
#define MQTTCMND_MENU                UNIQUE_DEVICE_NAME "/cmnd/MENU"
#define MQTTCMND_SCAN_PREVIOUS_TRACK UNIQUE_DEVICE_NAME "/cmnd/SCAN_PREVIOUS_TRACK"
#define MQTTCMND_REWIND_LONG         UNIQUE_DEVICE_NAME "/cmnd/REWIND_LONG"
#define MQTTCMND_REWIND              UNIQUE_DEVICE_NAME "/cmnd/REWIND"
#define MQTTCMND_PLAYPAUSE           UNIQUE_DEVICE_NAME "/cmnd/PLAYPAUSE"
#define MQTTCMND_FASTFORWARD         UNIQUE_DEVICE_NAME "/cmnd/FASTFORWARD"
#define MQTTCMND_FASTFORWARD_LONG    UNIQUE_DEVICE_NAME "/cmnd/FASTFORWARD_LONG"
#define MQTTCMND_SCAN_NEXT_TRACK     UNIQUE_DEVICE_NAME "/cmnd/SCAN_NEXT_TRACK"
#define MQTTCMND_MUTE                UNIQUE_DEVICE_NAME "/cmnd/MUTE"
#define MQTTCMND_VOLUME_INCREMENT    UNIQUE_DEVICE_NAME "/cmnd/VOLUME_INCREMENT"
#define MQTTCMND_VOLUME_DECREMENT    UNIQUE_DEVICE_NAME "/cmnd/VOLUME_DECREMENT"

void keyboard_setup(void);
void keyboard_write(uint8_t c);
void keyboard_home();
void keyboard_sendString(const String &s);
void consumerControl_write(uint8_t value);
void consumerControl_longpress(uint8_t value);