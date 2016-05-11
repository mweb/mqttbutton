#include "mqtthandler.h"

#include <string>

#include <FS.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient.git

MqttHandler::MqttHandler() {
    setServerAddress("");
    setPort("1883");
    setClientName("CLIENTNAME");
    setTopic("/YOURMQTTPATH/NAME");

    m_wifiClient = std::unique_ptr<WiFiClient>(new WiFiClient);
    m_mqttClient =
        std::unique_ptr<PubSubClient>(new PubSubClient(*m_wifiClient));
}

MqttHandler::MqttHandler(const char *server, const char *port, const char *name,
                         const char *topic) {
    setServerAddress(server);
    setPort(port);
    setClientName(name);
    setTopic(topic);

    m_wifiClient = std::unique_ptr<WiFiClient>(new WiFiClient());
    m_mqttClient =
        std::unique_ptr<PubSubClient>(new PubSubClient(*m_wifiClient));
}

void MqttHandler::loadFromConfigFile(const char *filename) {
    if (SPIFFS.begin()) {
        if (SPIFFS.exists(filename)) {
            // file exists, reading and loading
            File configFile = SPIFFS.open("/config.bin", "r");

            if (configFile) {
                configFile.readBytes(m_server, sizeof(char) * DEFAULT_LENGTH);
                configFile.readBytes((char *)&m_port, sizeof(char) * 6);
                configFile.readBytes(m_clientName,
                                     sizeof(char) * DEFAULT_LENGTH);
                configFile.readBytes(m_topic, sizeof(char) * DEFAULT_LENGTH);
            }
        }
    }
}

void MqttHandler::saveToConfigFile(const char *filename) {
    File configFile = SPIFFS.open(filename, "w");
    configFile.write((uint8_t *)m_server, sizeof(char) * DEFAULT_LENGTH);
    configFile.write((uint8_t *)&m_port, sizeof(char) * 6);
    configFile.write((uint8_t *)m_clientName, sizeof(char) * DEFAULT_LENGTH);
    configFile.write((uint8_t *)m_topic, sizeof(char) * DEFAULT_LENGTH);

    configFile.close();
}

void MqttHandler::setup(
    std::function<void(char *, uint8_t *, unsigned int)> callback) {
    m_mqttClient->setServer(m_server, getPortAsInt());
    m_mqttClient->setCallback(callback);
}

void MqttHandler::reconnect() {
    while (!m_mqttClient->connected()) {
        if (m_mqttClient->connect(m_clientName)) {
            m_mqttClient->subscribe(m_topicIn);
        } else {
            delay(1000);
        }
    }
}

void MqttHandler::loop() {
    reconnect();
    m_mqttClient->loop();
}

void MqttHandler::publish(const char *message) {
    m_mqttClient->publish(m_topicOut, message);
}

void MqttHandler::publish(const char *topic, const char *message) {
    m_mqttClient->publish(topic, message);
}

void MqttHandler::setServerAddress(const char *server) {
    strncpy(m_server, server, DEFAULT_LENGTH);
    m_server[DEFAULT_LENGTH] = 0;
}

void MqttHandler::setPort(const char *port) {
    strncpy(m_port, port, 6);
    m_port[6] = 0;
}

void MqttHandler::setClientName(const char *name) {
    strncpy(m_clientName, name, DEFAULT_LENGTH);
    m_clientName[DEFAULT_LENGTH] = 0;
}

void MqttHandler::setTopic(const char *topic) {
    strncpy(m_topic, topic, DEFAULT_LENGTH);
    m_topic[DEFAULT_LENGTH] = 0;

    strncpy(m_topicIn, m_topic, DEFAULT_LENGTH);
    strncat(m_topicIn, "/in", 3);
    strncpy(m_topicOut, m_topic, DEFAULT_LENGTH);
    strncat(m_topicOut, "/out", 4);
}

const char *MqttHandler::getServerAddress() { return m_server; }

const char *MqttHandler::getPort() { return m_port; }

uint16_t MqttHandler::getPortAsInt() { return strtol(m_port, NULL, 10); }

const char *MqttHandler::getClientName() { return m_clientName; }

const char *MqttHandler::getTopic() { return m_topic; }
