#include "pinConfig.h"
#include "commands.h"
#include "ButtonHandler.h"
#include "BLEHandler.h" // A BLE kapcsolat a PTT vezérléshez

extern OneButton button1;
#ifdef BUTTON2
extern OneButton button2;
#endif

extern BLERemoteCharacteristic *pWriteCharacteristic;

extern bool isPTTOn;
extern unsigned long totStartTime;
extern bool isRemoteMode;

void setupButtons()
{
    button1.attachClick(button1Click);
    button1.attachDoubleClick(button1DoubleClick);
    button1.attachMultiClick(button1MultiClick);
    button1.attachLongPressStart(button1LongPress);
#ifdef BUTTON2
    button2.attachClick(button2Click);
    button2.attachDoubleClick(button2DoubleClick);
    button2.attachMultiClick(button2MultiClick);
    button2.attachLongPressStart(button2LongPress);
#endif
}

void button1Click()
{
    if (!ensureWriteCharacteristic())
    {
        return;
    }
    Serial.println("PTT Felengedve");
    pWriteCharacteristic->writeValue(CMD_BTN_RELEASE, 1); // Release PTT button
    isPTTOn = false;
}

void button1DoubleClick()
{
    if (!ensureWriteCharacteristic())
    {
        return;
    }
    Serial.println("PTT Lenyomva");
    pWriteCharacteristic->writeValue(CMD_BTN_PTT, 1);
    isPTTOn = true;
    totStartTime = millis(); // TOT indítása
}

void button1MultiClick()
{
    if (!ensureWriteCharacteristic())
    {
        return;
    }
    if (isRemoteMode)
    {
        pWriteCharacteristic->writeValue(CMD_END_REMOTE, 1);
        isRemoteMode = false; // Most CMD módban van
    }
    else
    {
        pWriteCharacteristic->writeValue(CMD_START_REMOTE, 1);
        isRemoteMode = true; // Most Remote módban van
    }
}

void button1LongPress()
{
    if (!ensureWriteCharacteristic())
    {
        return;
    }
    pWriteCharacteristic->writeValue(CMD_BTN_LAMP, 1);
    delay(SHORTPRESS_DELAY);
    pWriteCharacteristic->writeValue(CMD_BTN_RELEASE, 1);
}

void button2Click()
{
    if (!ensureWriteCharacteristic())
    {
        return;
    }
    // controlPtt(pWriteCharacteristic, false);
}

void button2DoubleClick()
{
    if (!ensureWriteCharacteristic())
    {
        return;
    }
    // controlPtt(pWriteCharacteristic, true);
}

void button2MultiClick()
{
    if (!ensureWriteCharacteristic())
    {
        return;
    }
    // changeMode(pWriteCharacteristic, true);
}

void button2LongPress()
{
    if (!ensureWriteCharacteristic())
    {
        return;
    }
    // radioButtonPress(pWriteCharacteristic, true);
}