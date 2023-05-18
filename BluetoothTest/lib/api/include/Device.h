#ifndef DEVICE
#define DEVICE

#include <WiFiClientSecure.h>

bool GetIsRegistered(WiFiClientSecure &client);
void PostRegisterDevice(WiFiClientSecure &client);

#endif