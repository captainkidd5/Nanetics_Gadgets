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

bool PostLogin(WiFiClientSecure &client, DynamicJsonDocument &json, String userName, String passWord)
{
  const String FullEndPoint = BaseEndPoint + "/login";
  client.setTimeout(30000);
  int conn = client.connect(serverUri, serverPort);
  if (conn == 1)
  {

    json.clear();

    json["email"] = userName;
    json["password"] = passWord;
    // Do not send refresh token with login request, because we don't have one yet
    Serial.println("Sending login");
    bool success = SendRequest(RequestType::POST, FullEndPoint, client, json, false);
    json.clear();
/////////////
   JsonObject root_0 = ReadJson(client, json);
     String jsonString;
          serializeJson(root_0, jsonString);
          Serial.println(jsonString);
        String tokenResponse = root_0["token"];
    Serial.println("response is " + tokenResponse);


    ////////
    Headers headers = ParseHeaders(client);
    if(!isSuccessCode(headers.StatusCode)){
      Serial.println("...Unable to login");
      return false;
    }
    String response = client.readString();
    Serial.println("RESPONESE IS " + response);
    /////////
    //  JsonObject root_0 = ReadJson(client, json);
    //     String tokenResponse = root_0["token"];
    // Serial.println("response is " + tokenResponse);
    // std::map<String, String> myDict = {
    //     {"refreshToken",tokenResponse}};
        //////////
    std::map<String, String> myDict = {
        {"refreshToken", headers.GetHeader("token")}};
        //Store refresh token in file system.
    storeSPIFFSValue(&myDict);
    retrieveSPIIFSValue(&myDict);

    Serial.println("Stored refresh token is " + myDict["refreshToken"]);
    return success;
  }
  return false;
}
