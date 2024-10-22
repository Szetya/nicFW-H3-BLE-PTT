#ifndef _DISPLAYHANDLER_H_
#define _DISPLAYHANDLER_H_
#include <Arduino.h>

#define BGCOLOR 0x0000

void setup_display();
void drawText(uint8_t x, uint8_t y, uint8_t size, uint16_t color, String text);

#endif