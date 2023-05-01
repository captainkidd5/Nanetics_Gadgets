
#ifndef BASE_API_REQUEST
#define BASE_API_REQUEST
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include "HttpHelpers.h"

bool SendRequest(RequestType reqType, String fullEndPoint,
 WiFiClientSecure &client, DynamicJsonDocument &json,bool includeRefreshToken = true);

 #endif