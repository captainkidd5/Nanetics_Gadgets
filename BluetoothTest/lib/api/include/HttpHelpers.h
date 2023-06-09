#ifndef HTTP_HELPERS
#define HTTP_HELPERS
#include <Arduino.h>

#include <ArduinoJson.h>
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


extern String s_refreshToken;
String ParseSetCookie(Headers &headers);
void AppendHeader(RequestType reqType, WiFiClientSecure &client, String key, String value);
String RequestTypeToString(RequestType r);
void GetJsonDictionary(WiFiClientSecure &client,  std::map<String, String>& myDict);
bool isSuccessCode(int statusCode);
Headers ParseHeaders(WiFiClientSecure &client);
String ReadPlainText(WiFiClientSecure &client);
JsonObject ReadJson(WiFiClientSecure &client);
#endif