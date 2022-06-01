#include "Operations.h"

void rClock_Tick(){
    if(millis() - myTimers.lastReadTime >= 10){ //process every 10ms
        myTimers.lastReadTime = millis();
        myTimers.FiftyMillis_Count++;

        NOW_Recieve.curTime = NOW_Recieve.curTime + 0.01; //update the time

        imuHandler();

        if(myTimers.FiftyMillis_Count >= 5){ //every 50ms
            myTimers.FiftyMillis_Count = 0;
            //Check_wifi();
            imuHandler();
            BroadCastNOW();
        }
    }
}

void Check_wifi(){
    if(WiFi.status() != WL_CONNECTED){
        Serial.println("Not connected to WiFi - Restarting");
        ESP.restart();
    }
}

void BroadCastNOW(){

    digitalWrite(def_LEDPIN, HIGH);

    MyEspNowData.messageID = 3; //imu message being sent

    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};//broadcast address
    //uint8_t broadcastAddress[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x02};//GPS address

    esp_now_peer_info_t peerInfo = {};

    memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
    if (!esp_now_is_peer_exist(broadcastAddress))
    {
        esp_now_add_peer(&peerInfo);
    }


    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&MyEspNowData, sizeof(MyEspNowData));

     if (result == ESP_OK) {
        
    }
    else {
        Serial.println("Error sending the data");
        myConfig.errorCount++;
        if(myConfig.errorCount > 20){
            ESP.restart();
        }
    }

    digitalWrite(def_LEDPIN, LOW);

}



void imuHandler()
{
    int16_t temp = 0;
    float roll;
    int16_t heading = 0;

    //the heading x10
    Wire.beginTransmission(CMPS14_ADDRESS);
    Wire.write(0x1C);
    Wire.endTransmission();

    Wire.requestFrom(CMPS14_ADDRESS, 3);
    while (Wire.available() < 3);

    roll = int16_t(Wire.read() << 8 | Wire.read());

    // the heading x10
    Wire.beginTransmission(CMPS14_ADDRESS);
    Wire.write(0x02);
    Wire.endTransmission();

    Wire.requestFrom(CMPS14_ADDRESS, 3);
    while (Wire.available() < 3);

    temp = Wire.read() << 8 | Wire.read();
    heading = temp;
    itoa(temp, MyEspNowData.IMU_Heading, 10);

    // 3rd byte pitch
    int8_t pitch = Wire.read();
    itoa(pitch, MyEspNowData.IMU_Pitch, 10);

    // the roll x10
    temp = (int16_t)roll;
    itoa(temp, MyEspNowData.IMU_Roll, 10);

    CalYaw(heading);
    // YawRate - 0 for now
    itoa(myConfig.curYaw, MyEspNowData.IMU_Yaw, 10);

    //update time stamp
    MyEspNowData.messageTimeStamp = myGlobalVariables.currentTime;
}


void CalYaw(int myHeading){
    for(int i = def_yawCount; i >= 1; i--){
        myConfig.yawReadings[i] = myConfig.yawReadings[i-1];
    }
    myConfig.yawReadings[0] = myHeading;

    myConfig.curYaw = (myConfig.yawReadings[0] - myConfig.yawReadings[def_yawCount - 1]);
    if(myConfig.curYaw > 3000){
        myConfig.curYaw = myConfig.curYaw - 3600;
    }

    if(myConfig.curYaw < -3000){
        myConfig.curYaw = myConfig.curYaw + 3600;
    }

    //Serial.print("yaw:"); Serial.println(myConfig.curYaw);
}

/*void Calc_Pitch(){

    float RealPitch = 0.0;

    if(NOW_IMU.data[4] >= 0 && NOW_IMU.data[4] <= 7){
        //rolling right
        RealPitch = ((float(NOW_IMU.data[4]) * 255.00) + float(NOW_IMU.data[5])) / 10.00;
        //RealRoll = RealRoll * float(-1); uncomment to make negative
        //Serial.print(" | roll:"); //Serial.println(RealRoll);
    } else {
        RealPitch = (((255.00 - float(NOW_IMU.data[4])) * 255.00) + float(255.00 - NOW_IMU.data[5])) / 10.00;
        RealPitch = RealPitch * float(-1);
    }

    snprintf(NOW_IMU.IMU_Pitch, 8, "%.02f", RealPitch);

    //Serial.print(" | Pitch:"); Serial.println(NOW_IMU.IMU_Pitch);
}*/


/*void Calc_Heading(){

    float RealHeading = 0.0;

    RealHeading = ((NOW_IMU.data[0] * 255.00) + (NOW_IMU.data[1])) / 10;

    snprintf(NOW_IMU.IMU_Heading, 7, "%.02f", RealHeading);


    //Serial.print("0:"); Serial.print(NOW_IMU.data[0]);
    //Serial.print(" | 1:"); Serial.print(NOW_IMU.data[1]);
    //Serial.print(" | Heading:"); Serial.println(RealHeading);

}*/

/*void Calc_Roll(){
    float RealRoll = 0.0;
    //strlcpy("", NOW_IMU.IMU_Roll, 8);

    //Serial.print("2:"); Serial.print(NOW_IMU.data[2]);
    //Serial.print(" | 3:"); Serial.print(NOW_IMU.data[3]);

    if(NOW_IMU.data[2] >= 0 && NOW_IMU.data[2] <= 7){
        //rolling right
        RealRoll = ((float(NOW_IMU.data[2]) * 255.00) + float(NOW_IMU.data[3])) / 10.00;
        //RealRoll = RealRoll * float(-1); uncomment to make negative
        //Serial.print(" | roll:"); //Serial.println(RealRoll);
    } else {
        RealRoll = (((255.00 - float(NOW_IMU.data[2])) * 255.00) + float(255.00 - NOW_IMU.data[3])) / 10.00;
        RealRoll = RealRoll * float(-1);
        //Serial.print(" | roll:"); //Serial.println(RealRoll);
    }

    snprintf(NOW_IMU.IMU_Roll, 8, "%.02f", RealRoll);
    //Serial.println(NOW_IMU.IMU_Roll);

}*/

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&MyEspNowData, incomingData, sizeof(MyEspNowData));

    digitalWrite(def_LEDPIN, HIGH);

    switch(MyEspNowData.messageID){
        case 1:
            HandleTimeRecv();
        break;
    }

    digitalWrite(def_LEDPIN, LOW);
}


void HandleTimeRecv(){

    Serial.print("Time: "); Serial.println(myGlobalVariables.currentTime);
    myGlobalVariables.currentTime = MyEspNowData.messageTimeStamp;

}