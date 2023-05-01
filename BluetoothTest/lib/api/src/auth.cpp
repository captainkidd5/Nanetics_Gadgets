#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include "Server_configs.h"
#include "HttpHelpers.h"
#include <iostream>
#include "Helpers.h"
#include "Headers.h"
#include "BaseApiRequest.h"

const String BaseEndPoint = "/auth";

extern String s_username;
extern String s_password;
bool PostLogin(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  const String FullEndPoint = BaseEndPoint + "/login";
  client.setTimeout(30000);
  int conn = client.connect(serverUri, serverPort);
  if (conn == 1)
  {

    json.clear();

    json["email"] = s_username;
    json["password"] = s_password;
    // Do not send refresh token with login request, because we don't have one yet
    Serial.println("Sending login");
    bool success = SendRequest(RequestType::POST, FullEndPoint, client, json, false);

    Headers headers = ParseHeaders(client);
      String response = client.readString();
  Serial.println("Response body:");
  Serial.println(response);
    JsonObject root_0 = ReadJson(client, json);

    return success;
  }
  return false;
}
