#ifndef RESPONSEOBJECT
#define RESPONSEOBJECT
#include <Arduino.h>
#include <Headers.h>
#include <ArduinoJson.h>
#include <map>


class ResponseObject {
public:
    Headers header;
    std::map<String, String> jsonDictionary;
private:
};

#endif 
