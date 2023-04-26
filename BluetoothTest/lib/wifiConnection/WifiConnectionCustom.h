#ifndef _SETUPWIFICUSTOM_H
#define _SETUPWIFICUSTOM_H

#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#ifdef ESP32
  #include <SPIFFS.h>
#endif

#include <ArduinoJson.h>    

extern bool hasWifiCredentials;
void setupWifi(DynamicJsonDocument& json);

#endif // _SETUPWIFI_H
