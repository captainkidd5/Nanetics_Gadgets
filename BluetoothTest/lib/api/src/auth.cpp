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

const String BaseEndPoint = "/auth";

bool PostLogin(WiFiClientSecure &client, DynamicJsonDocument &json, String email, String passWord)
{
  ResponseObject responseObj;

  const String FullEndPoint = BaseEndPoint + "/login";

  json.clear();

  json["email"] = email;
  json["password"] = passWord;
  // Do not send refresh token with login request, because we don't have one yet
  bool success = SendRequest(RequestType::POST, FullEndPoint, client, json, responseObj, false);

  if (!success)
    return false;
  // Refresh token is always stored as a header
  String rToken = ParseSetCookie(responseObj.header);
  if (rToken == "")
  {
    Serial.println("Unable to retrieve REFRESH token value from HEADERS");
    return false;
  }
  std::map<String, String> myDict;
  myDict["refreshToken"] = rToken;
  // Store refresh token in file system
  storeSPIFFSValue(&myDict);
  retrieveSPIIFSValue(&myDict);

  Serial.println("Stored refresh token is " + myDict["refreshToken"]);

  return true;

}
