#include <Preferences.h>
#include <Arduino.h>
#include <SPIFFS.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <map>

Preferences preferences;

// Create an instance of the Preferences library
void printAvailableMemory()
{
  size_t freeHeap = esp_get_free_heap_size();

  // // Print the free heap memory to the serial monitor
  Serial.print("Free heap memory: ");
  Serial.print(freeHeap);
  Serial.println(" bytes");
}

bool storeFlashValue(String key, String value)
{
  Serial.println("Attemping to store flash key []" + key + "] with value [" + value + "]...");
  if (!preferences.begin("storage", false))
  {
    // Failed to open the partition, return an empty string
    Serial.println("...Attemping to store flash key []" + key + "] with value [" + value + "] [UNABLE TO OPEN PREFERENCES]");

    return false;
  }

  preferences.putString(key.c_str(), value);
  Serial.println("...Attemping to store flash key []" + key + "] with value [" + value + "] [SUCCESS]");

  // Close the preferences partition
  preferences.end();
  return true;
}

// Returns true if flash value was found, value is stored in outResult.
// Returns false otherwise.
bool retrieveFlashValue(String key, String &outResult)
{
  Serial.println("Attemping to retrieve flash key []" + key + "]...");

  if (!preferences.begin("storage", false))
  {
    Serial.println("...Attemping to retrieve flash key []" + key + "] [UNABLE TO OPEN PREFERENCES]");

    // Failed to open the partition, return an empty string
    return false;
  }
  preferences.begin("storage", false);
  String defaultVal = "NOTFOUND";
  outResult = preferences.getString(key.c_str(), "NOTFOUND");
  preferences.end();

  if (outResult == defaultVal)
  {
    Serial.println("...Attemping to retrieve flash key []" + key + "] [NOT FOUND]");

    return false;
  }

  Serial.println("...Attemping to retrieve flash key []" + key + "] [SUCCESS, value is [" + outResult + "]");

  return true;
}
bool retrieveSPIIFSValue(std::map<String, String>* dict)
{
  bool returnVal = false;
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

    DynamicJsonDocument json(1024);
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
