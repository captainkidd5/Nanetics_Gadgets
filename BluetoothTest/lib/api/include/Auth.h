#ifndef AUTH
#define AUTH

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>




bool PostLogin(WiFiClientSecure &client, DynamicJsonDocument &json, String userName, String passWord);
#endif