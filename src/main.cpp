#include <FS.h>
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino

#include "mqtthandler.h"
#include "confighandler.h"
#include "lightstrip.h"

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

MqttHandler mqtthandler{};
LightStrip *lightstrip;

/** Callback from MqttServer when getting a new message */
void mqttCallback(char *topic, byte *payload, unsigned int length) {
    if (payload[0] == '1') {
        digitalWrite(PIN_D1, 1);
    } else {
        digitalWrite(PIN_D1, 0);
    }
}

void interruptOne() { status |= 1; }

void interruptTwo() { status |= 2; }

void interruptThree() { status |= 4; }

/** Setup all the inputs and outputs */
void setupIOPins() {
    pinMode(PIN_D1, OUTPUT);
    pinMode(PIN_D3, OUTPUT);

    pinMode(PIN_D5, INPUT_PULLUP);
    pinMode(PIN_D6, INPUT_PULLUP);
    pinMode(PIN_D7, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_D5), interruptOne, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_D6), interruptTwo, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_D7), interruptThree, FALLING);
}

/** The setup entry point, gets called on start up */
void setup() {
    lightstrip = createLightStrip(12);
    lightstrip->setColorFor(0, 0, 128, 0);
    lightstrip->update(1);

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
    if (status & 1 == 1) {
        lightstrip->setColor(128, 0, 0);
        status = 0;
    } else if ((status & 2) == 2) {
        lightstrip->setColor(0, 128, 0);
        status = 0;
    } else if ((status & 4) == 4) {
        lightstrip->setColor(0, 0, 128);
        status = 0;
    }

    lightstrip->update(1);
    delay(150);
}
