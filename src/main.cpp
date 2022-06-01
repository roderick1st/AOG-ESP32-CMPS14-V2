#include <Arduino.h>
#include "init.h"
#include <esp_now.h>
#include "GlobalStructs.h"
#include "myTimers.h"


GlobalVariables myGlobalVariables;
Config myConfig;
Timers myTimers;
ESPNOWData MyEspNowData;
ESPNow_Recieve NOW_Recieve;



void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("AOG-IMU");

  initMacAddress();
  //initWiFi();

  initESPNOW();
  initCMPS14();
}

void loop() {
  Clock_Tick();
}


