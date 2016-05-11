#include <FS.h>
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino

#include "mqtthandler.h"
#include "confighandler.h"

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
constexpr int8_t PIN_SD3 = 10;
constexpr int8_t PIN_SD2 = 9;

uint16_t status = 0;

MqttHandler mqtthandler{};

/** Callback from MqttServer when getting a new message */
void mqttCallback(char *topic, byte *payload, unsigned int length) {}

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
    pinMode(PIN_D8, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_D5), interruptOne, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_D6), interruptTwo, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_D7), interruptThree, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_D8), interruptFour, FALLING);
}

/** The setup entry point, gets called on start up */
void setup() {
    mqtthandler.loadFromConfigFile("/config.bin");
    if (runWiFiManager(mqtthandler, "mqttbtnswitch", "1234")) {
        mqtthandler.saveToConfigFile("/config.bin");
    }

    mqtthandler.setup(mqttCallback);

    setupIOPins();
}

/** The main loop to run. */
void loop() {
    mqtthandler.loop();

    if ((status & 1) == 1) {
        mqtthandler.publish("1");
        status ^= 1;
    }
    if ((status & 1 << 1) == 1 << 1) {
        mqtthandler.publish("2");
        status ^= 1 << 1;
    }
    if ((status & 1 << 2) == 1 << 2) {
        mqtthandler.publish("3");
        status ^= 1 << 2;
    }
    if ((status & 1 << 3) == 1 << 3) {
        mqtthandler.publish("4");
        status ^= 1 << 3;
    }
    delay(50);
}
