#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WifiConnection.h>
#include <ServerConnection.h>
#include <WiFiClientSecure.h>
#include <azureIotConnection.h>


WiFiClientSecure wifiClient;
const char* serverName = "naneticsapi.azurewebsites.net";


void setupWifiConnection(String wifiName, String wifiPass) {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(4500);
  WiFi.mode(WIFI_STA);
  // Configures static IP address
 
  WiFi.begin(wifiName.c_str(), wifiPass.c_str());
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.flush();
}

void setup() {
  setupWifi();
  if(hasWifiCredentials){
    String ssid = WiFi.SSID();
String password = WiFi.psk();
setupWifiConnection(ssid, password);
SetupServerCredentials(serverName);
azureIotConnection::setupIotConnection();
  }
}

void loop() {
    if(hasWifiCredentials){
        apiLoop(wifiClient);
        MyClass::IotLoop();
    }
    // put your main code here, to run repeatedly:
}
