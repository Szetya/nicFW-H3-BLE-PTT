#ifndef BLEHANDLER_H
#define BLEHANDLER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

const std::string tidDevice = "TD-H3";

void connectToDevice();
void notifyCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify);

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
public:
    void onResult(BLEAdvertisedDevice advertisedDevice) override;
};

class MyClientCallback : public BLEClientCallbacks
{
public:
    void onConnect(BLEClient *pClient) override;    // Deklarálás
    void onDisconnect(BLEClient *pClient) override; // Deklarálás
};

bool ensureWriteCharacteristic();

#endif // BLEHANDLER_H
