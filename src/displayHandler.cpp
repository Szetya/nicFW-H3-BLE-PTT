#ifndef SIMPLE
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "pinConfig.h"
#include "displayHandler.h"

#ifdef USE_GC9107
Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_CLK, LCD_MOSI, -1, LCD_SPI); // HSPI
Arduino_GFX *gfx = new Arduino_GC9107(bus, LCD_RST, 0, true);
#endif

#ifdef USE_GC9A01A
Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_CLK, LCD_MOSI, -1, LCD_SPI); // HSPI
Arduino_GFX *gfx = new Arduino_GC9A01(bus, LCD_RST, 0, true);
#endif

void setup_display()
{
#if defined(LCD_BL) && !defined(BL_PWM)
    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);
#endif

#ifdef BL_PWM
    ledcSetup(0, 500, 8);     // PWM channel, feq, resolution
    ledcAttachPin(LCD_BL, 0); // PIN, PWM channel
    ledcWrite(0, BL_PWM);     // PWM channel, dutyCycle
#endif

    if (!gfx->begin())
    {
        Serial.println("gfx->begin() failed!");
    }
    gfx->fillScreen(BGCOLOR);
    gfx->setCursor(10, 10);
    gfx->setTextSize(1);
    gfx->setTextColor(BLUE);
    gfx->print("TD H3 remote");
    delay(1000);
    gfx->fillScreen(BGCOLOR);
#ifdef WATCH
    // https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/t-watch-s3/examples/display/TFT_Clock/TFT_Clock.ino
    float sx1 = 0, sy1 = 1;
    uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
    for (int i = 0; i < 360; i += 30)
    {
        sx1 = cos((i - 90) * 0.0174532925);
        sy1 = sin((i - 90) * 0.0174532925);
        x0 = sx1 * 114 + 120;
        yy0 = sy1 * 114 + 120;
        x1 = sx1 * 100 + 120;
        yy1 = sy1 * 100 + 120;

        gfx->drawLine(x0, yy0, x1, yy1, GREEN);
    }
    for (int i = 0; i < 360; i += 6)
    {
        sx1 = cos((i - 90) * 0.0174532925);
        sy1 = sin((i - 90) * 0.0174532925);
        x0 = sx1 * 102 + 120;
        yy0 = sy1 * 102 + 120;
        // Draw minute markers
        gfx->drawPixel(x0, yy0, YELLOW);

        // Draw main quadrant dots
        if (i == 0 || i == 180)
            gfx->fillCircle(x0, yy0, 2, WHITE);
        if (i == 90 || i == 270)
            gfx->fillCircle(x0, yy0, 2, WHITE);
    }
#endif
}

void drawText(uint8_t x, uint8_t y, uint8_t size, uint16_t color, String text)
{
    gfx->setCursor(x, y);
    gfx->setTextColor(color);
    gfx->setTextSize(size);
    int16_t xx, yy;
    uint16_t h, w;
    gfx->getTextBounds(text, int16_t(x), int16_t(y), &xx, &yy, &w, &h);
    // Serial.printf("x:%d, y:%d, xx:%d, yy:%d, w:%d, h:%d\r\n", x, y, xx, yy, w, h);
    gfx->fillRect(x, y, w, h, BGCOLOR);
    gfx->print(text);
}
#endif // SIMPLE