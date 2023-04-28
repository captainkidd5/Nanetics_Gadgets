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
  Headers(){}
  CustomContentType ContentType;

  void setHeader(String key, String value) {
    HeadersMap[key] = value;
  }

  String getHeader(String key) const {
    auto it = HeadersMap.find(key);
    if (it != HeadersMap.end()) {
      return it->second;
    } else {
      return "";
    }
  }

private:
  std::map<String, String> HeadersMap; // specify namespace explicitly
};
