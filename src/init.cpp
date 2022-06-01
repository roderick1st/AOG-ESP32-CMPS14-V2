#include "init.h"

void init_Global_Vars(){


    myGlobalVariables.newMillis = 0;
    myGlobalVariables.lastMillis = 0;
    myGlobalVariables.currentTime = 0;
    myGlobalVariables.previousTime = 0;
    myGlobalVariables.hundredthSecond = 0;
    myGlobalVariables.tenthSecond = 0;
    myGlobalVariables.oneSecond = 0;

}

void initMacAddress(){

    WiFi.mode(WIFI_AP_STA);

    uint8_t myMac[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x03};

    esp_wifi_set_mac(WIFI_IF_STA, &myMac[0]);
    Serial.print("New Mac address : "); Serial.println(WiFi.macAddress());

}

/*void initWiFi(){

    unsigned long startTime = millis();
    unsigned long currentTime = 0;

    pinMode(def_LEDPIN, OUTPUT);

    //initESPNOW();

    WiFi.begin("AOGROUTER");

    while (WiFi.status() != WL_CONNECTED){

        currentTime = millis();
        if(currentTime - startTime > 10000){
            break; //kill the connection loop
        }

    }

}*/

void initESPNOW(){

    //uint8_t gpsMac[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x02};
    //uint8_t broadcastMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        myConfig.systemGood = false;
    } else {
        //esp_now_register_recv_cb(OnDataRecv);
        Serial.println("ESP-Now initilised");
        myConfig.systemGood = true;
    }

    if(myConfig.systemGood){
        esp_now_register_send_cb(OnDataSent);
        esp_now_register_recv_cb(OnDataRecv);
    }

}


void initCMPS14(){

    pinMode(def_CMPS_POWER_Pin, OUTPUT);
    digitalWrite(def_CMPS_POWER_Pin, HIGH);
    delay(1000);

    Wire.begin(def_SDA_PIN, def_SCL_PIN);

    uint8_t error = 4;

    myTimers.cmpsConnectTime = millis();

    while(error != 0){
        if(millis() - myTimers.cmpsConnectTime >= 1000){
            break;
        }
        Wire.beginTransmission(CMPS14_ADDRESS);
        error = Wire.endTransmission();
    }
    

    if (error == 0)
    {
        //Serial.println("Error = 0");
        Serial.print("CMPS14 ADDRESS: 0x");
        Serial.println(CMPS14_ADDRESS, HEX);
        Serial.println("CMPS14 Ok.");
        myConfig.systemGood = true;
    }
    else
    {
        Serial.println("Error = 4");
        Serial.println("CMPS not Connected or Found");
        myConfig.systemGood = false;
    }
}
