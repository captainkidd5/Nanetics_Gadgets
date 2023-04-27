#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <server_configs.h>
#include <httpHelpers.h>
#include <iostream>
#include <helpers.h>
#include "../types/customHeader.h"

void PostRegisterDevice(WiFiClientSecure &client, DynamicJsonDocument &json)
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

        Serial.println("reading content body...");
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
        break;
      }

      case CustomContentType::JSON:
      {


        DeserializationError error = deserializeJson(json, client);

        if (error)
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
          return;
        }

        JsonObject root_0 = json[0];
        Serial.println(root_0);

        //  Get the Name:
        const char *root_0_name = root_0["name"];
        json.clear();

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
