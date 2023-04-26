#include <Preferences.h>



void setSettings(const char * key,const char * value){
    Preferences prefs;
  prefs.begin("settings", false);
  prefs.putString(key, value);
  prefs.end();
}

void printAvailableMemory(){
  size_t freeHeap = esp_get_free_heap_size();
  
  // Print the free heap memory to the serial monitor
  Serial.print("Free heap memory: ");
  Serial.print(freeHeap);
  Serial.println(" bytes");
}