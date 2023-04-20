#include <Preferences.h>
void setSettings(const char * key,const char * value){
    Preferences prefs;
  prefs.begin("settings", false);
  prefs.putString(key, value);
  prefs.end();
}