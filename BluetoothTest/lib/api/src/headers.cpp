#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>
#include <Arduino.h>
#include "Headers.h"



  void Headers::setHeader(String key, String value)  {
    HeadersMap[key] = value;
  }


  String Headers::getHeader(String key) const {
    auto it = HeadersMap.find(key);
    if (it != HeadersMap.end()) {
      return it->second;
    } else {
      return "";
    }
  }


