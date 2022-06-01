#include "myTimers.h"

void Clock_Tick(){

    myGlobalVariables.newMillis = millis();

    unsigned long timePassed = myGlobalVariables.newMillis - myGlobalVariables.lastMillis;

    myGlobalVariables.lastMillis = myGlobalVariables.newMillis;

    myGlobalVariables.currentTime = myGlobalVariables.currentTime + timePassed;

    if((myGlobalVariables.currentTime - myGlobalVariables.previousTime) >= 10){ //1/100th of a second
        
        myGlobalVariables.previousTime = myGlobalVariables.currentTime;
        hundredthSecond();

    }

}

void hundredthSecond(){

    imuHandler();
    BroadCastNOW();

    //Serial.println(myGlobalVariables.currentTime);
    
    myGlobalVariables.hundredthSecond++;

    if(myGlobalVariables.hundredthSecond >= 10){
        
        myGlobalVariables.hundredthSecond = 0;
        tenthSecond();

    }

}

void tenthSecond(){

    myGlobalVariables.tenthSecond++;

    if(myGlobalVariables.tenthSecond >= 10){
        
        myGlobalVariables.tenthSecond = 0;
        oneSecond();

    }

}


void oneSecond(){
    //Serial.println(myGlobalVariables.currentTime);
}