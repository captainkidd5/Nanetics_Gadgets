#include <Preferences.h>
#include <Arduino.h>
#include <SPIFFS.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <map>

Preferences preferences;
// Define the global StaticJsonDocument
StaticJsonDocument<1024> s_jsonDoc;

void setupJsonDoc(){
  s_jsonDoc.clear();
}
// Create an instance of the Preferences library
void printAvailableMemory()
{
  size_t freeHeap = esp_get_free_heap_size();

  // // Print the free heap memory to the serial monitor
  Serial.print("Free heap memory: ");
  Serial.print(freeHeap);
  Serial.println(" bytes");
}


bool retrieveSPIIFSValue(std::map<String, String>* dict)
{
  bool returnVal = false;
  if (SPIFFS.begin())
  {
    if (SPIFFS.exists("/config.json"))
    {
      //Serial.println("...Mounting FS [DONE]");

      //  file exists, reading and loading
      //Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile)
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

        StaticJsonDocument<1024> json;
        auto deserializeError = deserializeJson(json, buf.get());
       // serializeJson(json, Serial);
        if (!deserializeError)
        {
          //Serial.println("\nparsed json");
          for (auto& element : *dict) {
            String key = element.first;
            if (json.containsKey(key)) {
              element.second = json[key].as<String>();
              if(element.second != "")
                returnVal = true;
            }
          }
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
  return returnVal;
}

bool storeSPIFFSValue(std::map<String, String>* dict)
{
  bool returnVal = false;
  if (SPIFFS.begin())
  {
    Serial.println("...Mounting FS [DONE]");

    StaticJsonDocument<1024> json;
    for (auto& element : *dict) {
      String key = element.first;
      String value = element.second;
      json[key] = value;
    }

    File configFile = SPIFFS.open("/config.json", "w");
    if (configFile)
    {
      serializeJson(json, configFile);
      configFile.close();
      returnVal = true;
    }
    else
    {
      Serial.println("failed to open config file for writing");
    }
  }
  else
  {
    Serial.println("...Failed to mount FS");
  }
  return returnVal;
}
