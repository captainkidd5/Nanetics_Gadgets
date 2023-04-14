#include <Preferences.h>

void setSettings(char * key, char * value){
    Preferences prefs;
  prefs.begin("settings", false);
  prefs.putString(key, value);
  prefs.end();
}