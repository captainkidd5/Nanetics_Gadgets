#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

bool hasWifiCredentials = false;

void setupWifi() {
     WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(9600);

Serial.write("Firing up");
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
     //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("NaneticsAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
       ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi
        Serial.println("connected to wifi!");
        hasWifiCredentials = true;
    }

}

