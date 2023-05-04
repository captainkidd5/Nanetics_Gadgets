#ifndef DEVICE
#define DEVICE

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>

bool GetIsRegistered(WiFiClientSecure &client, DynamicJsonDocument &json);
void PostRegisterDevice(WiFiClientSecure &client, DynamicJsonDocument &json);

#endif