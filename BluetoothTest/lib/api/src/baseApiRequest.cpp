
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include "HttpHelpers.h"
#include "Helpers.h"
#include "Headers.h"
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
bool SendRequest(RequestType reqType, String fullEndPoint,
                 WiFiClientSecure &client, DynamicJsonDocument &json, bool includeRefreshToken = true)
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



    if (includeRefreshToken)
    {
        std::map<String, String> myDict = {
            {"token", ""}};

        if (retrieveSPIIFSValue(&myDict))
        {
            // client.println("Cookie: refreshToken=Bearer " + myDict["token"] + "\r\n");
            client.println("refreshToken: " + myDict["token"] + "\r\n");
        }
        else
        {
            Serial.println("Unable to retrieve refresh token value");
        }
    }
    client.println(F("Connection: close"));

    if (!json.isNull())
    {
        SendJsonPayload(payload, client, json);
    }
    client.flush();
    Headers headers = ParseHeaders(client);

    if (!isSuccessCode(headers.StatusCode))
    {
        Serial.println("Request unsuccessful " + headers.StatusCode);
        return false;
    }
    Serial.println("Request Successful " + headers.StatusCode);
    Serial.println("---------[END SEND REQUEST " + fullEndPoint + "]------------");

    Serial.println("");

    // Serial.println("...Sending new " + requestType + " request to endpoint " + full + " [DONE]");

    return true;
}

// Attempt to grab a new refresh token, called whenever we get a 401 unauthorized.
// Returns true if we are returned a new refresh token. Returns false otherwise, and
// removes username and password credentials
bool PostRefresh(WiFiClientSecure &client, DynamicJsonDocument &json)
{
    const String FullEndPoint = "/auth/refresh";
    client.setTimeout(30000);
    int conn = client.connect(serverUri, serverPort);
    if (conn == 1)
    {

        json.clear();

        // Do not send refresh token with login request, because we don't have one yet
        bool success = SendRequest(RequestType::POST, FullEndPoint, client, json, true);

        if (!success)
            return false;

        Headers headers = ParseHeaders(client);

        // if(root_0.isNull()){

        // }

        return true;
    }
    return false;
}