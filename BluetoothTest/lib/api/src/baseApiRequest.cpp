
#include <WiFiClientSecure.h>
#include "HttpHelpers.h"
#include "Helpers.h"
#include "Headers.h"
#include "ResponseObject.h"

// Sends a json payload in the body of the http request
void SendJsonPayload(String payload,
                     WiFiClientSecure &client,const StaticJsonDocument<1024> &jsonDoc)
{
    size_t sizeT = serializeJson(jsonDoc, payload);
    String size = String(sizeT);

    client.print("Content-Length: ");
    Serial.println("Content length is " + size);

    Serial.println("Content length is " + payload.length());

    client.println(sizeT);
    // Empty line is necessary to separate headers from payload
    client.println();
    //Serial.println("Sending payload...");

    //Serial.print("Payload is " + payload);
    client.println(payload);
    //Serial.println("...Sending Payload [DONE]");
    //jsonDoc.clear();
}

bool Send(RequestType reqType, String fullEndPoint,
          WiFiClientSecure &client, bool includeAccessToken,const StaticJsonDocument<1024> &jsonDoc, ResponseObject &responseObj, bool includeRefreshToken = false)
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


        if (s_accessToken != "")
        {
            //Serial.println("access token is: " + s_accessToken);
            AppendHeader(reqType, client, "Authorization: Bearer ", s_accessToken);
        }
        else
        {
            Serial.println("Unable to retrieve access token value");
            responseObj.header.StatusCode = 401;
            client.flush();
            return false;
        }
    }
    if (includeRefreshToken)
    {
        std::map<String, String> myDict = {
            {"refreshToken", ""}};

        if (retrieveSPIIFSValue(&myDict))
            AppendHeader(reqType, client, "Cookie: refreshToken=", myDict["refreshToken"]);
        else
            Serial.println("Unable to retrieve refresh token value (if includeRefreshToken)");
    }

    if (!jsonDoc.isNull())
    {
        SendJsonPayload(payload, client, jsonDoc);
    }
    client.println(F("Connection: close"));

    // client.flush() ensures all data is sent to the server. Should be called after all desired data is sent
    // but before the response is read
    client.flush();
    Headers headers = ParseHeaders(client);

    responseObj.header = headers;

    if (responseObj.header.ContentType == CustomContentType::PlainText)
    {
        Serial.println("Content type was plain text");
        ReadPlainText(client);
    }
    else if (responseObj.header.ContentType == CustomContentType::JSON)
    {
        Serial.println("Content type was Json");

        GetJsonDictionary(client, responseObj.jsonDictionary);
    }

    if (!isSuccessCode(headers.StatusCode))
    {
        return false;
    }

    Serial.println("Request Successful " + headers.StatusCode);
    Serial.println("---------[END SEND REQUEST " + fullEndPoint + "]------------");

    Serial.println("");

    // Serial.println("...Sending new " + requestType + " request to endpoint " + full + " [DONE]");

    return true;
}

bool SendRequest(RequestType reqType, String fullEndPoint,
                 WiFiClientSecure &client,const StaticJsonDocument<1024> &jsonDoc,
                 ResponseObject &responseObj, bool includeAccessToken = true)
{

    int conn = client.connect(serverUri, serverPort);
    bool reqSucceeded;
    if (conn == 1)
    {

        reqSucceeded = Send(reqType, fullEndPoint, client, includeAccessToken, jsonDoc, responseObj);
        // client.stop() Should be called after response is read. If this is not called, we get that
        // annoying unknown ssl error
        client.stop();
    }
    if (reqSucceeded)
        return true;

    if (responseObj.header.StatusCode == 401 || responseObj.header.StatusCode == 408)
    {
        // create a new json doc here because the original request will need to retain
        // its json data if it is a post request with a body
        const StaticJsonDocument<1024> j2;
        responseObj.jsonDictionary.clear();
        responseObj.header.ClearHeaders();
        conn = client.connect(serverUri, serverPort);
        bool refreshSuccess;
        if (conn == 1)
        {
            refreshSuccess = Send(RequestType::GET, "/auth/refresh", client, false, j2, responseObj, true);
            client.stop();
        }
        if (refreshSuccess)
        {
            Serial.println("Refresh success");

            String accessToken = ParseSetCookie(responseObj.header);
            if (accessToken == "")
            {
                Serial.println("Unable to retrieve ACCESS token value from HEADERS");
                return false;
            }
s_accessToken = accessToken;
         

            // Refresh token valid, Access token is good again, retry request
            responseObj.jsonDictionary.clear();
            responseObj.header.ClearHeaders();
            conn = client.connect(serverUri, serverPort);
            bool refreshSuccess;
            if (conn == 1)
            {
                Serial.println("Reattempting request at endpoint " + fullEndPoint);
                bool attempt2Success = Send(reqType, fullEndPoint, client, includeAccessToken, jsonDoc, responseObj);
                Serial.println("Reattempting request: " + attempt2Success);
                client.stop();

                return attempt2Success;
            }
        }
        else
        {
            // user needs to log in again
            Serial.println("User must log in again!");
        }
    }
    return false;
}