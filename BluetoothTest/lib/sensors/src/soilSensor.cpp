#include <ArduinoJson.h>


int moisturePin = 34;
//between 30 and 80% is good
const int AirValue = 3000; 
const int WaterValue = 2400;  
int precision = 8;
String moistureString;

const int _moistCutOff = 35;
const int _dryCutOff = 5;


uint32_t getMoistureLevel(){
  
   uint32_t totalMoisture = 0;

// //average the results for more accurate reading
   for(int i =0; i < precision; i++){
     totalMoisture+=analogRead(moisturePin);
   delay(100);

  }

  uint32_t soilMoistureValue = (uint32_t)((float)totalMoisture/(float)precision);
  Serial.println("Moisture level is " + String(soilMoistureValue));
return soilMoistureValue;
  //  return soilMoistureValue;
  //  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

}

void senseLoop() {
  

//    int totalMoisture = 0;

// // //average the results for more accurate reading
//    for(int i =0; i < precision; i++){
//      totalMoisture+=analogRead(moisturePin);
//    delay(100);

//   }

//    soilMoistureValue = (int)((float)totalMoisture/(float)precision);
//    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);


//     if (soilmoisturepercent >= _moistCutOff) {
//       moistureString = "Moist";

//     } else if (soilmoisturepercent < _moistCutOff && soilmoisturepercent >= _dryCutOff) {
//       moistureString = "Getting Dry";

//     } else if (soilmoisturepercent < _dryCutOff) {
//      moistureString = "Dry";

//     } else {
//       moistureString = "Error!";
//     }
//     Serial.println(moistureString);
//   //  Serial.println("sleeping...");
  

   // ESP.deepSleep(15e6);
  
}

