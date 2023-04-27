#ifndef DEVICE
#define DEVICE

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include "Server_configs.h"
#include "HttpHelpers.h"
#include <iostream>
#include "Helpers.h"
#include "CustomHeader.h"

void PostRegisterDevice(WiFiClientSecure &client, DynamicJsonDocument &json);

#endif