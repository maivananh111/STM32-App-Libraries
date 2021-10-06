/*
 * ili9341.h
 *
 * Created: 02/08/2021 9:19:18 CH
 *  Author: A315-56
 */ 
#ifndef ILI9341_H_
#define ILI9341_H_

#include <stdlib.h>
#include "pgmspace.h"

#define  swap(a,b)   {int16_t t = a; a = b; b = t;}
#define  DATA_PORT    GPIOB   //D7,6,5,4,3,2 to PORTD
#define D0_N          7
#define D1_N          6
#define D2_N          5
#define D3_N          4
#define D4_N          3
#define D5_N          2
#define D6_N          1
#define D7_N          0
#define D0_PIN        GPIO_PIN_7
#define D1_PIN        GPIO_PIN_6
#define D2_PIN        GPIO_PIN_5
#define D3_PIN        GPIO_PIN_4
#define D4_PIN        GPIO_PIN_3
#define D5_PIN        GPIO_PIN_2
#define D6_PIN        GPIO_PIN_1
#define D7_PIN        GPIO_PIN_0

#define COMMAND_PORT  GPIOA
#define LCD_CS        0  //Chip Select
#define LCD_CD        1  //Command/Data
#define LCD_WR        2  //LCD Write
#define LCD_RD        3  //LCD Read
#define LCD_RESET     4  //LCD Reset

class TFT {
    public:
	    TFT(uint16_t lcd_width, uint16_t lcd_height);
	    uint16_t randColor(void);
	    uint32_t readID(void);
	    uint16_t Width(void);
	    uint16_t Height(void);
	    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
	    uint16_t readPixel(uint16_t x, uint16_t y);
	    void readGram(uint16_t x, uint16_t y, uint16_t *block, uint16_t w, uint16_t h);
		void Init(void);
		void setRotation(uint8_t r);
		void setAddrWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
		void fillScreen(uint16_t color);
		void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
		void fillRectangle(uint16_t color,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void drawPixel(uint16_t x, uint16_t y, uint16_t color);
		void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
		void drawRect(uint16_t color,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void drawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
		void drawRGBBitmap(uint16_t x, uint16_t y, const uint16_t* bitmap, uint16_t w, uint16_t h);
		void testLines(void);

    private:
		uint16_t X_SIZE = 0, width = 0;
		uint16_t Y_SIZE = 0, height = 0;
		void setWriteDir (void);
		void setReadDir (void);
		void Command_Out(void);
		void port_Init(void);
		void SendCommand(uint8_t cmd);
		void SendData(uint8_t dt);
		void Write8(uint8_t dt);
		void WriteCMD(uint8_t CMD);
		uint32_t ReadReg(uint8_t r);
		void reset(void);
		void Flood(uint16_t color, uint32_t len);
		void WriteRegister32(uint8_t r, uint32_t d);

};
#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFDA0      /* 255, 180,   0 */
#define GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define PINK        0xFC9F

const uint16_t COLOR[19] = {BLACK, NAVY, DARKGREEN, DARKCYAN, MAROON, PURPLE, OLIVE, LIGHTGREY, DARKGREY, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE, ORANGE, GREENYELLOW, PINK};


#endif /* ILI9341_H_ */
