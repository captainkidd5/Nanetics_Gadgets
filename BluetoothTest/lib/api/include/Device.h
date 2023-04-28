#ifndef DEVICE
#define DEVICE

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>


void PostRegisterDevice(WiFiClientSecure &client, DynamicJsonDocument &json);

#endif