#ifndef HTTP_HELPERS
#define HTTP_HELPERS
#include <Arduino.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>
#include "Headers.h"

#include "Server_configs.h"


enum class RequestType{
  GET = 1,
  POST = 2,
  PUT = 3,
  DELETE = 4
};

extern String s_username;
extern String s_password;

String RequestTypeToString(RequestType r);

bool isSuccessCode(int statusCode);
Headers ReadHeaders(WiFiClientSecure &client);
String ReadPlainText(WiFiClientSecure &client);
JsonObject ReadJson(WiFiClientSecure &client,DynamicJsonDocument& json);
#endif