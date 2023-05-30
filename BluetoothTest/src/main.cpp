
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WifiConnectionCustom.h>
#include <WiFiClientSecure.h>
#include "IoTMain.h"
#include <Preferences.h>
#include "Device.h"
#include "Helpers.h"

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
//Serial.println("(main0) assigned id is " + String(s_assigned_id));


  delay(2500);

  if (!isRegistered)
  {

    Serial.println("Device not registered, attempting to register");
    PostRegisterDevice(wifiClient);
    isRegistered = GetIsRegistered(wifiClient);
  }
  hasWifiCredentials = true;
//Serial.println("(main) assigned id is " + String(s_assigned_id));
  String _ssid = WiFi.SSID();
  Serial.println("3_ssid is " + _ssid);

  setupIoT();
Serial.println("Device is setup. Begin Sensing...");

}

void loop()
{

  if (hasWifiCredentials)
  {

      loopIoT();
  }
  delay(4000);

  // put your main code here, to run repeatedly:
}
