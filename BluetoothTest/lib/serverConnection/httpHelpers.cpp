#include <ArduinoJson.h>

DynamicJsonDocument deserialize(String response)
{
    const size_t capacity = JSON_OBJECT_SIZE(2) + 70;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, response);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }
    return doc;
}