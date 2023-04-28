
#include <ArduinoJson.hpp>
#include <WiFiClientSecure.h>
#include "HttpHelpers.h"
#include "Helpers.h"

boolean SendRequest(RequestType reqType, String fullEndPoint, String payload,
                    WiFiClientSecure &client, DynamicJsonDocument &json, bool includeRefreshToken = true)
{

    String requestType = RequestTypeToString(reqType);

    String uri = serverUri;
    uri = " https://" + uri;
    String full = requestType + uri + fullEndPoint + " HTTP/1.1";
    client.println(full);
    client.println(String("Host: ") + serverUri);
    client.println(F("Connection: close"));

    if (includeRefreshToken)
    {
        String refreshTokenVal = "";
        if (retrieveFlashValue("refreshToken", refreshTokenVal))
        {
            client.println("Cookie: refreshToken=" + refreshTokenVal + "\r\n");
        }
        else
        {
            Serial.println("Unable to retrieve refresh token value");
        }
    }

    if (payload.length() > 0)
    {
        SendJsonPayload(payload, client, json);
    }

    return true;
}

// Sends a json payload in the body of the http request
void SendJsonPayload(String payload,
                     WiFiClientSecure &client, DynamicJsonDocument &json)
{
    client.println("Content-Type: application/json");

    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    serializeJson(json, payload);
    Serial.println("Sending payload...");
    Serial.println("Payload is " + payload);
    client.println(payload);
    Serial.println("...Sending Payload [DONE]");
    json.clear();
}
// Attempt to grab a new refresh token, called whenever we get a 401 unauthorized.
// Returns true if we are returned a new refresh token. Returns false otherwise, and
// removes username and password credentials
boolean PostRefresh(WiFiClientSecure &client, DynamicJsonDocument &json)
{
    const String FullEndPoint = "/auth/refresh";
    client.setTimeout(30000);
    int conn = client.connect(serverUri, serverPort);
    if (conn == 1)
    {

        json.clear();

        String payload;
        // Do not send refresh token with login request, because we don't have one yet
        boolean success = SendRequest(RequestType::POST, FullEndPoint, payload, client, json, true);

        if (!success)
        {
            return false;
        }
        CustomHeader headers = ReadHeaders(client, json);

        JsonObject root_0 = ReadJson(client, json);
        return true;
    }
    return false;
}