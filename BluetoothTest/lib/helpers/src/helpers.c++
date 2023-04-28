#include <Preferences.h>
#include <Arduino.h>


Preferences preferences;

// Create an instance of the Preferences library
void printAvailableMemory(){
   size_t freeHeap = esp_get_free_heap_size();
  
  // // Print the free heap memory to the serial monitor
   Serial.print("Free heap memory: ");
   Serial.print(freeHeap);
   Serial.println(" bytes");
}

boolean storeFlashValue(String key, String value){
  Serial.println("Attemping to store flash key []" + key + "] with value [" + value + "]...");
  if (!preferences.begin("storage", false)) {
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

//Returns true if flash value was found, value is stored in outResult.
//Returns false otherwise.
boolean retrieveFlashValue(String key, String& outResult){
  Serial.println("Attemping to retrieve flash key []" + key + "]...");

   if (!preferences.begin("storage", false)) {
  Serial.println("...Attemping to retrieve flash key []" + key + "] [UNABLE TO OPEN PREFERENCES]");

    // Failed to open the partition, return an empty string
    return false;
  }
  preferences.begin("storage", false);
  String defaultVal = "NOTFOUND";
  outResult = preferences.getString(key.c_str(), "NOTFOUND");
  preferences.end();

  if(outResult == defaultVal){
  Serial.println("...Attemping to retrieve flash key []" + key + "] [NOT FOUND]");

    return false;
  }

  Serial.println("...Attemping to retrieve flash key []" + key + "] [SUCCESS, value is [" + outResult +"]");

  return true;

}