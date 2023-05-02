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
    // Clear the JSON document before deserializing
    json.clear();
    
    // Reserve memory for the JSON document to prevent overflow
    size_t capacity = client.available();
    DeserializationError error = deserializeJson(json, client);
    
    if (error || !json.is<JsonObject>())
    {
      // If there was an error, or if the JSON is not an object, print an error message
      Serial.print("JSON deserialization error: ");
      Serial.println(error.c_str());
      return JsonObject(); // Return an empty object to indicate failure
    }
    else
    {
      JsonObject root = json.as<JsonObject>(); // Get the root object
      
      // Check if the object has the expected keys or elements
      // if (!root.containsKey("key1") || !root.containsKey("key2") || !root.containsKey("key3") || root.size() != 3)
      // {
      //   Serial.println("JSON object has unexpected structure");
      //   return JsonObject(); // Return an empty object to indicate failure
      // }
      
      return root; // Return the root object
    }
  }
  else
  {
    Serial.println("No data available");
    return JsonObject(); // Return an empty object to indicate failure
  }
}

/// @brief Fills dictionary reference with keys and values found in json document
/// @param client 
/// @param json 
/// @param myDict 
void GetJsonDictionary(WiFiClientSecure &client, DynamicJsonDocument &json, std::map<String, String>& myDict)
{
  JsonObject root_0 = ReadJson(client, json);
  // Iterate through all key-value pairs in the JSON object
  for (JsonPair pair : root_0)
  {
    // Get the key and value as strings
    String key = pair.key().c_str();
    String value = pair.value().as<String>();
    // Add the key-value pair to the dictionary
    myDict[key] = value;
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
