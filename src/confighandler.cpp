#include "confighandler.h"

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "mqtthandler.h"

// flag if we need to write data
bool shouldSaveConfig = false;

/** callback notifying us of changes within the WifiManager, write config back
 * to file
 */
void saveConfigCallback() { shouldSaveConfig = true; }

bool runWiFiManager(MqttHandler &handler, const char *ssid,
                    const char *passwd) {
    WiFiManagerParameter param_mqtt_server(
        "server", "mqtt server", handler.getServerAddress(), DEFAULT_LENGTH);
    WiFiManagerParameter param_mqtt_port("port", "mqtt port", handler.getPort(),
                                         5);
    WiFiManagerParameter param_mqtt_client_name(
        "client name", "mqtt client name", handler.getClientName(),
        DEFAULT_LENGTH);
    WiFiManagerParameter param_mqtt_topic("topic", "mqtt topic",
                                          handler.getTopic(), DEFAULT_LENGTH);

    WiFiManager wifiManager;

    wifiManager.addParameter(&param_mqtt_server);
    wifiManager.addParameter(&param_mqtt_port);
    wifiManager.addParameter(&param_mqtt_topic);
    wifiManager.addParameter(&param_mqtt_client_name);

    if (!wifiManager.autoConnect(ssid, passwd)) {
        // failed setup of wifi reset ESP and hope more luck next time
        ESP.reset();
    }

    // read updated parameters
    handler.setServerAddress(param_mqtt_server.getValue());
    handler.setPort(param_mqtt_port.getValue());
    handler.setTopic(param_mqtt_topic.getValue());
    handler.setClientName(param_mqtt_client_name.getValue());

    return shouldSaveConfig;
}
