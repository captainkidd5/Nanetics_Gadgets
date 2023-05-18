#ifndef HELPERS
#define HELPERS

#include <Arduino.h>

#include <ArduinoJson.h>
#include <map>
extern StaticJsonDocument<1024> s_jsonDoc;
void printAvailableMemory();
bool retrieveSPIIFSValue(std::map<String, String>* dict);
bool storeSPIFFSValue(std::map<String, String>* dict);

#endif