#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>
#include <string>
#include "../types/customHeader.h"


bool isSuccessCode(int statusCode) {
  return statusCode >= 200 && statusCode < 300;
}

Headers ReadHeaders(WiFiClientSecure &client,DynamicJsonDocument& json)
{
        CustomContentType contentType = CustomContentType::None;

      bool contentIsPlainText = false;
   while (client.connected())
      {
        String line = client.readStringUntil('\n'); // HTTP headers
        // Serial.println(line);
        if (line == "\r")
        {
          if (contentIsPlainText)
          {
            break;
          }
        }
        if (line.startsWith("HTTP/1."))
        {
          int statusCode = line.substring(line.indexOf(' ') + 1, line.indexOf(' ') + 4).toInt();
          Serial.println("Response code: " + String(statusCode));
          if (isSuccessCode(statusCode))
          {
            Serial.println("Request successful!");
            contentIsPlainText = true;
          }
          else
          {
            String errorMessage = line;
            Serial.println("Error response: " + errorMessage);
          }
        }
        else if (line.startsWith("Content-Type:"))
        {
          contentType = CustomContentType::PlainText;
          String contentTypeString = line.substring(line.indexOf(' ') + 1, line.length() - 1);
          Serial.println("Content-Type: " + contentTypeString);
          contentIsPlainText = (contentTypeString == "text/plain; charset=utf-8");
        }
      }
Headers myHeaders(contentType);

      return myHeaders;
}

String ReadPlainText(WiFiClientSecure &client){
   Serial.println("reading plaintext body...");
        String content = client.readStringUntil('\n');
        int contentLength = content.toInt();
        int bytesRead = content.length() + 1; // add 1 for the '\n' character
        while (bytesRead < contentLength)
        {
          String line = client.readStringUntil('\n');
          bytesRead += line.length() + 1; // add 1 for the '\n' character
          content += line;
        }
        Serial.println("Content body: " + content);
        return content;
}

JsonObject ReadJson(WiFiClientSecure &client,DynamicJsonDocument& json)
{
   DeserializationError error = deserializeJson(json, client);

        if (error)
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
          return;
        }

        JsonObject root_0 = json[0];
        return root_0;
        // Serial.println(root_0);

        // //  Get the Name:
        // const char *root_0_name = root_0["name"];
        // json.clear();
}