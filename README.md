# mqttbutton

[![Build Status](https://travis-ci.org/mweb/mqttbutton.svg?branch=master)](https://travis-ci.org/mweb/mqttbutton)

This Project provides a starting point for your own MQTT Actor/Sensor 
implementation for ESP8266. It uses 
[ESP8266 core for Arduino](https://github.com/esp8266/Arduino).

This branch does not use the WiFiManager. The WiFi and MQTT Parameters are 
stored within the code.

It uses [PlatformIO](http://platformio.org) to build.

## Dependencies

The dependencies are:
 - [PubSubClient](https://github.com/knolleary/pubsubclient.git)

They are integrated as git submodules.
