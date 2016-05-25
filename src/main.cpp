#include <FS.h>
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino

#include "mqtthandler.h"

const char *ssid = "Asgard";
const char *password = "ThorOdinsSohn";

const char *mqtt_server = "192.168.42.12";
const char *mqtt_port = "1883";
const char *mqtt_name = "EspOne";
const char *mqtt_topic = "EspTest/EspOne";

constexpr int8_t PIN_D0 = 16;
constexpr int8_t PIN_D1 = 5;
constexpr int8_t PIN_D2 = 4;
constexpr int8_t PIN_D3 = 0;
constexpr int8_t PIN_D4 = 2;
constexpr int8_t PIN_D5 = 14;
constexpr int8_t PIN_D6 = 12;
constexpr int8_t PIN_D7 = 13;
constexpr int8_t PIN_D8 = 15;
constexpr int8_t PIN_RX = 3;
constexpr int8_t PIN_TX = 1;

uint16_t status = 0;
MqttHandler mqtthandler(mqtt_server, mqtt_port, mqtt_name, mqtt_topic);

/** Callback from MqttServer when getting a new message */
void mqttCallback(char *topic, byte *payload, unsigned int length) {
    if (strcmp(topic, "EspTest/EspOne/in/1") == 0) {
        if (payload[0] == '1') {
            digitalWrite(PIN_D1, 1);
        } else {
            digitalWrite(PIN_D1, 0);
        }
    } else if (strcmp(topic, "EspTest/EspOne/in/2") == 0) {
        if (payload[0] == '1') {
            digitalWrite(PIN_D2, 1);
        } else {
            digitalWrite(PIN_D2, 0);
        }
    } else if (strcmp(topic, "EspTest/EspOne/in/3") == 0) {
        if (payload[0] == '1') {
            digitalWrite(PIN_D3, 1);
        } else {
            digitalWrite(PIN_D3, 0);
        }
    }
}

void interruptOne() { status |= 1; }

void interruptTwo() { status |= 1 << 1; }

void interruptThree() { status |= 1 << 2; }

void interruptFour() { status |= 1 << 3; }

/** Setup all the inputs and outputs */
void setupIOPins() {
    pinMode(PIN_D1, OUTPUT);
    pinMode(PIN_D2, OUTPUT);
    pinMode(PIN_D3, OUTPUT);

    pinMode(PIN_D5, INPUT_PULLUP);
    pinMode(PIN_D6, INPUT_PULLUP);
    pinMode(PIN_D7, INPUT_PULLUP);
    pinMode(PIN_RX, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_D5), interruptOne, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_D6), interruptTwo, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_D7), interruptThree, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_RX), interruptFour, RISING);
}

/** The setup entry point, gets called on start up */
void setup() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    mqtthandler.setup(mqttCallback);

    setupIOPins();
}

/** The main loop to run. */
void loop() {
    mqtthandler.loop();

    if ((status & 1) == 1) {
        mqtthandler.publish("EspTest/EspOne/out/1", "1");
        status ^= 1;
    }
    if ((status & 1 << 1) == 1 << 1) {
        mqtthandler.publish("EspTest/EspOne/out/2", "1");
        status ^= 1 << 1;
    }
    if ((status & 1 << 2) == 1 << 2) {
        mqtthandler.publish("EspTest/EspOne/out/3", "1");
        status ^= 1 << 2;
    }
    if ((status & 1 << 3) == 1 << 3) {
        mqtthandler.publish("EspTest/EspOne/out/4", "1");
        status ^= 1 << 3;
    }
    delay(50);
}
