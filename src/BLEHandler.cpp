#include <string>
#include "BLEHandler.h"
#include "DataParser.h"
#include "commands.h"

extern BLEClient *pClient;
extern BLEScan *pBLEScan;
extern BLEAdvertisedDevice *myDevice;
extern BLERemoteCharacteristic *pNotifyCharacteristic;
extern BLERemoteCharacteristic *pWriteCharacteristic;
extern bool isRemoteMode;

void connectToDevice()
{
   if (myDevice != nullptr)
   {
      Serial.println("Csatlakozás a BLE eszközhöz...");
      pClient = BLEDevice::createClient();
      pClient->setClientCallbacks(new MyClientCallback());

      if (pClient->connect(myDevice))
      {
         Serial.println("Csatlakozás sikerült!");

         // Szolgáltatás betöltése
         BLERemoteService *pRemoteService = pClient->getService(BLEUUID(BLE_COMM_SERVICE));
         if (pRemoteService == nullptr)
         {
            Serial.println("Nem található a szolgáltatás.");
            return;
         }

         // Karakterisztikák betöltése
         pNotifyCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BLE_COMM_CHARACTERISTIC_NOTIFY));
         if (pNotifyCharacteristic == nullptr)
         {
            Serial.println("Nem található a notifikáció karakterisztika.");
            return;
         }

         pWriteCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BLE_COMM_CHARACTERISTIC_WRITE));
         if (pWriteCharacteristic == nullptr)
         {
            Serial.println("Nem található a write karakterisztika.");
            return;
         }

         // Engedélyezzük a notifikációkat
         if (pNotifyCharacteristic->canNotify())
         {
            pNotifyCharacteristic->registerForNotify(notifyCallback);
         }

         // Küldjünk egy parancsot a távoli mód indításához (CMD_START_REMOTE)
         pWriteCharacteristic->writeValue(CMD_START_REMOTE, 1);
         isRemoteMode = true;
         Serial.println("Remote Started");
      }
      else
      {
         Serial.println("Csatlakozás sikertelen.");
      }
   }
   else
   {
      Serial.println("Nem található eszköz a csatlakozáshoz.");
   }
}

void notifyCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
#if 0
   if (pData[0] != 0x64) {
      Serial.print("Kapott adat: ");
      for (int i = 0; i < length; i++)
      {
         Serial.printf("0x%02X ", pData[i]);
      }
      Serial.println();
   }
#endif
   if (length > 2)
      decodeBLEData(pData, length);
}

void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
   Serial.printf("Talált eszköz: %s\n", advertisedDevice.toString().c_str());

   // Eszköz neve egyeztetés
   if (advertisedDevice.getName() == tidDevice)
   {
      Serial.println("TD-H3 Detected");
      myDevice = new BLEAdvertisedDevice(advertisedDevice); // Tároljuk az eszközt
      pBLEScan->stop();                                     // Megállítjuk a keresést, megtaláltuk az eszközt
      Serial.println("Scan Stopped");
   }
}

void MyClientCallback::onConnect(BLEClient *pclient)
{
   Serial.println("Sikeresen csatlakozott!");
}

void MyClientCallback::onDisconnect(BLEClient *pclient)
{
   Serial.println("Eszköz lecsatlakozott.");
}

// Check Characteristic
bool ensureWriteCharacteristic()
{
   if (pWriteCharacteristic == nullptr)
   {
      BLERemoteService *pRemoteService = pClient->getService(BLEUUID(BLE_COMM_SERVICE));
      if (pRemoteService == nullptr)
      {
         Serial.println("Nem található a szolgáltatás.");
         return false;
      }

      pWriteCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BLE_COMM_CHARACTERISTIC_WRITE));
      if (pWriteCharacteristic == nullptr)
      {
         Serial.println("Nem található a karakterisztika.");
         return false;
      }
   }
   return true;
}
