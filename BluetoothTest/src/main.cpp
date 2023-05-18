
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WifiConnectionCustom.h>
#include <WiFiClientSecure.h>
#include "IoTMain.h"
#include <Preferences.h>
#include "Device.h"


WiFiClientSecure wifiClient;



void setup()
{

  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  while (!Serial)
  {
  }
  Serial.println("Booting up...");
//wifiClient.setNoDelay(true);
  setupWifi(wifiClient);
  bool isRegistered = GetIsRegistered(wifiClient);
  delay(2500);

  if (!isRegistered)
  {

    Serial.println("Device not registered, attempting to register");
    PostRegisterDevice(wifiClient);
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
   // senseLoop();
    // ApiLoop(wifiClient, json);
      loopIoT();
  }
  delay(4000);

  // put your main code here, to run repeatedly:
}
