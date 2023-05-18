
#ifndef BASE_API_REQUEST
#define BASE_API_REQUEST
#include <WiFiClientSecure.h>
#include "HttpHelpers.h"
#include "ResponseObject.h"

bool SendRequest(RequestType reqType, String fullEndPoint,
                 WiFiClientSecure &client,
                 ResponseObject &responseObj, bool includeAccessToken = true);

 #endif