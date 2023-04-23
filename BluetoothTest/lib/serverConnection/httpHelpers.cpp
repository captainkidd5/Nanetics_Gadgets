#include <ArduinoJson.h>

enum class CustomContentType {
  None,
  PlainText,
  JSON
};

bool isSuccessCode(int statusCode) {
  return statusCode >= 200 && statusCode < 300;
}



DynamicJsonDocument deserialize(String response)
{
    const size_t capacity = JSON_OBJECT_SIZE(2) + 70;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, response);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
    }
    return doc;
}