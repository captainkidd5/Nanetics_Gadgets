#ifndef CUSTOMER_HEADER
#define CUSTOMER_HEADER

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>

using namespace std;

enum class CustomContentType {
  None,
  PlainText,
  JSON
};

class Headers {
public:
  CustomContentType ContentType;
  Headers() {}

  void setHeader(String key, String value);
  String getHeader(String key) const;

private:
  std::map<String, String> HeadersMap;
};
#endif
