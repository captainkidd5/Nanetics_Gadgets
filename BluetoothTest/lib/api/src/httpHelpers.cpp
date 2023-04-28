#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <map>
#include <string>
#include "CustomHeader.h"
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

boolean SendRequest(RequestType reqType, String fullEndPoint, String payload,
                    WiFiClientSecure &client, DynamicJsonDocument &json, bool includeRefreshToken = true)
{
  try
  {

    String requestType = RequestTypeToString(reqType);
    String payload;

    serializeJson(json, payload);
    Serial.println("Sending payload...");
    String uri = serverUri;
    uri = " https://" + uri;
    Serial.println("Payload is " + payload);
    String full = requestType + uri + fullEndPoint + " HTTP/1.1";
    Serial.println("Full is " + full);
    client.println(requestType + uri + fullEndPoint + " HTTP/1.1");
    client.println(String("Host: ") + serverUri);
    client.println(F("Connection: close"));
    client.println("Content-Type: application/json");

    if (includeRefreshToken)
    {
      client.println("Content-Type: application/json");
    }
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.println(payload);
    json.clear();
    Serial.println("...Sending Payload [DONE]");

    return true;
  }

  catch (const std::exception &e)
  {
    json.clear();

    Serial.print("Exception caught: ");
    Serial.println(e.what());
    return false;
  }
  return false;
}

bool isSuccessCode(int statusCode)
{
  return statusCode >= 200 && statusCode < 300;
}

Headers ReadHeaders(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  CustomContentType contentType = CustomContentType::None;

  bool contentIsPlainText = false;
  Serial.println("Reading headers...");
  Headers myHeaders;

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
      //myHeaders.ContentType =  contentTypeString;

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



        myHeaders.setHeader(key, value); // add the header as a key-value pair to the Headers object
      }
    }
  }
  Serial.println("...Reading headers [DONE]");

  return myHeaders;
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

JsonObject ReadJson(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  DeserializationError error = deserializeJson(json, client);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    throw std::invalid_argument("Bad Json");
  }

  JsonObject root_0 = json[0];
  return root_0;
  // Serial.println(root_0);

  // //  Get the Name:
  // const char *root_0_name = root_0["name"];
  // json.clear();
}