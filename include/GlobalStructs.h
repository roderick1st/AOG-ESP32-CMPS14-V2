#ifndef GLOBALSTRUCTS_H
#define GLOBALSTRUCTS_H

#include <Arduino.h>
#include "Definitions.h"


struct GlobalVariables { 
    unsigned long lastMillis;
    unsigned long newMillis;
    unsigned long currentTime; 
    unsigned long previousTime;
    unsigned long hundredthSecond;
    unsigned long tenthSecond;
    unsigned long oneSecond;
};
extern GlobalVariables myGlobalVariables;


struct Config{
    uint16_t port;
    uint16_t listenPort;
    uint16_t portDestination;
    IPAddress broadcastAddress;
    uint8_t helloFromIMU[7];
    uint8_t data[14];
    int16_t dataSize;
    bool systemGood;
    int SystemNoGoodCount;
    uint8_t macAddress[6];
    int errorCount;
    int yawReadings[def_yawCount];
    int curYaw;

};
extern Config myConfig;


struct Timers{
    unsigned long wifiConnectTime;
    unsigned long lastReadTime;
    unsigned long cmpsConnectTime;
    int FiftyMillis_Count;
};
extern Timers myTimers;

struct WiFiConfig{
    char ssid[9];
    char password;
};
extern WiFiConfig wifiConfig;

struct ESPNOWData{

    int messageID;
    unsigned long messageTimeStamp;

    char IMU_Roll[6];
    char IMU_Pitch[6];
    char IMU_Heading[6];
    char IMU_Yaw[6];

    char Panda[def_LengthOfPandaString];

    int ntripLen;

};

extern ESPNOWData MyEspNowData;

typedef struct ESPNow_Recieve{
    double curTime;
}ESPNow_Recieve;

extern ESPNow_Recieve NOW_Recieve;

#endif