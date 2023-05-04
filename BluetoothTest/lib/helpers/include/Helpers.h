#ifndef HELPERS
#define HELPERS

#include <Arduino.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <map>

void printAvailableMemory();
bool retrieveSPIIFSValue(std::map<String, String>* dict);
bool storeSPIFFSValue(std::map<String, String>* dict);

#endif