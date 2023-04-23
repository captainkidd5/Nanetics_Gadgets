#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <server_configs.h>
#include <registerDevice.h>
//https://randomnerdtutorials.com/esp32-https-requests/




void Get(WiFiClientSecure& client) {
  //need to use 443 for https
  int conn = client.connect(serverUri,serverPort);

   if (conn == 1) {
      Serial.println(); Serial.print("Sending Parameters...");
      //Request
      String uri = serverUri;
      uri = "https://" + uri;
      client.println("GET " + uri + "/Devices/GetData HTTP/1.1");
      //Headers
      client.print("Host: "); client.println(serverUri);
      client.println("Connection: Close");
      client.println();

      while (client.connected()) {
      String line = client.readStringUntil('\n');
        Serial.println(line);

      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
      client.stop();

   } else {
      client.stop();
      Serial.println("Connection Failed");
   }
   delay(5000);
}



  void apiLoop(WiFiClientSecure& client) {

if (WiFi.status() == WL_CONNECTED) {
    delay(10000);
   
     client.setCACert(root_ca);
   //Get(client);
postRegisterDevice(client);

}
else {
    Serial.println("WiFi Disconnected");
  }

}