#include <FS.h>          //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#ifdef ESP32
#include <SPIFFS.h>
#endif

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include "HttpHelpers.h"

// custom parameters with validation: https://github.com/tzapu/WiFiManager/issues/736
//  define your default values here, if there are different values in config.json, they are overwritten.
char username[48] = "USERNAME";
char password[48] = "PASSWORD";
 extern String s_username;
 extern String s_password;
// flag for saving data
bool shouldSaveConfig = false;
bool hasWifiCredentials = false;
const bool shouldWipeFileSystemOnBoot = false;
const bool shouldWipeWifiCredentialsOnBoot = false;

// callback notifying us of the need to save config
void saveConfigCallback()
{

  Serial.println("Should save config = true");
  shouldSaveConfig = true;
}

void setupWifi(DynamicJsonDocument &json)
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  //   // clean FS, for testing
  if (shouldWipeFileSystemOnBoot)
  {
    Serial.println("Wiping FS...");
    SPIFFS.format();
    Serial.println("...Wiping FS [DONE]");
  }

  //   // read configuration from FS json
  Serial.println("Mounting FS...");

  try
  {

    if (SPIFFS.begin())
    {
      if (SPIFFS.exists("/config.json"))
      {
        Serial.println("...Mounting FS [DONE]");

        //  file exists, reading and loading
        Serial.println("reading config file");
        File configFile = SPIFFS.open("/config.json", "r");
        if (configFile)
        {
          Serial.println("opened config file");
          size_t size = configFile.size();
          // Allocate a buffer to store contents of the file.
          std::unique_ptr<char[]> buf(new char[size]);

          configFile.readBytes(buf.get(), size);

          DynamicJsonDocument json(1024);
          auto deserializeError = deserializeJson(json, buf.get());
          serializeJson(json, Serial);
          if (!deserializeError)
          {

            Serial.println("\nparsed json");
            strcpy(username, json["username"]);
            strcpy(password, json["password"]);
            s_username = username;
            s_password = password;
          }
          else
          {
            Serial.println("failed to load json config");
          }
          configFile.close();
        }
      }

      else
      {
        Serial.println("...Failed to mount FS");
      }
    }
  }
  catch (const std::exception &ex)
  {
    // code to handle the exception
    Serial.println(ex.what());
  }
  //   // end read

  //   // The extra parameters to be configured (can be either global or just in the setup)
  //   // After connecting, parameter.getValue() will get you the configured value
  //   // id/name placeholder/prompt default length
WiFiManagerParameter user_name("username", "User Email Address", username, 48,"type=\"email\" required");
WiFiManagerParameter pass_word("password", "Password", password, 48,"type=\"password\" required");

  //   // WiFiManager
  //   // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //   // set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //   // set static ip

  //   // add all your parameters here
  wifiManager.addParameter(&user_name);
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
  if (!wifiManager.autoConnect("AutoConnectAP", "password"))
  {
    Serial.println("...Failed to connect and hit timeout");
    delay(3000);
    //     // reset and try again, or maybe put it to deep sleep
    //ESP.restart();
    delay(5000);
  }

  hasWifiCredentials = true;
  //   // if you get here you have connected to the WiFi
  Serial.println("...Connected to wifi");

  //   // read updated parameters

  strcpy(username, user_name.getValue());
  strcpy(password, pass_word.getValue());
  Serial.println("The values in the file are: ");
  Serial.println("\tusername : " + String(username));
  Serial.println("\tpassword : " + String(password));

  //   // save the custom parameters to FS
  if (shouldSaveConfig)
  {
    Serial.println("Saving config...");

    json["username"] = String(username);
    json["password"] = String(password);
s_username = username;
            s_password = password;
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
      Serial.println("failed to open config file for writing");
    }

    serializeJson(json, Serial);
    serializeJson(json, configFile);
    configFile.close();
    if (!SPIFFS.exists("/config.json"))
    {
      Serial.println("SPIFFS DOES NOT EXIST!");
    }
    else
    {
      Serial.println("Config file found");
    }
    json.clear();
  }
  //     // end save
  
Serial.println("local ip");
Serial.println(WiFi.localIP());

}
