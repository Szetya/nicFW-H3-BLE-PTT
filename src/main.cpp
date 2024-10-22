#include <Arduino.h>
#include <string>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "pinConfig.h"
#include "commands.h"
#include "ButtonHandler.h"
#include "BLEHandler.h"
#include "DataParser.h"
#ifndef SIMPLE
#include "displayHandler.h"
#endif

OneButton button1(BUTTON1, true, true);
#ifdef BUTTON2
OneButton button2(BUTTON2, true, true);
#endif

BLEClient *pClient;
BLEScan *pBLEScan;
BLEAdvertisedDevice *myDevice = nullptr;
BLERemoteService *pRemoteService = nullptr;
BLERemoteCharacteristic *pNotifyCharacteristic = nullptr;
BLERemoteCharacteristic *pWriteCharacteristic = nullptr;

unsigned long totStartTime = 0;
const unsigned long totMaxTime = 2 * 60 * 1000;
bool isPTTOn = false;
bool isRemoteMode = true;

void setup()
{
    Serial.begin(115200);
    setupButtons();
#ifndef SIMPLE
    setup_display();
#endif

    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
}

void loop()
{
    button1.tick();
#ifdef BUTTON2
    button2.tick();
#endif

    if (myDevice != nullptr && pClient == nullptr)
    {
        connectToDevice();
    }

    if (isPTTOn)
    {
        unsigned long currentTime = millis();
        if (currentTime - totStartTime >= totMaxTime)
        {
            button1Click();
            Serial.println("TOT lejárt: PTT felengedve");
        }
    }
}
