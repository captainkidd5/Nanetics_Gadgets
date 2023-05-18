#ifndef CUSTOM_HEADER
#define CUSTOM_HEADER
#include <Arduino.h>

#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <map>

enum class CustomContentType {
  None,
  PlainText,
  JSON
};
//const means cannot modify state of headers class
class Headers {
public:
void ClearHeaders();
  void SetHeader(String key, String value);
  String GetHeader(String key) const;
  void ParseHeaders(WiFiClientSecure &client);

  CustomContentType ContentType;
  int StatusCode;
private:
  std::map<String, String> HeadersMap;
};

#endif 
