#ifndef AUTH
#define AUTH

#include <ArduinoJson.h>
#include <WiFiClientSecure.h>




bool PostLogin(WiFiClientSecure &client, String userName, String passWord);
#endif