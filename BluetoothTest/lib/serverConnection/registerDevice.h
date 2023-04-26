
#ifndef REGISTER_DEVICE
#define REGISTER_DEVICE

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <server_configs.h>
#include <httpHelpers.h>
#include <iostream>
#include <helpers.h>


void postRegisterDevice(WiFiClientSecure &client,DynamicJsonDocument& json);

#endif