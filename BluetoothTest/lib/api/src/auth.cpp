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

bool PostLogin(WiFiClientSecure &client, String email, String passWord)
{
  ResponseObject responseObj;

  const String FullEndPoint = BaseEndPoint + "/login";

  s_jsonDoc.clear();

  s_jsonDoc["email"] = email;
  s_jsonDoc["password"] = passWord;
  bool success = SendRequest(RequestType::POST, FullEndPoint, client, responseObj, false);

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
