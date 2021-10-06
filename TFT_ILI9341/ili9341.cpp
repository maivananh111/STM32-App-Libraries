/*
 * ili9341.c
 *
 * Created: 02/08/2021 9:06:46 CH
 *  Author: A315-56
 */ 
#include "ili9341.h"
#include "stm32f4xx_hal.h"
#include <math.h>
//--------------------------------------------------------------
extern TIM_HandleTypeDef htim1;

#define write(d) { \
    DATA_PORT->BSRR = 0x00ff << 16; \
    DATA_PORT->BSRR = (((d) & (1<<0)) << 7) \
				    | (((d) & (1<<1)) << 5) \
				    | (((d) & (1<<2)) << 3) \
				    | (((d) & (1<<3)) << 1) \
				    | (((d) & (1<<4)) >> 1) \
				    | (((d) & (1<<5)) >> 3) \
				    | (((d) & (1<<6)) >> 5) \
				    | (((d) & (1<<7)) >> 7); \
    }
#define read() (   \
                    (((DATA_PORT->IDR & (1<<0)) << 7) \
				   | ((DATA_PORT->IDR & (1<<1)) << 5) \
				   | ((DATA_PORT->IDR & (1<<2)) << 3) \
				   | ((DATA_PORT->IDR & (1<<3)) << 1) \
				   | ((DATA_PORT->IDR & (1<<4)) >> 1) \
				   | ((DATA_PORT->IDR & (1<<5)) >> 3) \
				   | ((DATA_PORT->IDR & (1<<6)) >> 5) \
				   | ((DATA_PORT->IDR & (1<<7)) >> 7))\
)
static void _delay_us (uint16_t time);
void PIN_LOW (uint16_t GPIO_Pin);
void PIN_HIGH (uint16_t GPIO_Pin);

static void _delay_us (uint16_t time){
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim1))<time);
}
void PIN_LOW (uint16_t GPIO_Pin){
	COMMAND_PORT -> BSRR |= ((1<<GPIO_Pin)<<16);
}
void PIN_HIGH (uint16_t GPIO_Pin){
	COMMAND_PORT -> BSRR |= (1<<GPIO_Pin);
}
#define  RESET_IDLE   PIN_HIGH(LCD_RESET)
#define  CS_IDLE      PIN_HIGH(LCD_CS)
#define  WR_IDLE      PIN_HIGH(LCD_WR)
#define  RD_IDLE      PIN_HIGH(LCD_RD)

#define  RESET_ACTIVE PIN_LOW(LCD_RESET)
#define  CS_ACTIVE    PIN_LOW(LCD_CS)
#define  WR_ACTIVE    PIN_LOW(LCD_WR)
#define  RD_ACTIVE    PIN_LOW(LCD_RD)

