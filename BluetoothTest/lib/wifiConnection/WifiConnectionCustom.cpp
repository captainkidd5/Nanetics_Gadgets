#include <FS.h>          //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#ifdef ESP32
#include <SPIFFS.h>
#endif

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include "HttpHelpers.h"
#include "Helpers.h"
#include "Auth.h"
const bool shouldWipeFileSystemOnBoot = false;
const bool shouldWipeWifiCredentialsOnBoot = false;
// custom parameters with validation: https://github.com/tzapu/WiFiManager/issues/736
//  define your default values here, if there are different values in config.json, they are overwritten.
char email[48] = "email@gmail.com";
char password[48] = "PASSWORD";

// flag for saving data
bool shouldSaveConfig = true;
bool hasWifiCredentials = true;

// callback notifying us of the need to save config
void saveConfigCallback()
{

  Serial.println("Should save config = true");
  shouldSaveConfig = true;
}

void setupWifi(WiFiClientSecure &client, DynamicJsonDocument &json)
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  //   // clean FS, for testing
  if (shouldWipeFileSystemOnBoot)
  {
    Serial.println("Wiping FS...");
    SPIFFS.format();
    Serial.println("...Wiping FS [DONE]");
  }

  //   // The extra parameters to be configured (can be either global or just in the setup)
  //   // After connecting, parameter.getValue() will get you the configured value
  WiFiManagerParameter e_mail("email", "Email", email, 48, "type=\"email\" required");
  WiFiManagerParameter pass_word("password", "Password", password, 48, "type= required");

  //   // WiFiManager
  //   // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //   // set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  wifiManager.addParameter(&e_mail);
  wifiManager.addParameter(&pass_word);

  if (shouldWipeWifiCredentialsOnBoot)
  {
    //   // reset settings - for testing
    Serial.println("Resetting WiFi Credentials...");
    wifiManager.resetSettings();
    Serial.println("...Resetting WiFi Credentials [DONE]");
  }

  //   // set minimu quality of signal so it ignores AP's under that quality
  //   // defaults to 8%
  wifiManager.setMinimumSignalQuality();

  //   // sets timeout until configuration portal gets turned off
  //   // useful to make it all retry or go to sleep
  //   // in seconds
  wifiManager.setTimeout(120);

  //   // fetches ssid and pass and tries to connect
  //   // if it does not connect it starts an access point with the specified name
  //   // here  "AutoConnectAP"
  //   // and goes into a blocking loop awaiting configuration
  // TODO: Randomize these for each unit and print on packaging
  if (!wifiManager.autoConnect("NaneticsAp", "password"))
  {
    Serial.println("...Failed to connect and hit timeout");
    delay(3000);
    //     // reset and try again, or maybe put it to deep sleep
    // ESP.restart();
    delay(5000);
  }
  client.setCACert(root_ca);

  hasWifiCredentials = true;
  //   // if you get here you have connected to the WiFi
  Serial.println("...Connected to wifi");

  //   // read updated parameters

  strcpy(email, e_mail.getValue());
  strcpy(password, pass_word.getValue());
  String usrnm = email;
  String psword = password;

  if (usrnm == "email@gmail.com" && psword == "PASSWORD")
  {
    // we autoconnected so the default values were not changed,
    // we should assume that we have a refresh token now.

    //   // read configuration from FS json
    Serial.println("Mounting FS...");
    std::map<String, String> myDict = {
        {"token", ""}};
    bool retrievedSPIIFS = retrieveSPIIFSValue(&myDict);


    if (retrievedSPIIFS)
      s_refreshToken = myDict["token"];
    else
    {
      Serial.println("Unable to retrieve refresh token from spiffs (wificonnectioncustom.cpp)");
      Serial.println("Resetting wifi settings...");
    wifiManager.resetSettings();

    }
  }
  else
  {
    // else we need to send our username and password to auth login endpoint and grab
    // the refresh token

    if (WiFi.status() == WL_CONNECTED)
    {
      delay(1200);
  client.setTimeout(30000);

      bool loginSuccess = PostLogin(client, json, usrnm, psword);
      if (loginSuccess)
        Serial.println("Successfully logged in and stored refresh token.");
      else
        Serial.println("Problem logged in and stored refresh token.");




    }
  }
char ssid[48];
char wifiPass[48];

  strcpy(ssid, WiFi.SSID().c_str());
  strcpy(wifiPass, WiFi.psk().c_str());
  //store wifi ssid and password for iot hub usage.
   std::map<String, String> wifiDict = {
        {"ssid", ssid},
        {"wifiPass",wifiPass}};
  storeSPIFFSValue(&wifiDict);
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}
