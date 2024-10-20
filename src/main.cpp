#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "commands.h"
#include "OneButton.h"

OneButton button1(0, true, true);

BLEClient *pClient;
BLEScan *pBLEScan;
BLEAdvertisedDevice *myDevice = nullptr;                  // Globális változó
BLERemoteService *pRemoteService = nullptr;               // 0000FF00
BLERemoteCharacteristic *pNotifyCharacteristic = nullptr; // 0000FF01
BLERemoteCharacteristic *pWriteCharacteristic = nullptr;  // 0000FF02

unsigned long totStartTime = 0;                 // PTT aktiválásának kezdete
const unsigned long totMaxTime = 2 * 60 * 1000; // 2 perc (ms)
bool isPTTOn = false;                           // PTT állapot figyelése
const std::string tidDevice = "TD-H3";          // Keresett eszköz neve

void connectToDevice();
void decodeBLEData(uint8_t *data, size_t length);

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
   void onResult(BLEAdvertisedDevice advertisedDevice)
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
};

static void notifyCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
   Serial.print("Kapott adat: ");
   for (int i = 0; i < length; i++)
   {
      Serial.printf("0x%02X ", pData[i]);
   }
   Serial.println();
   if (length > 2)
      decodeBLEData(pData, length);
}

class MyClientCallback : public BLEClientCallbacks
{
   void onConnect(BLEClient *pclient)
   {
      Serial.println("Sikeresen csatlakozott!");
   }

   void onDisconnect(BLEClient *pclient)
   {
      Serial.println("Eszköz lecsatlakozott.");
   }
};

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

// PTT kontroll
void controlPtt(BLERemoteCharacteristic *pWriteCharacteristic, bool turnOn)
{
   if (turnOn)
   {
      Serial.println("PTT Lenyomva");
      pWriteCharacteristic->writeValue(CMD_BTN_PTT, 1);
      isPTTOn = true;
      totStartTime = millis(); // TOT indítása
   }
   else
   {
      Serial.println("PTT Felengedve");
      pWriteCharacteristic->writeValue(CMD_BTN_RELEASE, 1);
      isPTTOn = false;
   }
}

// Button control e.g. LAMP toggle
void radioButtonPress(BLERemoteCharacteristic *pWriteCharacteristic, bool press)
{
   if (press)
   {
      pWriteCharacteristic->writeValue(CMD_BTN_LAMP, 1);
      delay(SHORTPRESS_DELAY);
      pWriteCharacteristic->writeValue(CMD_BTN_RELEASE, 1);
   }
}

// Release PTT
void buttonClick()
{
   if (pWriteCharacteristic == nullptr)
   {
      BLERemoteService *pRemoteService = pClient->getService(BLEUUID(BLE_COMM_SERVICE));
      if (pRemoteService == nullptr)
      {
         Serial.println("Nem található a szolgáltatás.");
         return;
      }

      pWriteCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BLE_COMM_CHARACTERISTIC_WRITE));
      if (pWriteCharacteristic == nullptr)
      {
         Serial.println("Nem található a karakterisztika.");
         return;
      }
   }

   controlPtt(pWriteCharacteristic, false);
}

// Press PTT
void buttonDoubleClick()
{
   if (pWriteCharacteristic == nullptr)
   {
      BLERemoteService *pRemoteService = pClient->getService(BLEUUID(BLE_COMM_SERVICE));
      if (pRemoteService == nullptr)
      {
         Serial.println("Nem található a szolgáltatás.");
         return;
      }

      pWriteCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BLE_COMM_CHARACTERISTIC_WRITE));
      if (pWriteCharacteristic == nullptr)
      {
         Serial.println("Nem található a karakterisztika.");
         return;
      }
   }

   controlPtt(pWriteCharacteristic, true);
}

void buttonMultiClick() {
   // tripla klikk, próbáljunk meg leiratkozni a 0000FF01-ről > CMD_END_REMOTE
}

void buttonLongPress() {
   if (pWriteCharacteristic == nullptr)
   {
      BLERemoteService *pRemoteService = pClient->getService(BLEUUID(BLE_COMM_SERVICE));
      if (pRemoteService == nullptr)
      {
         Serial.println("Nem található a szolgáltatás.");
         return;
      }

      pWriteCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BLE_COMM_CHARACTERISTIC_WRITE));
      if (pWriteCharacteristic == nullptr)
      {
         Serial.println("Nem található a karakterisztika.");
         return;
      }
   }

   radioButtonPress(pWriteCharacteristic, true);
}

void decodeBLEData(uint8_t *data, size_t length)
{
   if (data[0] != 0x77)
   {
      Serial.println("Ismeretlen parancs!");
      return;
   }

   uint8_t x = data[1];                        // X koordináta
   uint8_t y = data[2];                        // Y koordináta
   uint8_t fontSize = data[3];                 // Betűméret
   uint16_t colour = (data[4] << 8) | data[5]; // Szín (2 byte)

   // Szöveg kiolvasása (nullával lezárt)
   String text = "";
   for (int i = 6; i < length && data[i] != 0x00; i++)
   {
      text += (char)data[i];
   }

   // Eredmények kiírása
   Serial.print("X: ");
   Serial.println(x);
   Serial.print("Y: ");
   Serial.println(y);
   Serial.print("Betűméret: ");
   Serial.println(fontSize);
   Serial.print("Szín 565: 0x");
   Serial.println(colour, HEX);
   Serial.print("Szöveg: ");
   Serial.println(text);
}

void setup()
{
   Serial.begin(115200);

   button1.attachClick(buttonClick);
   button1.attachDoubleClick(buttonDoubleClick);
   button1.attachMultiClick(buttonMultiClick);
   button1.attachLongPressStart(buttonLongPress);

   BLEDevice::init("");

   pBLEScan = BLEDevice::getScan();
   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
   pBLEScan->setActiveScan(true);
   pBLEScan->start(30);
}

void loop()
{
   // connectToDevice() meghívása ha eszközt talál
   if (myDevice != nullptr && pClient == nullptr)
   {
      connectToDevice(); // Kapcsolódunk az eszközhöz
   }
   button1.tick();

   // TOT ellenőrzés
   if (isPTTOn)
   {
      unsigned long currentTime = millis();
      // Ha eltelt a TOT idő (pl. 2 perc), felengedjük a PTT-t
      if (currentTime - totStartTime >= totMaxTime)
      {
         buttonClick(); // PTT felengedése
         Serial.println("TOT: PTT felengedve a max idő miatt.");
      }
   }
}