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
  Headers(CustomContentType contentType = CustomContentType::None) : ContentType(contentType) {}
  CustomContentType ContentType;
  

  
  void setHeader(const string& key, const string& value) {
    HeadersMap[key] = value;
  }
  
  string getHeader(const string& key) const {
    auto it = HeadersMap.find(key);
    if (it != HeadersMap.end()) {
      return it->second;
    } else {
      return "";
    }
  }
  
private:
  map<string, string> HeadersMap;
};