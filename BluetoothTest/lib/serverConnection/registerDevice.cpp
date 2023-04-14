#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <server_configs.h>
#include <httpHelpers.h>
#include <iostream>
#include <helpers.h>
const char *servName;

void postRegisterDevice(WiFiClientSecure &client)
{
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
            doc["DeviceHardWareId"] = ESP.getEfuseMac();
            ;
            String payload;
            serializeJson(doc, payload);
            // Request
            client.println("POST /api/RegistrationRequest HTTP/1.1");
            client.println(String("Host: ") + serverUri);

            client.println("Content-Type: application/json");
            client.print("Content-Length: ");
            client.println(payload.length());
            client.println();
            client.println(payload);
            String response = "";
            while (client.available())
            {
                char c = client.read();
                response += c;
            }

            DynamicJsonDocument doc = deserialize(response);

            String assignedId = doc["assignedId"];
            String x509Thumbprint = doc["x509Thumbprint"];
            setSettings("AssignedId", assignedId.c_str());
            setSettings("X509Thumbprint", x509Thumbprint.c_str());

        }
        catch (const std::exception &ex)
        {
            std::cerr << "Caught exception: " << ex.what() << '\n';
        }
        client.stop();
    }
    else
    {
        client.stop();
        Serial.println("Connection Failed");
    }
}
