#ifndef _PIN_CONFIG_H_
#define _PIN_CONFIG_H_
#include <Arduino.h>

//ESP32 devkit e.g.: NodeMcu
#ifdef SIMPLE
#define LED_PIN         2
#define BUTTON1         0
#define BUTTON2         33


//Lilygo T-QT Pro 8M, NO PSRAM
#elif defined(T_QT_PRO)
#define LED_PIN         2
#define BUTTON1         0
#define BUTTON2         47
//GC9107 LCD 128x128
#define LCD_BL          10
#define BL_PWM          50      // Backlight PWM enable & Initial backlight duty cycle
#define LCD_DC          6
#define LCD_CS          5
#define LCD_CLK         3
#define LCD_MOSI        2
#define LCD_RST         1
#define LCD_SPI         1

#elif defined (WAVESHARE_ROUND)
#define LED_PIN         2
#define BUTTON1         0
//GC9107 LCD 128x128
#define LCD_BL          40
#define BL_PWM          50      // Backlight PWM enable & Initial backlight duty cycle
#define LCD_DC          8
#define LCD_CS          9
#define LCD_CLK         10
#define LCD_MOSI        11
#define LCD_RST         12
#define LCD_SPI         1
// Extra
#define BAT_ADC         1
#endif

#endif