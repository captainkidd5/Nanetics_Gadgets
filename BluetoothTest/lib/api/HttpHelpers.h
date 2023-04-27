#ifndef HTTP_HELPERS
#define HTTP_HELPERS

#include <ArduinoJson.h>
#include "../types/customHeader.h"
enum class RequestType{
  GET = 1,
  POST = 2,
  PUT = 3,
  DELETE = 4
};
bool isSuccessCode(int statusCode);
Headers ReadHeaders(WiFiClientSecure &client,DynamicJsonDocument& json);
String ReadPlainText(WiFiClientSecure &client);
JsonObject ReadJson(WiFiClientSecure &client,DynamicJsonDocument& json);
#endif