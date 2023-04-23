#ifndef HTTP_HELPERS
#define HTTP_HELPERS

#include <ArduinoJson.h>
enum class CustomContentType {
  None,
  PlainText,
  JSON
};

bool isSuccessCode(int statusCode);
DynamicJsonDocument deserialize(String response);
#endif