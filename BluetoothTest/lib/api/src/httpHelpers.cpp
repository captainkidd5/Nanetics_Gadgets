#include <ArduinoJson.h>
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

void AppendHeader(RequestType reqType, WiFiClientSecure &client, String key, String value)
{

  if (reqType == RequestType::POST)
  {
    client.print(key);
    client.println(value);
    return;
  }

  Serial.println("Appending header... " + key + value);
  Serial.println("Appending header... " + key + value);
  client.println(key + value + "\r\n");
}

JsonObject ReadJson(WiFiClientSecure &client)
{
  // Serial.print("Attempting to deserialize JSON...");

  if (client.available())
  {
    s_jsonDoc.clear();
    // Clear the JSON document before deserializing


    // Reserve memory for the JSON document to prevent overflow
    size_t capacity = client.available();
    DeserializationError error = deserializeJson(s_jsonDoc, client);

    if (error || !s_jsonDoc.is<JsonObject>())
    {
      // If there was an error, or if the JSON is not an object, print an error message
      Serial.print("JSON deserialization error: ");
      Serial.println(error.c_str());
      return JsonObject(); // Return an empty object to indicate failure
    }
    else
    {
      JsonObject root = s_jsonDoc.as<JsonObject>(); // Get the root object

      return root; // Return the root object
    }
  }
  else
  {
    Serial.println("No data available");
    return JsonObject(); // Return an empty object to indicate failure
  }
}

//Fills dictionary reference with keys and values found in json document

void GetJsonDictionary(WiFiClientSecure &client, std::map<String, String> &myDict)
{
  JsonObject root = ReadJson(client);
  // Iterate through all key-value pairs in the JSON object
  for (JsonPair pair : root)
  {
    // Get the key as a string
    String key = pair.key().c_str();
    // Get the value as a string, boolean, or other type depending on the JSON data type
    if (pair.value().is<bool>())
    {
      bool value = pair.value().as<bool>();
      // Add the key-value pair to the dictionary
      myDict[key] = value ? "true" : "false";
    }
    else
    {
      String value = pair.value().as<String>();
      // Add the key-value pair to the dictionary
      myDict[key] = value;
    }

    Serial.println("Json Key:" + key + " value:" + myDict[key]);
  }
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
String ParseSetCookie(Headers &headers)
{

  Serial.println("Parsing Set Cookie...");
  String value = "";

  String setCookieVal = headers.GetHeader("Set-Cookie");
  if (setCookieVal == "")
    Serial.println("Set-Cookie has no value!");

  int start = setCookieVal.indexOf('=') + 1; // Find the start index of the token value
  int end = setCookieVal.indexOf(';');       // Find the end index of the token value

  value = setCookieVal.substring(start, end);
  Serial.println("Parsed token is " + value);
  return value;
}
String ReadPlainText(WiFiClientSecure &client)
{
  Serial.println("reading plaintext body...");
  String content = client.readStringUntil('\n');
  Serial.println("content is before" + content);
  int contentLength = content.toInt();
  Serial.println("content length is " + contentLength);

  int bytesRead = content.length() + 1; // add 1 for the '\n' character
  while (bytesRead < contentLength)
  {
    String line = client.readStringUntil('\n');
    Serial.println("line:" + line);

    bytesRead += line.length() + 1; // add 1 for the '\n' character
    content += line;
  }
  Serial.println("Content body: " + content);
  return content;
}
