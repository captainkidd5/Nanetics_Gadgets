#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include "Server_configs.h"
#include "HttpHelpers.h"
#include <iostream>
#include "Helpers.h"
#include "Headers.h"
#include "BaseApiRequest.h"
#include "ResponseObject.h"
const String BaseEndPoint = "/devices";

bool GetIsRegistered(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  ResponseObject responseObj;
  String macAddress = String(ESP.getEfuseMac());
  const String FullEndPoint = BaseEndPoint + "/isRegistered?hardwareId=" + macAddress;

  json.clear();

  bool success = SendRequest(RequestType::GET, FullEndPoint, client, json, responseObj);

  if (responseObj.jsonDictionary["isRegistered"] == "")
  {
    Serial.println("No value found for is registered");
    return false;
  }
  else if (responseObj.jsonDictionary["isRegistered"] == "false")
  {
    // Serial.println("Not registered!");
    return false;
  }
  else if (responseObj.jsonDictionary["isRegistered"] == "true")
  {
    Serial.println("IS registered!");
    return true;
  }
  else
  {
    Serial.println("Uhhh" + responseObj.jsonDictionary["isRegistered"]);
  }

  return false;
}
void PostRegisterDevice(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  ResponseObject responseObj;

  const String FullEndPoint = BaseEndPoint + "/RegistrationRequest";

  // Get the MAC address as a string
  String macAddress = String(ESP.getEfuseMac());
  // Convert the string to a ulong
  unsigned long long macAddressUlong = strtoull(macAddress.c_str(), NULL, 16);
  // Add the MAC address as a ulong to the JSON object
  json.clear();
  json["deviceHardWareId"] = "343242323";

  bool success = SendRequest(RequestType::POST, FullEndPoint, client, json, responseObj,true);

  if (success)
  {
    Serial.println("Device registered");
  }
  else{
    Serial.println("Unable to register device");
  }
}
