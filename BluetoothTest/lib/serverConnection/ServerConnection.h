#include <Arduino.h>
#include <WiFiManager.h> 
#include <WiFiClientSecure.h>


void SetupServerCredentials(const char* sName);
  void apiLoop(WiFiClientSecure& client);