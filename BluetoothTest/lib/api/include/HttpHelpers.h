#ifndef HTTP_HELPERS
#define HTTP_HELPERS

#include <ArduinoJson.h>
#include "CustomHeader.h"
enum class RequestType{
  GET = 1,
  POST = 2,
  PUT = 3,
  DELETE = 4
};

extern String s_username;
extern String s_password;

boolean SendRequest(RequestType reqType, String fullEndPoint, String payload,
 WiFiClientSecure &client, DynamicJsonDocument &json);
bool isSuccessCode(int statusCode);
Headers ReadHeaders(WiFiClientSecure &client,DynamicJsonDocument& json);
String ReadPlainText(WiFiClientSecure &client);
JsonObject ReadJson(WiFiClientSecure &client,DynamicJsonDocument& json);
#endif