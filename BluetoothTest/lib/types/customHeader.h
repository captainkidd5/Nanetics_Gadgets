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
Headers(CustomContentType contentType = CustomContentType::None) : ContentType(contentType) {}

void setHeader(const string& key, const string& value);
string getHeader(const string& key) const;

private:
std::map<string, string> HeadersMap;
};
#endif