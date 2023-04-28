#ifndef AUTH
#define AUTH

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>




void PostLogin(WiFiClientSecure &client, DynamicJsonDocument &json);
void PostRefresh(WiFiClientSecure &client, DynamicJsonDocument &json); 
#endif