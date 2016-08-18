#pragma once

class MqttHandler;

bool runWiFiManager(MqttHandler &handler, const char *ssid, const char *passwd);
