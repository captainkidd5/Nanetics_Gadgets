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
  unsigned long long macAddressUlong = strtoull(macAddress.c_str(), NULL, 16);

  const String FullEndPoint = BaseEndPoint + "/isRegistered?hardwareId=" + macAddressUlong;

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
  json["deviceHardWareId"] = macAddressUlong;

  bool success = SendRequest(RequestType::POST, FullEndPoint, client, json, responseObj, true);

  if (success)
  {
    Serial.println("Device registered");
    std::map<String, String> myDict;
    String x509Thumbprint = responseObj.header.GetHeader("x509Thumbprint");
    if (x509Thumbprint == "")
    {
      Serial.println("Unable to retrieve x509 thumbprint");
    }
    String assignedId = responseObj.header.GetHeader("assignedId");
    if (assignedId == "")
    {
      Serial.println("Unable to retrieve assignedId");
    }

    myDict["x509Thumbprint"] = x509Thumbprint;
    myDict["assignedId"] = assignedId;

    storeSPIFFSValue(&myDict);
    retrieveSPIIFSValue(&myDict);

    Serial.println("Stored x509Thumbprint token is " + myDict["x509Thumbprint"]);
    Serial.println("Stored assignedId token is " + myDict["assignedId"]);
  }
  else
  {
    Serial.println("Unable to register device");
  }
}
