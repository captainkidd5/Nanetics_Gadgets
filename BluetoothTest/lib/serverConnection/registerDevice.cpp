#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <server_configs.h>
#include <httpHelpers.h>
#include <iostream>
#include "../helpers/Helpers.h"
#include "../types/customHeader.h"

boolean PostRegisterDevice(WiFiClientSecure &client, DynamicJsonDocument &json)
{

  client.setTimeout(30000);
  int conn = client.connect(serverUri, serverPort);
  if (conn == 1)
  {
    try
    {
      Serial.println();
      Serial.print("Sending JSON body...");

      String macAddress = String(ESP.getEfuseMac());
      Serial.println("Hardware id is " + macAddress);
      json.clear();

      json["deviceHardWareId"] = macAddress;

      String payload;
      serializeJson(json, payload);
      Serial.println("Sending payload...");
      String uri = serverUri;
      uri = "https://" + uri;
      client.println("POST " + uri + "/Devices/RegistrationRequest HTTP/1.1");
      client.println(String("Host: ") + serverUri);
      client.println(F("Connection: close"));
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(payload.length());
      client.println();
      client.println(payload);
      json.clear();

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
}
