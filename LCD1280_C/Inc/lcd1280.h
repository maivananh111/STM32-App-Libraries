#ifndef INC_LCD1280_H_
#define INC_LCD1280_H_

#include "stm32f10x.h"

#define P0 GPIO_PIN_0 
#define P1 GPIO_PIN_1 
#define P2 GPIO_PIN_2 
#define P3 GPIO_PIN_3 
#define P4 GPIO_PIN_4 
#define P5 GPIO_PIN_5 
#define P6 GPIO_PIN_6 
#define P7 GPIO_PIN_7 
#define P8 GPIO_PIN_8 
#define P9 GPIO_PIN_9 
#define P10 GPIO_PIN10 
#define P11 GPIO_PIN_11 
#define P12 GPIO_PIN_12 
#define P13 GPIO_PIN_13 
#define P14 GPIO_PIN_14 
#define P15 GPIO_PIN_15 

#define LCD_D  1
#define LCD_C  0

#define LCD_X  96
#define LCD_Y  68
#define LCD_STR 9

#define SetYAddr   0xB0
#define SetXAddr4  0x00
#define SetXAddr3  0x10

#define swap(a, b) {uint8_t t = a; a = b; b = t; }

char LCD_RAM[LCD_X*LCD_STR];

void LCD_SendByte(uint8_t mode, uint8_t c);
void lcd_Init(char port, char scl, char sda, char cs, char rst, char portLED, char led);
void lcd_SetBL(char mode);
void lcd_Clear();
void lcd_Update();
void lcd_DrawPixel (uint8_t x, uint8_t y, uint8_t color);
void lcd_DrawChar(uint8_t x, uint8_t y, uint8_t color, unsigned char c);
void lcd_print(uint8_t x, uint8_t y, uint8_t color, char *str);
void lcd_write(uint8_t x, uint8_t y, uint8_t color, float num, uint8_t type);
void lcd_DrawBitmap(uint8_t x, uint8_t y, const char *bitmap, uint8_t w, uint8_t h, uint8_t color);
void lcd_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void lcd_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color);
void lcd_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color);
void lcd_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void lcd_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void lcd_FillScreen(uint8_t color);
void lcd_DrawCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, uint8_t color);
void lcd_DrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color);
void lcd_FillCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, uint8_t delta, uint8_t color);
void lcd_FillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color);
void lcd_DrawCircle(uint8_t x0, uint8_t y0, int16_t r, uint8_t color);
void lcd_FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
void lcd_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void lcd_FillTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void lcd_DrawNum(uint8_t x, uint8_t y, uint8_t color, uint8_t c, uint8_t size);

#endif
