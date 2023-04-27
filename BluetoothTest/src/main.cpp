#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WifiConnectionCustom.h>
#include <WiFiClientSecure.h>
#include <azureIotConnection.h>
#include <Preferences.h>
#include <Server_configs.h>
#include <ServerConnection.h>


WiFiClientSecure wifiClient;
DynamicJsonDocument json(64);

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

}

void setup()
{

  if (!hasWifiCredentials)
  {
    Serial.begin(115200);
    // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
    while(!Serial){}
    Serial.println("Booting up...");
 
    setupWifi(json);
  }

  setupWifiConnection(WiFi.SSID(), WiFi.psk());
  //setupIotConnection();
size_t size = ESP.getMaxAllocHeap();
Serial.println("Available space is " + String(size));
     wifiClient.setCACert(root_ca);

}

void loop()
{
  if (hasWifiCredentials)
  {
    //Serial.println("in loop");

    apiLoop(wifiClient, json);
    // IotLoop();
  }
    delay(1000);

  // put your main code here, to run repeatedly:
}
