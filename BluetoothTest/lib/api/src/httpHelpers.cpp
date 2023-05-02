#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>
#include "Headers.h"
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

String s_refreshToken = "";

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
  
  if (client.available())
  {
    DeserializationError error = deserializeJson(json, client);
    if (error)
    {
      Serial.println("...Attempting to deserialize JSON [FAILED]");
      Serial.println(error.c_str());
    }
    else if (json.isNull())
    {
      Serial.println("JSON is empty");
    }
    else
    {
      JsonObject root_0 = json[0];
      return root_0;
    }
  }
  else
  {
    Serial.println("No data available");
  }
  
  return JsonObject();
}

bool isSuccessCode(int statusCode)
{
  return statusCode >= 200 && statusCode < 300;
}



Headers ParseHeaders(WiFiClientSecure &client)
{

  Headers headers;
  
    headers.ParseHeaders(client);
    

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
