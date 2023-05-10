
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include "HttpHelpers.h"
#include "Helpers.h"
#include "Headers.h"
#include "ResponseObject.h"
const byte _maxFailedAttempts = 7;

// Sends a json payload in the body of the http request
void SendJsonPayload(String payload,
                     WiFiClientSecure &client, DynamicJsonDocument &json)
{
    size_t sizeT = serializeJson(json, payload);
    String size = String(sizeT);

    client.print("Content-Length: ");
    Serial.println("Content length is " + size);
    client.println(payload.length());
    client.println();
    Serial.println("Sending payload...");
    Serial.print("Payload is " + payload);
    client.println(payload);
    Serial.println("...Sending Payload [DONE]");
    json.clear();
}

bool Send(RequestType reqType, String fullEndPoint,
          WiFiClientSecure &client, DynamicJsonDocument &json, bool includeAccessToken, ResponseObject &responseObj, bool includeRefreshToken = false)
{

    Serial.println("");
    Serial.println("---------[BEGIN SEND REQUEST " + fullEndPoint + "]------------");

    /// int remainingSpace = client.availableForWrite();
    // Serial.println("Available space in buffer is " + String(remainingSpace));
    String payload;
    String requestType = RequestTypeToString(reqType);
    String uri = serverUri;
    uri = " https://" + uri;
    String full = requestType + uri + fullEndPoint + " HTTP/1.0";
    Serial.println("Sending new " + requestType + " request to endpoint " + full + "...");

    client.println(full);

    client.println(String("Host: ") + serverUri);

    client.println("Content-Type: application/json");

    if (includeAccessToken)
    {
        std::map<String, String> myDict = {
            {"token", ""}};

        if (retrieveSPIIFSValue(&myDict))
            AppendHeader(client, "Authorization: Bearer ", myDict["token"]);
        else
            Serial.println("Unable to retrieve access token value");
    }
     if (includeRefreshToken)
    {
        std::map<String, String> myDict = {
            {"refreshToken", ""}};

        if (retrieveSPIIFSValue(&myDict))
            AppendHeader(client, "Cookie: refreshToken=", myDict["refreshToken"]);
        else
            Serial.println("Unable to retrieve refresh token value");
    }
    client.println(F("Connection: close"));

    if (!json.isNull())
    {
        SendJsonPayload(payload, client, json);
    }
    //client.flush() ensures all data is sent to the server. Should be called after all desired data is sent
    //but before the response is read
    client.flush();
    Headers headers = ParseHeaders(client);

    responseObj.header = headers;

    if (!isSuccessCode(headers.StatusCode))
    {
        return false;
    }
    GetJsonDictionary(client, json, responseObj.jsonDictionary);

    Serial.println("Request Successful " + headers.StatusCode);
    Serial.println("---------[END SEND REQUEST " + fullEndPoint + "]------------");

    Serial.println("");

    // Serial.println("...Sending new " + requestType + " request to endpoint " + full + " [DONE]");

    return true;
}

bool SendRequest(RequestType reqType, String fullEndPoint,
                 WiFiClientSecure &client, DynamicJsonDocument &json,
                 ResponseObject &responseObj, bool includeAccessToken = true)
{
    byte _failedAttempts = 0;

    int statusCode = 400;
    bool reqSucceeded = Send(reqType, fullEndPoint, client, json, includeAccessToken,responseObj);
    //client.stop() Should be called after response is read. If this is not called, we get that
    //annoying unknown ssl error
    client.stop();

    if (reqSucceeded)
        return true;

    if (statusCode = 401)
    {
        json.clear();
responseObj.jsonDictionary.clear();
responseObj.header.ClearHeaders();
        bool refreshSuccess = Send(RequestType::POST, "/auth/refresh", client, json, false,responseObj,true);

        if (refreshSuccess)
        {
         Serial.println("Refresh success");
            if (responseObj.jsonDictionary["token"] == "")
            {
                Serial.println("Unable to retrieve ACCESS token value from json");
                return false;
            }

            bool storedValue = storeSPIFFSValue(&responseObj.jsonDictionary);

            if (storedValue)
            {
                Serial.println("Stored access token");
                return true;
            }
            // Refresh token valid, Access token is good again, retry request
            bool attempt2Success = Send(reqType, fullEndPoint, client, json, includeAccessToken, responseObj);

            return attempt2Success;
        }
        else
        {
            // user needs to log in again
            Serial.println("User must log in again!");
        }
    }
    return false;
}