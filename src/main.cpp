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

enum class CurrentStatus {
    Off,
    White,
    LightBlue,
    Pink,
    LightGreen,
    RedBlueRotate
};

CurrentStatus currentLightStatus{CurrentStatus::Off};
bool buttonReleased{true};

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
}

/** The setup entry point, gets called on start up */
void setup() {
    lightstrip = createLightStrip(12);
    lightstrip->setColorFor(0, 0, 128, 0);
    lightstrip->update();

    mqtthandler.loadFromConfigFile("/config.bin");
    lightstrip->setColorFor(1, 0, 128, 0);
    lightstrip->update();
    if (runWiFiManager(mqtthandler, "mqttlight", "1234")) {
        mqtthandler.saveToConfigFile("/config.bin");
        lightstrip->setColorFor(1, 0, 128, 128);
        lightstrip->update();
    }
    lightstrip->setColorFor(2, 0, 128, 0);
    lightstrip->update();

    mqtthandler.setup(mqttCallback);
    lightstrip->setColorFor(3, 0, 128, 0);
    lightstrip->update();

    setupIOPins();
    lightstrip->setColorFor(4, 0, 128, 0);
    lightstrip->update();
}

/** The main loop to run. */
void loop() {
    // lightstrip->setColorFor(4, 128, 0, 0);
    // lightstrip->update();
    // mqtthandler.loop();
    // lightstrip->setColorFor(4, 0, 0, 128);
    // lightstrip->update();
    int val = digitalRead(PIN_D5);
    if (val == HIGH && buttonReleased) {
        switch (currentLightStatus) {
        case CurrentStatus::Off:
            currentLightStatus = CurrentStatus::White;
            lightstrip->fadeToColor(128, 128, 128, 2000);
            break;
        case CurrentStatus::White:
            currentLightStatus = CurrentStatus::LightBlue;
            lightstrip->fadeToColor(20, 20, 128, 2000);
            break;
        case CurrentStatus::LightBlue:
            currentLightStatus = CurrentStatus::LightGreen;
            lightstrip->fadeToColor(20, 128, 20, 2000);
            break;
        case CurrentStatus::LightGreen:
            currentLightStatus = CurrentStatus::Pink;
            lightstrip->fadeToColor(128, 20, 20, 2000);
            break;
        case CurrentStatus::Pink:
            currentLightStatus = CurrentStatus::RedBlueRotate;
            lightstrip->setColor(0, 0, 0);
            lightstrip->setColorFor(0, 0, 0, 128);
            lightstrip->setColorFor(1, 0, 0, 128);
            lightstrip->setColorFor(6, 128, 0, 0);
            lightstrip->setColorFor(7, 128, 0, 0);
            lightstrip->rotateLeft(200);
            break;
        case CurrentStatus::RedBlueRotate:
            currentLightStatus = CurrentStatus::Off;
            lightstrip->fadeToColor(0, 0, 0, 2000);
            break;
        }
        buttonReleased = false;
    } else if (val == LOW && !buttonReleased) {
        buttonReleased = true;
    }

    lightstrip->update();
    delay(50);
}
