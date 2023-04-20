#ifndef SERVER_CONNECTION
#define SERVER_CONNECTION

#include <Arduino.h>
#include <WiFiManager.h> 
#include <WiFiClientSecure.h>


void SetupServerCredentials(const char* sName);
  void apiLoop(WiFiClientSecure& client);

  #endif