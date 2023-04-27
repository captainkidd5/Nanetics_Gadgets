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

const String BaseEndPoint = "/devices";

void PostRegisterDevice(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  const String FullEndPoint = BaseEndPoint + "/RegistrationRequest";
  client.setTimeout(30000);
  int conn = client.connect(serverUri, serverPort);
  if (conn == 1)
  {
    try
    {

      String macAddress = String(ESP.getEfuseMac());
      // Serial.println("Hardware id is " + macAddress);
      json.clear();

      json["deviceHardWareId"] = macAddress;

      String payload;
      boolean success = SendRequest(RequestType::POST, FullEndPoint, payload, client, json);

      Headers headers = ReadHeaders(client, json);
      switch (headers.ContentType)
      {
      case CustomContentType::PlainText:
      {

        String content = ReadPlainText(client);
        break;
      }

      case CustomContentType::JSON:
      {
        JsonObject root_0 = ReadJson(client, json);
        break;
      }
      }
    }

    catch (const std::exception &e)
    {
      json.clear();

      Serial.print("Exception caught: ");
      Serial.println(e.what());
    }
  }
}
