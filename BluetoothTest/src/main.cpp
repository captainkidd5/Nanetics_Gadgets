#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WifiConnectionCustom.h>
#include <ServerConnection.h>
#include <WiFiClientSecure.h>
#include <azureIotConnection.h>
#include <Preferences.h>

WiFiClientSecure wifiClient;
const size_t capacity = JSON_OBJECT_SIZE(4) + 48;
DynamicJsonDocument json(1024);

void setupWifiConnection(String wifiName, String wifiPass)
{
  // Initialize serial and wait for port to open:

  WiFi.mode(WIFI_STA);
  // Configures static IP address

  WiFi.begin(wifiName.c_str(), wifiPass.c_str());
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.flush();
}

void setup()
{

  if (!hasWifiCredentials)
  {
    Serial.begin(9600);
    // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
    delay(4500);
    Serial.println("Booting up...");
 
    setupWifiTwo(json);
  }

  setupWifiConnection(WiFi.SSID(), WiFi.psk());
  setupIotConnection();
}

void loop()
{
  if (hasWifiCredentials)
  {
    Serial.println("in loop");

    //apiLoop(wifiClient, json);
    // IotLoop();
  }
  // put your main code here, to run repeatedly:
}
