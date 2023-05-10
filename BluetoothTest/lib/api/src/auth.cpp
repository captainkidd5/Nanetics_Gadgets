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
  const String FullEndPoint = BaseEndPoint + "/login";
  int conn = client.connect(serverUri, serverPort);
  if (conn == 1)
  {

    json.clear();

    json["email"] = email;
    json["password"] = passWord;
    // Do not send refresh token with login request, because we don't have one yet
    bool success = SendRequest(RequestType::POST, FullEndPoint, client, json, false);

//Refresh token is always stored as a header
    Headers headers = ParseHeaders(client);
String rToken = ParseSetCookie(headers);
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
    client.stop();

    return success;
  }
  return false;
}
