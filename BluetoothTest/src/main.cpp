
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WifiConnectionCustom.h>
#include <WiFiClientSecure.h>
#include <azureIotConnection.h>
#include <Preferences.h>
// #include <ServerConnection.h>

#include "ServerConnection.h"


WiFiClientSecure wifiClient;
DynamicJsonDocument json(1024);


void setup()
{
  if (!hasWifiCredentials)
  {
    Serial.begin(115200);
    // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
    while(!Serial){}
    Serial.println("Booting up...");
 
    setupWifi(wifiClient,json);
    hasWifiCredentials = true;
  }

  //setupIotConnection();


}

void loop()
{

  if (hasWifiCredentials)
  {

    //ApiLoop(wifiClient, json);
    // IotLoop();
  }
    delay(1000);

  // put your main code here, to run repeatedly:
}
