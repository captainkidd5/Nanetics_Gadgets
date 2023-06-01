#include <ArduinoJson.h>
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
#include "iot_configs.h"

const String BaseEndPoint = "/devices";

bool GetIsRegistered(WiFiClientSecure &client)
{
  ResponseObject responseObj;
  String macAddress = String(ESP.getEfuseMac());
  Serial.println("mac address is " + macAddress);
 // unsigned long macAddressUlong = strtoull(macAddress.c_str(), NULL, 16);

  const String FullEndPoint = BaseEndPoint + "/isRegistered?hardwareId=" + macAddress;

  s_jsonDoc.clear();

  bool success = SendRequest(RequestType::GET, FullEndPoint, client,s_jsonDoc, responseObj);

  if (responseObj.jsonDictionary["isRegistered"] == "")
  {
    Serial.println("No value found for is registered");
    return false;
  }
  else if (responseObj.jsonDictionary["isRegistered"] == "false")
  {
    return false;
  }
  else if (responseObj.jsonDictionary["isRegistered"] == "true")
  {
    Serial.println("IS registered!");
    Serial.println("HAS assignedId of " + responseObj.jsonDictionary["assignedId"]);


   s_assigned_id = responseObj.jsonDictionary["assignedId"];
     s_primaryKey = responseObj.jsonDictionary["primaryKey"];
     s_scope_id = responseObj.jsonDictionary["idScope"];

    //Serial.println("s_primary key is " + s_primaryKey);

    return true;
  }


  return false;
}
void PostRegisterDevice(WiFiClientSecure &client)
{
  ResponseObject responseObj;

  const String FullEndPoint = BaseEndPoint + "/RegistrationRequest";

  // Get the MAC address as a string
  String macAddress = String(ESP.getEfuseMac());
  // Add the MAC address as a ulong to the JSON object
  s_jsonDoc.clear();
  s_jsonDoc["deviceHardWareId"] = macAddress;
  s_jsonDoc["templateName"] = TEMPLATE_NAME;


  if (SendRequest(RequestType::POST, FullEndPoint, client,s_jsonDoc, responseObj, true))
  {
    Serial.println("Device registered");
 if (responseObj.jsonDictionary["assignedId"] == "")
  {
    Serial.println("No value found for is registered");
  }
  else 
  {
      s_assigned_id = responseObj.jsonDictionary["assignedId"];
      s_primaryKey = responseObj.jsonDictionary["primaryKey"];
     s_scope_id = responseObj.jsonDictionary["idScope"];

   // Serial.println("s_primary key is " + s_primaryKey);


  }

  }
  else
  {
    Serial.println("Unable to register device");
  }
}