#define  CD_COMMAND   PIN_LOW(LCD_CD)
#define  CD_DATA      PIN_HIGH(LCD_CD)
#define  WR_STROBE    { WR_ACTIVE; WR_IDLE; }
#define  RD_STROBE 	  { RD_IDLE; RD_ACTIVE; RD_ACTIVE; RD_ACTIVE; }
#define  READ_8(dst)   { RD_STROBE; dst = read(); RD_IDLE; RD_IDLE; }
#define  READ_16(dst)  { uint8_t hi; READ_8(hi); READ_8(dst); dst |= (hi << 8); }
//--------------------------------------------------------------
TFT::TFT(uint16_t lcd_width, uint16_t lcd_height){
	X_SIZE = lcd_width;
	Y_SIZE = lcd_height;
}
void TFT::setWriteDir (void){
	DATA_PORT -> MODER |=     (1<<(D0_N*2))
							 |(1<<(D1_N*2))
							 |(1<<(D2_N*2))
							 |(1<<(D3_N*2))
							 |(1<<(D4_N*2))
							 |(1<<(D5_N*2))
							 |(1<<(D6_N*2))
							 |(1<<(D7_N*2));
	DATA_PORT -> OTYPER &=~   (1<<D0_N)
				             |(1<<D1_N)
							 |(1<<D2_N)
							 |(1<<D3_N)
							 |(1<<D4_N)
							 |(1<<D5_N)
							 |(1<<D6_N)
							 |(1<<D7_N);
	DATA_PORT -> OSPEEDR |=   (1<<(D0_N*2))|(1<<(D0_N*2 + 1))
				             |(1<<(D1_N*2))|(1<<(D1_N*2 + 1))
							 |(1<<(D2_N*2))|(1<<(D2_N*2 + 1))
							 |(1<<(D3_N*2))|(1<<(D3_N*2 + 1))
							 |(1<<(D4_N*2))|(1<<(D4_N*2 + 1))
							 |(1<<(D5_N*2))|(1<<(D5_N*2 + 1))
							 |(1<<(D6_N*2))|(1<<(D6_N*2 + 1))
							 |(1<<(D7_N*2))|(1<<(D7_N*2 + 1));
	DATA_PORT -> PUPDR  &=~   (1<<(D0_N*2))|(1<<(D0_N*2 + 1))
							 |(1<<(D1_N*2))|(1<<(D1_N*2 + 1))
							 |(1<<(D2_N*2))|(1<<(D2_N*2 + 1))
							 |(1<<(D3_N*2))|(1<<(D3_N*2 + 1))
							 |(1<<(D4_N*2))|(1<<(D4_N*2 + 1))
							 |(1<<(D5_N*2))|(1<<(D5_N*2 + 1))
							 |(1<<(D6_N*2))|(1<<(D6_N*2 + 1))
							 |(1<<(D7_N*2))|(1<<(D7_N*2 + 1));
}
void TFT::setReadDir (void){
	 GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = D0_PIN|D1_PIN|D2_PIN|D3_PIN|D4_PIN|D5_PIN|D6_PIN|D7_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(DATA_PORT, &GPIO_InitStruct);
}
void TFT::Command_Out(void){
	COMMAND_PORT -> MODER |=   (1<<(LCD_CS*2))
							  |(1<<(LCD_CD*2))
							  |(1<<(LCD_WR*2))
							  |(1<<(LCD_RD*2))
							  |(1<<(LCD_RESET*2));
	COMMAND_PORT -> OTYPER &=~ (1<<LCD_CS)
							  |(1<<LCD_CD)
							  |(1<<LCD_WR)
							  |(1<<LCD_RD)
							  |(1<<LCD_RESET);
	COMMAND_PORT -> OSPEEDR |= (1<<(LCD_CS*2))|(1<<(LCD_CS*2 + 1))
							  |(1<<(LCD_CD*2))|(1<<(LCD_CD*2 + 1))
							  |(1<<(LCD_WR*2))|(1<<(LCD_WR*2 + 1))
							  |(1<<(LCD_RD*2))|(1<<(LCD_RD*2 + 1))
							  |(1<<(LCD_RESET*2))|(1<<(LCD_RESET*2 + 1));
	COMMAND_PORT -> PUPDR  &=~ (1<<(LCD_CS*2))|(1<<(LCD_CS*2 + 1))
							  |(1<<(LCD_CD*2))|(1<<(LCD_CD*2 + 1))
							  |(1<<(LCD_WR*2))|(1<<(LCD_WR*2 + 1))
							  |(1<<(LCD_RD*2))|(1<<(LCD_RD*2 + 1))
							  |(1<<(LCD_RESET*2))|(1<<(LCD_RESET*2 + 1));
}
uint16_t TFT::color565(uint8_t r, uint8_t g, uint8_t b) {
   return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}
