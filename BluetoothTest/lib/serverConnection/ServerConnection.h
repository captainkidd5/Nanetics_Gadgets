#ifndef SERVER_CONNECTION
#define SERVER_CONNECTION

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <registerDevice.h>


  void apiLoop(WiFiClientSecure& client,DynamicJsonDocument& json);

  #endif