
#ifndef REGISTER_DEVICE
#define REGISTER_DEVICE

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <server_configs.h>
#include <HttpHelpers.h>
#include <iostream>
#include <helpers.h>
#include "../types/customHeader.h"


void PostRegisterDevice(WiFiClientSecure &client,DynamicJsonDocument& json);

#endif