void TFT::port_Init(void){
   setWriteDir();
   Command_Out();
}
//--------------------------------------------------------------
void TFT::SendCommand(uint8_t cmd){
	CD_COMMAND;//????? ? ????????? ??????? ???????
	RD_IDLE;//???????? ??????
	CS_ACTIVE;//????? ???????
	write(cmd);
	WR_STROBE;
	CS_IDLE;
}
//--------------------------------------------------------------
void TFT::SendData(uint8_t dt){
	CD_DATA;//????? ? ????????? ??????? ??????
	RD_IDLE;//???????? ??????
	CS_ACTIVE;//????? ???????
	write(dt);
	WR_STROBE;
	CS_IDLE;
}
//--------------------------------------------------------------
void TFT::Write8(uint8_t dt){
	write(dt);
	WR_STROBE;
}
void TFT::WriteCMD(uint8_t CMD){
	CD_COMMAND;
	Write8(CMD);
	CD_DATA;
}
//--------------------------------------------------------------
uint32_t TFT::ReadReg(uint8_t r){
	uint32_t id;
	uint8_t x;
	CS_ACTIVE;//????? ???????
	CD_COMMAND;//????? ? ????????? ??????? ???????
	Write8(r);
	setReadDir();
	CD_DATA;
	_delay_us(50);
	RD_ACTIVE;
	_delay_us(5);
	x = read();
	RD_IDLE;
	id = x;
	id <<= 8;
	RD_ACTIVE;
	_delay_us(5);
	x = read();
	RD_IDLE;
	id |= x;
	id <<= 8;
	RD_ACTIVE;
	_delay_us(5);
	x = read();
	RD_IDLE;
	id |= x;
	id <<= 8;
	RD_ACTIVE;
	_delay_us(5);
	x = read();
	RD_IDLE;
	id |= x;
	if(r==0xEF){
		id<<=8;
		RD_ACTIVE;
		_delay_us(5);
		x = read();
		RD_IDLE;
		id |= x;
	}
	CS_IDLE;
	setWriteDir();
	_delay_us(150);//stabilization time
	return id;
}
//--------------------------------------------------------------
void TFT::reset(void){
	CS_IDLE;
	WR_IDLE;
	RD_IDLE;
	RESET_ACTIVE;
	HAL_Delay(2);
	RESET_IDLE;
	CS_ACTIVE;
	SendCommand(0x01); //Software Reset
	for (uint8_t i=0;i<3;i++) WR_STROBE;
	CS_IDLE;
}
//--------------------------------------------------------------
void TFT::setRotation(uint8_t r){
	SendCommand(0x36);
	switch(r){
		case 0:
		SendData(0x48);
		width = Y_SIZE;
		height = X_SIZE;
		break;
		case 1:
		SendData(0x28);
		width = X_SIZE;
		height = Y_SIZE;
		break;
		case 2:
		SendData(0x88);
		width = Y_SIZE;
		height = X_SIZE;
		break;
		case 3:
		SendData(0xE8);
		width = X_SIZE;
		height = Y_SIZE;
		break;
	}
}
//--------------------------------------------------------------
uint16_t TFT::Width(void) {
	return width;
}
//--------------------------------------------------------------
uint16_t TFT::Height(void) {
	return height;
}
//--------------------------------------------------------------
void TFT::Flood(uint16_t color, uint32_t len){
	uint16_t blocks;
	uint8_t i, hi = color>>8, lo=color;
	CS_ACTIVE;
	WriteCMD(0x2C);
	len--;
	blocks=(uint16_t)(len/64);//64 pixels/block
	if (hi==lo){
		Write8(hi);
		Write8(lo);
		while(blocks--){
			i=16;
			do {
				WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE;//2bytes/pixel
				WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE;//x4 pixel
			} while (--i);
		}
		//Fill any remaining pixels(1 to 64)
		for (i=(uint8_t)len&63;i--;){
			WR_STROBE;
			WR_STROBE;
		}
	}
	else{
//		while(blocks--){
//			i=16;
//			do{
//				Write8(hi); Write8(lo); Write8(hi); Write8(lo);
//				Write8(hi); Write8(lo); Write8(hi); Write8(lo);
//			} while (--i);
//		}
//		//Fill any remaining pixels(1 to 64)
//		for (i=(uint8_t)len&63;i--;){
//			Write8(hi);
//			Write8(lo);
//		}
	    for (uint32_t i=0; i<width*height; i++ ) {
	    	Write8(hi);
	    	Write8(lo);
		  }
	}
	CS_IDLE;
}
//--------------------------------------------------------------
void TFT::WriteRegister32(uint8_t r, uint32_t d){
	CS_ACTIVE;
	WriteCMD(r);
	Write8(d>>24);
	Write8(d>>16);
	Write8(d>>8);
	Write8(d);
	CS_IDLE;
}
//--------------------------------------------------------------
void TFT::setAddrWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	uint32_t t;
	CS_ACTIVE;
	t = x1;
	t<<=16;
	t |= x2;
	WriteRegister32(0x2A,t);//Column Addres Set
	t = y1;
	t<<=16;
	t |= y2;
	WriteRegister32(0x2B,t);//Page Addres Set
	CS_IDLE;
}
//--------------------------------------------------------------
void TFT::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
	uint8_t hi = color>>8, lo = color & 0xFF;
	setAddrWindow(x, y, x+w-1, y+h-1);
	CS_ACTIVE;
	WriteCMD(0x2C);
	if(hi == lo) {
	  Write8(hi);
	  Write8(lo);
	  CD_DATA;
	  for(uint32_t y=0; y<(w*h); y++){
	    WR_ACTIVE;  // WR 0
	    WR_IDLE; // WR 1
	    WR_ACTIVE;  // WR 0
		WR_IDLE; // WR 1
	  }
	}
	else {
		for (uint32_t i=0; i<w*h; i++ ) {
		  Write8(hi);
		  Write8(lo);
	    }
	}
	CS_IDLE;
	setAddrWindow(0, 0, Width() - 1, Height() - 1);
}
//--------------------------------------------------------------
void TFT::fillScreen(uint16_t color){
	setAddrWindow(0,0,width-1,height-1);
//	CS_ACTIVE;
////	WriteCmd(_MW);
//	Write8(0x2C);
//	for (uint16_t i=0; i<height; i++ ) {
//	  for (uint16_t j=0; j<width; j++) {
//		  Write8(color>>8);
//		  Write8(color);
//	  }
//	}
//	CS_IDLE;
//	setAddrWindow(0, 0, width-1, height-1);
	Flood(color,width*height);
}
//--------------------------------------------------------------
void TFT::fillRectangle(uint16_t color,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	setAddrWindow(x1, y1, x2, y2);
	Flood(color, (x2-x1+1) * (y2-y1+1));
}
//--------------------------------------------------------------
void TFT::drawPixel(uint16_t x, uint16_t y, uint16_t color){
	if((x<0)||(y<0)||(x>=width)||(y>=height)) return;
	CS_ACTIVE;
	setAddrWindow(x,y,width-1,height-1);
	CS_ACTIVE;
	CD_COMMAND;
	Write8(0x2C);
	CD_DATA;
	Write8(color>>8);
	Write8(color);
	CS_IDLE;
}
//--------------------------------------------------------------
void TFT::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep;
    if (y0 < y1) { ystep = 1;
    } else { ystep = -1; }
    for (; x0<=x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}
