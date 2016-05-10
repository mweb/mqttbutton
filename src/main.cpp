#include <FS.h>
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino

//#include <DNSServer.h>
//#include <ESP8266WebServer.h>

#include "mqtthandler.h"
#include "confighandler.h"

constexpr int8_t PIN_01 = 0;
constexpr int8_t PIN_02 = 1;
constexpr int8_t PIN_03 = 3;
constexpr int8_t PIN_04 = 4;
constexpr int8_t PIN_05 = 5;
constexpr int8_t PIN_06 = 12;
constexpr int8_t PIN_07 = 13;
constexpr int8_t PIN_08 = 14;
constexpr int8_t PIN_09 = 15;
constexpr int8_t PIN_19 = 16;

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
    pinMode(PIN_01, OUTPUT);
    pinMode(PIN_02, OUTPUT);
    pinMode(PIN_03, OUTPUT);
    pinMode(PIN_04, OUTPUT);

    pinMode(PIN_05, INPUT_PULLUP);
    pinMode(PIN_06, INPUT_PULLUP);
    pinMode(PIN_07, INPUT_PULLUP);
    pinMode(PIN_08, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_05), interruptOne, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_06), interruptTwo, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_07), interruptThree, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_08), interruptFour, FALLING);
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

    // if ((status & PIN_05) == PIN_05) {
    //     mqtt_client.publish(mqtt_topic, "1"); // TODO add /out to topic
    //     status ^= PIN_05;
    // }
}
