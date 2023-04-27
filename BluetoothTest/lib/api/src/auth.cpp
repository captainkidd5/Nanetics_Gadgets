#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include "Server_configs.h"
#include "HttpHelpers.h"
#include <iostream>
#include "Helpers.h"
#include "CustomHeader.h"

const String BaseEndPoint = "/auth";

extern String s_username;
extern String s_password;
void PostLogin(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  const String FullEndPoint = BaseEndPoint + "/login";
  client.setTimeout(30000);
  int conn = client.connect(serverUri, serverPort);
  if (conn == 1)
  {
    try
    {

      String macAddress = String(ESP.getEfuseMac());
      // Serial.println("Hardware id is " + macAddress);
      json.clear();

      json["email"] = s_username;
      json["password"] = s_password;
        Serial.println("Username is " + s_username);
        Serial.println("Password is " + s_password);
      String payload;
      boolean success = SendRequest(RequestType::POST, FullEndPoint, payload, client, json);

      Headers headers = ReadHeaders(client, json);
     

      
        JsonObject root_0 = ReadJson(client, json);
      
      
    }

    catch (const std::exception &e)
    {
      json.clear();

      Serial.print("Exception caught: ");
      Serial.println(e.what());
    }
  }
}