//--------------------------------------------------------------
void TFT::drawRect(uint16_t color,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	drawLine(color,x1,y1,x2,y1);
	drawLine(color,x2,y1,x2,y2);
	drawLine(color,x1,y1,x1,y2);
	drawLine(color,x1,y2,x2,y2);
}
//--------------------------------------------------------------
void TFT::drawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color){
	int f = 1-r;
	int ddF_x=1;
	int ddF_y=-2*r;
	int x = 0;
	int y = r;
	drawPixel(x0,y0+r,color);
	drawPixel(x0,y0-r,color);
	drawPixel(x0+r,y0,color);
	drawPixel(x0-r,y0,color);
	while (x<y){
		if (f>=0){
			y--;
			ddF_y+=2;
			f+=ddF_y;
		}
		x++;
		ddF_x+=2;
		f+=ddF_x;
		drawPixel(x0+x,y0+y,color);
		drawPixel(x0-x,y0+y,color);
		drawPixel(x0+x,y0-y,color);
		drawPixel(x0-x,y0-y,color);
		drawPixel(x0+y,y0+x,color);
		drawPixel(x0-y,y0+x,color);
		drawPixel(x0+y,y0-x,color);
		drawPixel(x0-y,y0-x,color);
	}
}
//--------------------------------------------------------------
uint16_t TFT::randColor(void){
	uint8_t c = rand()%19;
  return COLOR[c];
}
//--------------------------------------------------------------
uint32_t TFT::readID(void){
	return ReadReg(0xD3);
}
//--------------------------------------------------------------
void TFT::readGram(uint16_t x, uint16_t y, uint16_t *block, uint16_t w, uint16_t h){
	uint8_t r, g, b;
	uint16_t color;
	setAddrWindow(x, y, x+w-1, y+h-1);
	CS_ACTIVE;
	WriteCMD(0x2E);
	setReadDir();
    for(uint16_t i=0; i<(w*h);i++){
		READ_16(color);
		color = (color >> 8) | (color << 8);
		*block++ = color;
    }
	RD_IDLE;
	CS_IDLE;
	setWriteDir();
	setAddrWindow(0, 0, Width() - 1, Height() - 1);
//	return color;
}
//--------------------------------------------------------------
uint16_t TFT::readPixel(uint16_t x, uint16_t y) {
	uint8_t r, g, b;
	uint16_t color;
	setAddrWindow(x, y, x, y);
	CS_ACTIVE;
	WriteCMD(0x2E);
	setReadDir();

//	READ_8(r);
//	READ_8(g);
//	READ_8(b);
//    color = color565(b, g, r);
	READ_16(color);
	color = (color >> 8) | (color << 8);

	RD_IDLE;
	CS_IDLE;
	setWriteDir();
	setAddrWindow(0, 0, Width() - 1, Height() - 1);
	return color;
}
//--------------------------------------------------------------
void TFT::drawRGBBitmap(uint16_t x, uint16_t y, const uint16_t* bitmap, uint16_t w, uint16_t h){
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    CS_ACTIVE;
    WriteCMD(0x2C);
    for (uint16_t i=0; i<h; i++ ) {
      for (uint16_t j=0; j<w; j++) {
          Write8(bitmap[j+i*w]>>8);
          Write8(bitmap[j+i*w]);
      }
    }
    CS_IDLE;
    setAddrWindow(0, 0, Width() - 1, Height() - 1);
}
//--------------------------------------------------------------
void TFT::testLines(void){
  int x1, y1, x2, y2,
	  w = Width(),
	  h = Height();
    fillScreen(BLACK);
    x1 = y1 = 0;
    y2    = h - 1;
    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, GREEN);
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) drawLine(x1, y1, x2, y2, GREEN);
//    fillScreen(BLACK);
//    x1    = w - 1;
//    y1    = 0;
//    y2    = h - 1;
//    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, color);
//    x2    = 0;
//    for (y2 = 0; y2 < h; y2 += 6) drawLine(x1, y1, x2, y2, color);
////    fillScreen(BLACK);
//    x1    = 0;
//    y1    = h - 1;
//    y2    = 0;
//    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, color);
//    x2    = w - 1;
//    for (y2 = 0; y2 < h; y2 += 6) drawLine(x1, y1, x2, y2, color);
////    fillScreen(BLACK);
//    x1    = w - 1;
//    y1    = h - 1;
//    y2    = 0;
//    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, color);
//    x2    = 0;
//    for (y2 = 0; y2 < h; y2 += 6) drawLine(x1, y1, x2, y2, color);
}
//--------------------------------------------------------------
void TFT::Init(void){

	port_Init();
	reset();
	HAL_Delay(100);
	CS_ACTIVE;
	/*/////////////////////////////////////////////ILI9341/////////////////////////////////////////////////////////////*/
//	SendCommand(0x01);//Software Reset
//	SendCommand(0xCB);//Power Control A
//	SendData(0x39);
//	SendData(0x2C);
//	SendData(0x00);
//	SendData(0x34);
//	SendData(0x02);
//	SendCommand(0xCF);//Power Control B
//	SendData(0x00);
//	SendData(0xC1);
//	SendData(0x30);
//	SendCommand(0xE8);//Driver timing control A
//	SendData(0x85);
//	SendData(0x00);
//	SendData(0x78);
//	SendCommand(0xEA);//Driver timing control B
//	SendData(0x00);
//	SendData(0x00);
//	SendCommand(0xED);//Power on Sequence control
//	SendData(0x64);
//	SendData(0x03);
//	SendData(0x12);
//	SendData(0x81);
//	SendCommand(0xF7);//Pump ratio control
//	SendData(0x20);
//	SendCommand(0xC0);//Power Control 1
//	SendData(0x10);
//	SendCommand(0xC1);//Power Control 2
//	SendData(0x10);
//	SendCommand(0xC5);//VCOM Control 1
//	SendData(0x3E);
//	SendData(0x28);
//	SendCommand(0xC7);//VCOM Control 2
//	SendData(0x86);
//	setRotation(0);
//	SendCommand(0x3A);//Pixel Format Set
//	SendData(0x55);//16bit
//	SendCommand(0xB1);
//	SendData(0x00);
//	SendData(0x18);// ??????? ?????? 79 ??
//	SendCommand(0xB6);//Display Function Control
//	SendData(0x08);
//	SendData(0x82);
//	SendData(0x27);//320 ?????
//	SendCommand(0xF2);//Enable 3G (???? ?? ???? ??? ??? ?? ?????)
//	SendData(0x00);//?? ????????
//	SendCommand(0x26);//Gamma set
//	SendData(0x01);//Gamma Curve (G2.2) (?????? ???????? ?????)
//	SendCommand(0xE0);//Positive Gamma  Correction
//	SendData(0x0F);
//	SendData(0x31);
//	SendData(0x2B);
//	SendData(0x0C);
//	SendData(0x0E);
//	SendData(0x08);
//	SendData(0x4E);
//	SendData(0xF1);
//	SendData(0x37);
//	SendData(0x07);
//	SendData(0x10);
//	SendData(0x03);
//	SendData(0x0E);
//	SendData(0x09);
//	SendData(0x00);
//	SendCommand(0xE1);//Negative Gamma  Correction
//	SendData(0x00);
//	SendData(0x0E);
//	SendData(0x14);
//	SendData(0x03);
//	SendData(0x11);
//	SendData(0x07);
//	SendData(0x31);
//	SendData(0xC1);
//	SendData(0x48);
//	SendData(0x08);
//	SendData(0x0F);
//	SendData(0x0C);
//	SendData(0x31);
//	SendData(0x36);
//	SendData(0x0F);
//	SendCommand(0x11);//?????? ?? ??????? ?????
//	HAL_Delay(100);
//	SendCommand(0x29);//????????? ???????
//	SendData(0x2C);
//	HAL_Delay(100);
	/*/////////////////////////////////S6D05A1/////////////////////////////////////*/
	SendCommand(0xF0); // PASSWD1 - OK
	SendData(0x5A);
	SendData(0x5A);

	SendCommand(0xF1); // PASSWD2 - OK
	SendData(0x5A);
	SendData(0x5A);

	SendCommand(0xF2); // DISCTL (Display Control)
	SendData(0x3B);    // 480 scan lines
	SendData(0x33);
	SendData(0x03);
	SendData(0x0C);
	SendData(0x08);
	SendData(0x08);
	SendData(0x08);
	SendData(0x00);
	SendData(0x08);
	SendData(0x08);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x33);
	SendData(0x0C);
	SendData(0x08);
	SendData(0x0C);
	SendData(0x08);

	SendCommand(0xF4);
	SendData(0x07);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x04);
	SendData(0x70);
	SendData(0x03);
	SendData(0x04);
	SendData(0x70);
	SendData(0x03);

	SendCommand(0xF5);
	SendData(0x00);
	SendData(0x46);//Set VCOMH
	SendData(0x70);//Set VCOM Amplitude
	SendData(0x00);
	SendData(0x00);
	SendData(0x02);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x46);
	SendData(0x70);

	SendCommand(0xF6);
	SendData(0x03);
	SendData(0x00);
	SendData(0x08);
	SendData(0x03);
	SendData(0x03);
	SendData(0x00);
	SendData(0x03);
	SendData(0x00);

	SendCommand(0xF7);
	SendData(0x00);//////////////////////////////////////
	SendData(0x80);//RGB_MCU(DM)
	SendData(0x10);
	SendData(0x02);
	SendData(0x00);

	SendCommand(0xF8);
	SendData(0x11);
	SendData(0x00);

	SendCommand(0xF9); //Gamma Selection
	SendData(0x14);

	SendCommand(0xFA); //Positive Gamma Control
	SendData(0x33);
	SendData(0x07);
	SendData(0x04);
	SendData(0x1A);
	SendData(0x18);
	SendData(0x1C);
	SendData(0x24);
	SendData(0x1D);
	SendData(0x26);
	SendData(0x28);
	SendData(0x2F);
	SendData(0x2E);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0xFB); //Positive Gamma Control
	SendData(0x33);
	SendData(0x03);
	SendData(0x00);
	SendData(0x2E);
	SendData(0x2F);
	SendData(0x28);
	SendData(0x26);
	SendData(0x1D);
	SendData(0x24);
	SendData(0x1C);
	SendData(0x18);
	SendData(0x1A);
	SendData(0x04);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0xF9); //Gamma Selection
	SendData(0x12);

	SendCommand(0xFA); //Positive Gamma Control
	SendData(0x36);
	SendData(0x07);
	SendData(0x04);
	SendData(0x1C);
	SendData(0x1C);
	SendData(0x23);
	SendData(0x28);
	SendData(0x1C);
	SendData(0x25);
	SendData(0x26);
	SendData(0x2E);
	SendData(0x2B);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0xFB); //Positive Gamma Control
	SendData(0x33);
	SendData(0x06);
	SendData(0x00);
	SendData(0x2B);
	SendData(0x2E);
	SendData(0x26);
	SendData(0x25);
	SendData(0x1C);
	SendData(0x28);
	SendData(0x23);
	SendData(0x1C);
	SendData(0x1C);
	SendData(0x04);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0xF9); //Gamma Selection
	SendData(0x11);

	SendCommand(0xFA); //Positive Gamma Control
	SendData(0x33);
	SendData(0x07);
	SendData(0x04);
	SendData(0x30);
	SendData(0x32);
	SendData(0x34);
	SendData(0x35);
	SendData(0x11);
	SendData(0x1D);
	SendData(0x20);
	SendData(0x28);
	SendData(0x20);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0xFB); //Positive Gamma Control
	SendData(0x33);
	SendData(0x03);
	SendData(0x00);
	SendData(0x20);
	SendData(0x28);
	SendData(0x20);
	SendData(0x1D);
	SendData(0x11);
	SendData(0x35);
	SendData(0x34);
	SendData(0x32);
	SendData(0x30);
	SendData(0x04);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0x44);
	SendData(0x00);
	SendData(0x01);

	SendCommand(0x2A);
	SendData(0x00);
	SendData(0x00);
	SendData(0x01);
	SendData(0x3F);

	SendCommand(0x2B);
	SendData(0x00);
	SendData(0x00);
	SendData(0x01);
	SendData(0xDF);

	SendCommand(0x36); //Memory Data Access Control
	SendData(0x08);

	SendCommand(0x3A); //SET 65K Color
	SendData(0x55);

	SendCommand(0x11);
	SendCommand(120);

	SendCommand(0x29);//Display on
	SendCommand(0x2C);//Write GRAM

	_delay_us(10);
	SendCommand(0x36); //Set_address_mode
	SendData(0x60); //???????????????????

}
