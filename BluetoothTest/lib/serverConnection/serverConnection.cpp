#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>

// Your Domain name with URL path or IP address with path
//https://randomnerdtutorials.com/esp32-https-requests/
const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n" \
"MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n" \
"2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n" \
"1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n" \
"q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n" \
"tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n" \
"vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n" \
"BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n" \
"5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n" \
"1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n" \
"NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n" \
"Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n" \
"8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n" \
"pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n" \
"MrY=\n" \
"-----END CERTIFICATE-----\n";
const char* servName;

void SetupServerCredentials(const char* sName){
  servName = sName;
}
void Get(WiFiClientSecure& client) {
  //need to use 443 for https
  int conn = client.connect(servName, 443);

   if (conn == 1) {
      Serial.println(); Serial.print("Sending Parameters...");
      //Request
      client.println("GET https://naneticsapi.azurewebsites.net/Nanites/GetData HTTP/1.0");
      //Headers
      client.print("Host: "); client.println(servName);
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

void Post(WiFiClientSecure& client) {
  //need to use 443 for https
  int conn = client.connect(servName, 443);

  if (conn == 1) {
    Serial.println(); Serial.print("Sending JSON body...");
    // Prepare the JSON payload
    const size_t capacity = JSON_OBJECT_SIZE(1) + 20;
    DynamicJsonDocument doc(capacity);
    doc["Message"] = "Hello from Arduino!";
    String payload;
    serializeJson(doc, payload);
    //Request
    client.println("POST /ping HTTP/1.1");
    client.println("Host: naneticsapi.azurewebsites.net");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.println(payload);
     while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    client.stop();

   } else {
      client.stop();
      Serial.println("Connection Failed");
   }
}


  void apiLoop(WiFiClientSecure& client) {

if (WiFi.status() == WL_CONNECTED) {
    delay(10000);
   
     client.setCACert(root_ca);
   //Post();
   Get(client);


}
else {
    Serial.println("WiFi Disconnected");
  }

}