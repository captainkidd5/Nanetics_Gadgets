#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>
#include "CustomHeader.h"
#include <Arduino.h>
#include "Helpers.h"
#include "Server_configs.h"

enum class RequestType
{
  GET = 1,
  POST = 2,
  PUT = 3,
  DELETE = 4
};

String s_username = "";
String s_password = "";

String RequestTypeToString(RequestType r)
{
  switch (r)
  {
  case RequestType::GET:
    return "GET";
  case RequestType::POST:
    return "POST";
  case RequestType::PUT:
    return "PUT";
  case RequestType::DELETE:
    return "DELETE";
  default:
    throw std::invalid_argument("Invalid request type.");
  }
}

JsonObject ReadJson(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  Serial.println("Attempting to deserialize JSON...");
  DeserializationError error = deserializeJson(json, client);

  if (error)
  {
   Serial.println("...Attempting to deserialize JSON [FAILED]");

    Serial.println(error.c_str());
    throw std::invalid_argument("Bad Json");
  }

  JsonObject root_0 = json[0];
  return root_0;
}

bool isSuccessCode(int statusCode)
{
  return statusCode >= 200 && statusCode < 300;
}



CustomHeader ReadHeaders(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  CustomContentType contentType = CustomContentType::None;

  bool contentIsPlainText = false;
  Serial.println("Reading headers...");
  CustomHeader headers;

  while (client.connected())
  {
    String line = client.readStringUntil('\n'); // HTTP headers
    Serial.println(line);
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
      headers.StatusCode = statusCode;
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
      if(!contentIsPlainText){
        contentType = CustomContentType::JSON;
      }
      headers.ContentType = contentType;
    }
    else
    {
      int separatorIndex = line.indexOf(':');
      if (separatorIndex != -1)
      { // check if the line has a ':' separator
        String key = line.substring(0, separatorIndex);
        String value = line.substring(separatorIndex + 1);
        Serial.println("Setting Header: ");
        Serial.println("Key: " + key);
        Serial.println("Value: " + value);

        headers.setHeader(key, value); // add the header as a key-value pair to the Headers object
      }
    }
  }
  Serial.println("...Reading headers [DONE]");

  return headers;
}

String ReadPlainText(WiFiClientSecure &client)
{
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
