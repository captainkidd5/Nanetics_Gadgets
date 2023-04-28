#ifndef CUSTOM_HEADER
#define CUSTOM_HEADER
#include <Arduino.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>

enum class CustomContentType {
  None,
  PlainText,
  JSON
};

class Headers {
public:
  void setHeader(String key, String value);
  String getHeader(String key) const;
  CustomContentType ContentType;
  int StatusCode;

private:
  std::map<String, String> HeadersMap;
};

#endif 
