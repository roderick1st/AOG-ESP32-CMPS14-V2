#ifndef INIT_H
#define INIT_H

#include <Arduino.h>
#include <Wire.h>
#include "Definitions.h"
#include "Operations.h"
#include <esp_wifi.h>
#include <esp_now.h>


void initMacAddress();
void initWiFi();
void initChannelChange();
void initCMPS14();
void initESPNOW();

#endif