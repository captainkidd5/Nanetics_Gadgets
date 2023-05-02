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
  client.setTimeout(30000);
  int conn = client.connect(serverUri, serverPort);
  if (conn == 1)
  {

    json.clear();

    json["email"] = email;
    json["password"] = passWord;
    // Do not send refresh token with login request, because we don't have one yet
    Serial.println("Sending login");
    bool success = SendRequest(RequestType::POST, FullEndPoint, client, json, false);


    Headers headers = ParseHeaders(client);
    if(!isSuccessCode(headers.StatusCode)){
      Serial.println("...Unable to login");
      return false;
    }
     std::map<String, String> myDict;;
GetJsonDictionary(client,json, myDict);
     
        if(myDict["token"] == "")
        {
          Serial.println("Unable to retrieve token value from json");
          return false;
        }

        //Store refresh token in file system
    storeSPIFFSValue(&myDict);
   // retrieveSPIIFSValue(&myDict);

   // Serial.println("Stored refresh token is " + myDict["token"]);
    return success;
  }
  return false;
}
