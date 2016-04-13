#include <FS.h>
#include <ESP8266WiFi.h>       // https://github.com/esp8266/Arduino

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>       // https://github.com/tzapu/WiFiManager

#include <PubSubClient.h>      // https://github.com/knolleary/pubsubclient.git

#define PIN_01 0
#define PIN_02 1
#define PIN_03 3
#define PIN_04 4
#define PIN_05 5
#define PIN_06 12
#define PIN_07 13
#define PIN_08 14
#define PIN_09 15
#define PIN_19 16

// define default values for the mqtt server, the values are going to be
// overwriten by the config file
char mqtt_server[40];
char mqtt_port[6] = "8080";
char mqtt_client_name[40] = "CLIENTNAME";
char mqtt_topic[40] = "/YOURMQTTPATH/NAME";

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

uint16_t status = 0;

// flag if we need to write data
bool shouldSaveConfig = false;

/** callback notifying us of changes within the WifiManager, write config back
 * to file
 */
void saveConfigCallback () {
  shouldSaveConfig = true;
}

/** Load the /config.bin file and read the values for mqtt_server, mqtt_port
 * and mqtt_topic.
 */
void loadConfigFromFile() {
    if (SPIFFS.begin()) {
      if (SPIFFS.exists("/config.bin")) {
        //file exists, reading and loading
        File configFile = SPIFFS.open("/config.bin", "r");

        if (configFile) {
          configFile.readBytes(mqtt_server, sizeof(char)*40);
          configFile.readBytes(mqtt_port, sizeof(char)*6);
          configFile.readBytes(mqtt_topic, sizeof(char)*40);
          configFile.readBytes(mqtt_client_name, sizeof(char)*40);
        }
      }
    }
}

/** Save custom config values mqtt_server, mqtt_port and mqtt_topic to config
 * file (/config.bin)
 */
void saveConfigToFile() {
    File configFile = SPIFFS.open("/config.bin", "w");
    configFile.write((uint8_t*)mqtt_server, sizeof(char)*40);
    configFile.write((uint8_t*)mqtt_port, sizeof(char)*6);
    configFile.write((uint8_t*)mqtt_topic, sizeof(char)*40);
    configFile.write((uint8_t*)mqtt_client_name, sizeof(char)*40);

    configFile.close();
}

/** Setup WifiManager and add the mqtt_* paramters as custom parameters */
void setupWifiManager() {
    WiFiManagerParameter param_mqtt_server("server", "mqtt server", mqtt_server, 40);
    WiFiManagerParameter param_mqtt_port("port", "mqtt port", mqtt_port, 5);
    WiFiManagerParameter param_mqtt_topic("topic", "mqtt topic", mqtt_topic, 40);
    WiFiManagerParameter param_mqtt_client_name("client name", "mqtt client name", mqtt_client_name, 40);

    WiFiManager wifiManager;

    wifiManager.addParameter(&param_mqtt_server);
    wifiManager.addParameter(&param_mqtt_port);
    wifiManager.addParameter(&param_mqtt_topic);
    wifiManager.addParameter(&param_mqtt_client_name);

    if(!wifiManager.autoConnect("MqttButtonLed", "1234")) {
      // failed setup of wifi reset ESP and hope more luck next time
      ESP.reset();
    }

    //read updated parameters
    strcpy(mqtt_server, param_mqtt_server.getValue());
    strcpy(mqtt_port, param_mqtt_port.getValue());
    strcpy(mqtt_topic, param_mqtt_topic.getValue());
    strcpy(mqtt_client_name, param_mqtt_client_name.getValue());
}

/** Callback from MqttServer when getting a new message */
void mqttCallback(char* topic, byte* payload, unsigned int length) {

}

/** Setup the Mqtt connection */
void setupMqtt() {
    int port = strtol(mqtt_port, NULL, 10);
    mqtt_client.setServer(mqtt_server, port);
    mqtt_client.setCallback(mqttCallback);
}

void interruptOne();
void interruptTwo();
void interruptThree();
void interruptFour();

/** Setup all the inputs and outputs */
void setupIOPins() {
    pinMode(PIN_01, OUTPUT);
    pinMode(PIN_02, OUTPUT);
    pinMode(PIN_03, OUTPUT);
    pinMode(PIN_04, OUTPUT);

    pinMode(PIN_05, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_05), interruptOne, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_06), interruptTwo, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_07), interruptThree, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_08), interruptFour, FALLING);
}

/** The setup entry point, gets called on start up */
void setup() {
    loadConfigFromFile();
    setupWifiManager();

    if (shouldSaveConfig) {
        saveConfigToFile();
    }

    setupMqtt();
}

/** Reconnect mqtt client and subscribe if necessary */
void reconnect() {
    while(!mqtt_client.connected()) {
        if (mqtt_client.connect(mqtt_client_name)) {
            mqtt_client.subscribe(mqtt_topic); // TODO add /in to topic
        } else {
            delay(1000);
        }
    }
}

/** The main loop to run. */
void loop() {
    if (!mqtt_client.connected()) {
        reconnect();
    }

    mqtt_client.loop();

    if ((status & PIN_05) == PIN_05) {
        mqtt_client.publish(mqtt_topic, "1"); // TODO add /out to topic
        status ^= PIN_05;
    }
}
