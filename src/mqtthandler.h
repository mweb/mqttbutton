#ifndef mqtthandler_h
#define mqtthandler_h

#include <memory>
#include <functional>
#include <stdint.h>

#include <PubSubClient.h>

class WiFiClient;
class PubSubClient;

constexpr int DEFAULT_LENGTH = 40;

/** This class handles everything around the Mqtt connection */
class MqttHandler {
  public:
    /* ctor with default values
      server = ""
      port = 1883
      name = "CLIENTNAME"
      topic = "/YOURMQTTPATH/NAME"
    */
    MqttHandler();
    /* ctor set the default paramters */
    MqttHandler(const char *server, const char *port, const char *name,
                const char *topic);

    /** Load configuration from file with mqtt values. */
    void loadFromConfigFile(const char *filename);
    /** Save configuration to file with mqtt values. */
    void saveToConfigFile(const char *filename);

    void setup(std::function<void(char *, uint8_t *, unsigned int)> callback);
    void loop();
    /** Send message on configures TOPIC/out */
    void publish(const char *message);
    void publish(const char *topic, const char *message);

    /* Setters */
    void setServerAddress(const char *server);
    void setPort(const char *port);
    void setClientName(const char *name);
    void setTopic(const char *name);

    /* Getters */
    const char *getServerAddress();
    uint16_t getPortAsInt();
    const char *getPort();
    const char *getClientName();
    const char *getTopic();

  private:
    void reconnect();

    char m_server[DEFAULT_LENGTH + 1];
    char m_port[6];
    char m_clientName[DEFAULT_LENGTH + 1];
    char m_topic[DEFAULT_LENGTH + 1];

    char m_topicIn[DEFAULT_LENGTH + 1 + 3];
    char m_topicOut[DEFAULT_LENGTH + 1 + 4];

    std::unique_ptr<WiFiClient> m_wifiClient;
    std::unique_ptr<PubSubClient> m_mqttClient;
};

#endif
