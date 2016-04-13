#include <FS.h>
#include <ESP8266WiFi.h>       // https://github.com/esp8266/Arduino

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>       // https://github.com/tzapu/WiFiManager

#include <PubSubClient.h>      // https://github.com/knolleary/pubsubclient.git

// define default values for the mqtt server, the values are going to be
// overwriten by the config file
char mqtt_server[40];
char mqtt_port[6] = "8080";
char mqtt_topic[40] = "/YOURMQTTPATH/NAME";

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

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

    configFile.close();
}

/** Setup WifiManager and add the mqtt_* paramters as custom parameters */
void setupWifiManager() {
    WiFiManagerParameter param_mqtt_server("server", "mqtt server", mqtt_server, 40);
    WiFiManagerParameter param_mqtt_port("port", "mqtt port", mqtt_port, 5);
    WiFiManagerParameter param_mqtt_topic("topic", "mqtt topic", mqtt_topic, 40);

    WiFiManager wifiManager;

    wifiManager.addParameter(&param_mqtt_server);
    wifiManager.addParameter(&param_mqtt_port);
    wifiManager.addParameter(&param_mqtt_topic);

    if(!wifiManager.autoConnect("MqttButtonLed", "1234")) {
      // failed setup of wifi reset ESP and hope more luck next time
      ESP.reset();
    }

    //read updated parameters
    strcpy(mqtt_server, param_mqtt_server.getValue());
    strcpy(mqtt_port, param_mqtt_port.getValue());
    strcpy(mqtt_topic, param_mqtt_topic.getValue());
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

/** The setup entry point, gets called on start up */
void setup() {
    loadConfigFromFile();
    setupWifiManager();

    if (shouldSaveConfig) {
        saveConfigToFile();
    }

    setupMqtt();
}

/** The main loop to run. */
void loop() {
    if (!mqtt_client.connected()) {
        reconnect();
    }
    
    mqtt_client.loop();
}
