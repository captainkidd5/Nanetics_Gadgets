#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include "Device.h"
#include "Auth.h"
// https://randomnerdtutorials.com/esp32-https-requests/

bool loginSuccess = false;
void ApiLoop(WiFiClientSecure &client, DynamicJsonDocument &json)
{

  if (WiFi.status() == WL_CONNECTED)
  {
    delay(10000);

  //  // PostRegisterDevice(client, json);
  //  if(!loginSuccess){
  //   loginSuccess =  PostLogin(client,json);
  //  }
  
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}