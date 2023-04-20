#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <server_configs.h>
#include <httpHelpers.h>
#include <iostream>
#include <helpers.h>

void postRegisterDevice(WiFiClientSecure &client)
{

  client.setTimeout(30000); // 30 seconds timeout
  // need to use 443 for https
  int conn = client.connect(serverUri, serverPort);

  if (conn == 1)
  {

    try
    {

      Serial.println();
      Serial.print("Sending JSON body...");
      // Prepare the JSON payload
      const size_t capacity = JSON_OBJECT_SIZE(1) + 20;
      DynamicJsonDocument doc(capacity);
      String macAddress = String(ESP.getEfuseMac());
      Serial.println("Hardware id is " + macAddress);

      doc["deviceHardWareId"] = ESP.getEfuseMac();
      String payload;
      serializeJson(doc, payload);
      // Request
      Serial.println("Sending payload...");

      client.println("POST https://naneticsapi.azurewebsites.net/Devices/RegistrationRequest HTTP/1.0");
      client.println(String("Host: ") + serverUri);
      client.println(F("Connection: close"));
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(payload.length());
      client.println();
      client.println(payload);

      while (client.connected())
      {
        String line = client.readStringUntil('\n'); // HTTP headers
        if (line == "\r")
        {
          break;
        }
      }
      String line = client.readStringUntil('\n'); // payload first row
    }
  
  catch (const std::exception &e)
  {
    Serial.print("Exception caught: ");
    Serial.println(e.what());
  }
}
}
