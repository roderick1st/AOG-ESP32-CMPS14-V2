#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <Arduino.h>
#include "Definitions.h"
#include "GlobalStructs.h"
#include <AsyncUDP.h>
#include <Wire.h>
#include <WiFi.h>
#include "init.h"
#include <esp_now.h>

extern AsyncUDP udp;

void rClock_Tick();
void Check_wifi();
void Get_CMPS_Reading();
void Checksum_CMPS_Reading();
void Send_GPS_Reading();
void Send_CMPS_Reading();
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void Calc_Roll();
void Calc_Heading();
void Calc_Pitch();
void imuHandler();
void BroadCastNOW();
void CalYaw(int myHeading);

void HandleTimeRecv();

#endif