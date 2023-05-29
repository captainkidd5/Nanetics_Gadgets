#ifndef HELPERS
#define HELPERS

#include <Arduino.h>

#include <ArduinoJson.h>
#include <map>
extern StaticJsonDocument<1024> s_jsonDoc;
extern char* s_setupEmail;
extern String s_assigned_id;
extern String s_scope_id;
extern String s_primaryKey;
extern String s_accessToken;
void printAvailableMemory();
bool retrieveSPIIFSValue(std::map<String, String>* dict);
bool storeSPIFFSValue(std::map<String, String>* dict);

#endif