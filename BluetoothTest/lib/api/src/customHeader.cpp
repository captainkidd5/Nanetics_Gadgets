#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>
#include <Arduino.h>
#include "CustomHeader.h"



  void CustomHeader::setHeader(String key, String value)  {
    HeadersMap[key] = value;
  }


  String CustomHeader::getHeader(String key) const {
    auto it = HeadersMap.find(key);
    if (it != HeadersMap.end()) {
      return it->second;
    } else {
      return "";
    }
  }


