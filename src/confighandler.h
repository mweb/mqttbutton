#ifndef confighandler_h
#define confighandler_h

class MqttHandler;

bool runWiFiManager(MqttHandler &handler, const char *ssid, const char *passwd);

#endif
