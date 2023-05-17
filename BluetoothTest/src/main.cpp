
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WifiConnectionCustom.h>
#include <WiFiClientSecure.h>
#include "azureIotConnection.h"
#include <Preferences.h>
// #include <ServerConnection.h>

#include "ServerConnection.h"
#include "Device.h"

#include "SoilSensor.h"

WiFiClientSecure wifiClient;
DynamicJsonDocument json(1024);


void setup()
{

  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  while (!Serial)
  {
  }
  Serial.println("Booting up...");
//wifiClient.setNoDelay(true);
  setupWifi(wifiClient, json);
  bool isRegistered = GetIsRegistered(wifiClient, json);
  delay(2500);

  if (!isRegistered)
  {
DynamicJsonDocument json2(1024);

    Serial.println("Device not registered, attempting to register");
    PostRegisterDevice(wifiClient, json2);
  }
  hasWifiCredentials = true;
  setupIoT();
Serial.println("Device is setup. Begin Sensing...");
  // setupIotConnection();
}

void loop()
{

  if (hasWifiCredentials)
  {
    senseLoop();
    // ApiLoop(wifiClient, json);
      loopIoT();
  }
  delay(4000);

  // put your main code here, to run repeatedly:
}
