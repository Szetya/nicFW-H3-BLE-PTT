#include "DataParser.h"
#include "commands.h"
#ifndef SIMPLE
#include "displayHandler.h"
#endif

void decodeBLEData(uint8_t *data, size_t length)
{
   if (data[0] == CMD_DRAW_TEXT) // Draw text
   {
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
#if 0
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
#endif
#ifndef SIMPLE
#ifndef WAVESHARE_ROUND
      drawText(x, y, fontSize, colour, text);
#else
      drawText(x + 56, y + 56, fontSize, colour, text); // 240*240 round
#endif
#endif // SIMPLE
   }
   else if (data[0] == CMD_FILL_REGION) // Fill region
   {
      // Ellenőrizzük, hogy van-e elég adat a csomaghoz
      if (length >= 7)
      {
         uint8_t x = data[1];                        // X koordináta
         uint8_t y = data[2];                        // Y koordináta
         uint8_t width = data[3];                    // Szélesség
         uint8_t height = data[4];                   // Magasság
         uint16_t colour = (data[5] << 8) | data[6]; // Szín (2 byte)

#if 0
         // Eredmények kiírása
         Serial.print("Fill Region - X: ");
         Serial.print(x);
         Serial.print(", Y: ");
         Serial.print(y);
         Serial.print(", Width: ");
         Serial.print(width);
         Serial.print(", Height: ");
         Serial.print(height);
         Serial.print(", Szín 565: 0x");
         Serial.println(colour, HEX);
#endif
         // fillRegion(x, y, width, height, colour); // in displayHandler.cpp next time
      }
      else
      {
         Serial.println("Nincs elég adat a Fill Region feldolgozásához.");
      }
   }
   else if (data[0] == CMD_SMETER_LEVEL) // S-Meter Level
   {
      uint8_t mode = data[1];
      uint8_t level = data[2];
      // drawSmeterBar(level); // in displayHandler.cpp next time
      Serial.println("Mode: " + String(mode) + " Level: " + String(level));
   }
   else if (data[0] == CMD_NOISEMETER_LEVEL) // Noise Meter Level
   {
      uint8_t level = data[1];
      Serial.println("Level: " + String(level));
      // drawNoiseBar(level); // in displayHandler.cpp next time
   }
   else
   {
      // Sérült és ismeretlen adatcsomagok kiírása
      Serial.print("ismeretlen adat: ");
      for (int i = 0; i < length; i++)
      {
         Serial.printf("0x%02X ", data[i]);
      }
      Serial.println();
   }
}