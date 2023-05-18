#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <map>
#include <Arduino.h>
#include "Headers.h"
#include "HttpHelpers.h"

void Headers::SetHeader(String key, String value)
{
  HeadersMap[key] = value;
}

String Headers::GetHeader(String key) const
{
  auto it = HeadersMap.find(key);
  if (it != HeadersMap.end())
  {
    return it->second;
  }
  else
  {
    return "";
  }
}

int ParseStatusCode(String line)
{
  int statusCode = line.substring(line.indexOf(' ') + 1, line.indexOf(' ') + 4).toInt();
  Serial.println("Response code: " + String(statusCode));
  if (isSuccessCode(statusCode))
  {
    Serial.println("Request successful!");
  }
  else
  {
    String errorMessage = line;
    Serial.println("Error response: " + errorMessage);
  }
  return statusCode;
}
CustomContentType ParseContentType(String line)
{
  CustomContentType contentType = CustomContentType::PlainText;
  String contentTypeString = line.substring(line.indexOf(' ') + 1, line.length() - 1);
  bool contentIsPlainText = (contentTypeString == "text/plain; charset=utf-8");
  if (!contentIsPlainText)
  {
    contentType = CustomContentType::JSON;
  }

  return contentType;
}
void Headers::ClearHeaders(){
  HeadersMap.clear();
}
void Headers::ParseHeaders(WiFiClientSecure &client)
{
  Serial.print("Parsing headers...");
  delay(1500);
  while (client.connected())
  {
    Serial.print("...");

    String line = client.readStringUntil('\n');
Serial.println(line);
    if (line == "\r")
    {
      Serial.println("Response headers received");
      return;
    }
    if (line.startsWith("HTTP/1."))
      StatusCode = ParseStatusCode(line);

    else if (line.startsWith("Content-Type:")){
      ContentType = ParseContentType(line);
    }

    else
    {
      int separatorIndex = line.indexOf(':');
      if (separatorIndex != -1)
      { // check if the line has a ':' separator
        String key = line.substring(0, separatorIndex);
        String value = line.substring(separatorIndex + 1);
       // Serial.println("Key:" + key);
      //  Serial.println("Value:" + value);
        if (key == "Set-Cookie")
        {

          // We only want to parse refresh token or authorization Set-Cookie
          if (!value.startsWith(" refreshToken") && !value.startsWith(" Authorization"))
            continue;

        }

        SetHeader(key, value); // add the header as a key-value pair
      }
    }
  }

  for (const auto &pair : HeadersMap)
  {

    Serial.print(pair.first);
    Serial.print(": ");
    Serial.println(pair.second);
  }

  Serial.println("...Parsing headers [DONE]");
}
