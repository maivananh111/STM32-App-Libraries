#include "lcd1280.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "font.h"
//////////////////////////////////////////////////////

/////////////////////////////////////////////////////
extern char LCD_RAM[LCD_X*LCD_STR];
volatile char PORT, SCL, SDA, CS, RST, PORTLED, LED;
/////////////////////////////////////////////////////
//#define SCL GPIO_PIN_4
//#define SDA GPIO_PIN_5
//#define CS  GPIO_PIN_6
//#define RST GPIO_PIN_7
//#define LED GPIO_PIN_13
/////////////////////////////////////////////////////
void lcd_Init(char port, char scl, char sda, char cs, char rst, char portLED, char led){
	PORT = port;
	SCL = scl;
	SDA = sda;
	CS = cs;
	RST = rst;
	PORTLED = portLED;
	LED = led;
	
	if(PORT == 'A'){
		HAL_GPIO_WritePin(GPIOA, RST, GPIO_PIN_RESET); 
	  HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOA, RST, GPIO_PIN_SET);   
    HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_RESET);     
	  HAL_GPIO_WritePin(GPIOA, SDA, GPIO_PIN_RESET);   
	  HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_RESET);			 
    HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_SET);		
	}
	if(PORT == 'B'){
		HAL_GPIO_WritePin(GPIOB, RST, GPIO_PIN_RESET); 
	  HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOB, RST, GPIO_PIN_SET);   
    HAL_GPIO_WritePin(GPIOB, SCL, GPIO_PIN_RESET);     
	  HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_RESET);   
	  HAL_GPIO_WritePin(GPIOB, CS, GPIO_PIN_RESET);			 
    HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOB, CS, GPIO_PIN_SET);		
	}
	if(PORT == 'C'){
		HAL_GPIO_WritePin(GPIOC, RST, GPIO_PIN_RESET); 
	  HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOC, RST, GPIO_PIN_SET);   
    HAL_GPIO_WritePin(GPIOC, SCL, GPIO_PIN_RESET);     
	  HAL_GPIO_WritePin(GPIOC, SDA, GPIO_PIN_RESET);   
	  HAL_GPIO_WritePin(GPIOC, CS, GPIO_PIN_RESET);			 
    HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOC, CS, GPIO_PIN_SET);		
	}
	if(PORT == 'D'){
		HAL_GPIO_WritePin(GPIOD, RST, GPIO_PIN_RESET); 
	  HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOD, RST, GPIO_PIN_SET);   
    HAL_GPIO_WritePin(GPIOD, SCL, GPIO_PIN_RESET);     
	  HAL_GPIO_WritePin(GPIOD, SDA, GPIO_PIN_RESET);   
	  HAL_GPIO_WritePin(GPIOD, CS, GPIO_PIN_RESET);			 
    HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOD, CS, GPIO_PIN_SET);		
	}
	if(PORT == 'E'){
		HAL_GPIO_WritePin(GPIOE, RST, GPIO_PIN_RESET); 
	  HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOE, RST, GPIO_PIN_SET);   
    HAL_GPIO_WritePin(GPIOE, SCL, GPIO_PIN_RESET);     
	  HAL_GPIO_WritePin(GPIOE, SDA, GPIO_PIN_RESET);   
	  HAL_GPIO_WritePin(GPIOE, CS, GPIO_PIN_RESET);			 
    HAL_Delay(5);
	  HAL_GPIO_WritePin(GPIOE, CS, GPIO_PIN_SET);		
	}
	
	LCD_SendByte(LCD_C, 0xE2);  
  HAL_Delay(5);
	LCD_SendByte(LCD_C, 0x3D);  
  LCD_SendByte(LCD_C, 0x02); 	
	LCD_SendByte(LCD_C, 0xE1); 
 	LCD_SendByte(LCD_C, 0x90); 
  LCD_SendByte(LCD_C, 0xA4);  	
	LCD_SendByte(LCD_C, 0x2F);  
	LCD_SendByte(LCD_C, 0xA0);  
  LCD_SendByte(LCD_C, 0xAF);  
	HAL_Delay(10);
	lcd_Clear();
	lcd_Update();
}
//////////////////////////////////////////////////////////
void LCD_SendByte(uint8_t mode, uint8_t c){
  switch(PORT){
	 case 'A':	
	  HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_RESET);
	  if (mode) { HAL_GPIO_WritePin(GPIOA, SDA, GPIO_PIN_SET); }
	  else { HAL_GPIO_WritePin(GPIOA, SDA, GPIO_PIN_RESET); }
	
	  HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_SET);
	  for(uint8_t i=0; i<8; i++) {
    HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_RESET);
		if (c & 0x80) { HAL_GPIO_WritePin(GPIOA, SDA, GPIO_PIN_SET); }
		else { HAL_GPIO_WritePin(GPIOA, SDA, GPIO_PIN_RESET); }
		HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_SET);
    c <<= 1;
	  }
	 HAL_GPIO_WritePin(GPIOA, SCL, GPIO_PIN_RESET);	
  break;	
		
	 case 'B':	
	  HAL_GPIO_WritePin(GPIOB, CS, GPIO_PIN_RESET);
	  if (mode) { HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_SET); }
	  else { HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_RESET); }
	
	  HAL_GPIO_WritePin(GPIOB, SCL, GPIO_PIN_SET);
	  for(uint8_t i=0; i<8; i++) {
    HAL_GPIO_WritePin(GPIOB, SCL, GPIO_PIN_RESET);
		if (c & 0x80) { HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_SET); }
		else { HAL_GPIO_WritePin(GPIOB, SDA, GPIO_PIN_RESET); }
		HAL_GPIO_WritePin(GPIOB, SCL, GPIO_PIN_SET);
    c <<= 1;
	  }
	 HAL_GPIO_WritePin(GPIOB, SCL, GPIO_PIN_RESET);	
  break;	

	 case 'C':	
	  HAL_GPIO_WritePin(GPIOC, CS, GPIO_PIN_RESET);
	  if (mode) { HAL_GPIO_WritePin(GPIOC, SDA, GPIO_PIN_SET); }
	  else { HAL_GPIO_WritePin(GPIOC, SDA, GPIO_PIN_RESET); }
	
	  HAL_GPIO_WritePin(GPIOC, SCL, GPIO_PIN_SET);
	  for(uint8_t i=0; i<8; i++) {
    HAL_GPIO_WritePin(GPIOC, SCL, GPIO_PIN_RESET);
		if (c & 0x80) { HAL_GPIO_WritePin(GPIOC, SDA, GPIO_PIN_SET); }
		else { HAL_GPIO_WritePin(GPIOC, SDA, GPIO_PIN_RESET); }
		HAL_GPIO_WritePin(GPIOC, SCL, GPIO_PIN_SET);
    c <<= 1;
	  }
	 HAL_GPIO_WritePin(GPIOC, SCL, GPIO_PIN_RESET);	
  break;
		
	 case 'D':	
	  HAL_GPIO_WritePin(GPIOD, CS, GPIO_PIN_RESET);
	  if (mode) { HAL_GPIO_WritePin(GPIOD, SDA, GPIO_PIN_SET); }
	  else { HAL_GPIO_WritePin(GPIOD, SDA, GPIO_PIN_RESET); }
	
	  HAL_GPIO_WritePin(GPIOD, SCL, GPIO_PIN_SET);
	  for(uint8_t i=0; i<8; i++) {
    HAL_GPIO_WritePin(GPIOD, SCL, GPIO_PIN_RESET);
		if (c & 0x80) { HAL_GPIO_WritePin(GPIOD, SDA, GPIO_PIN_SET); }
		else { HAL_GPIO_WritePin(GPIOD, SDA, GPIO_PIN_RESET); }
		HAL_GPIO_WritePin(GPIOD, SCL, GPIO_PIN_SET);
    c <<= 1;
	  }
	 HAL_GPIO_WritePin(GPIOD, SCL, GPIO_PIN_RESET);	
  break;	
		
	case 'E':	
	  HAL_GPIO_WritePin(GPIOE, CS, GPIO_PIN_RESET);
	  if (mode) { HAL_GPIO_WritePin(GPIOE, SDA, GPIO_PIN_SET); }
	  else { HAL_GPIO_WritePin(GPIOE, SDA, GPIO_PIN_RESET); }
	
	  HAL_GPIO_WritePin(GPIOE, SCL, GPIO_PIN_SET);
	  for(uint8_t i=0; i<8; i++) {
    HAL_GPIO_WritePin(GPIOE, SCL, GPIO_PIN_RESET);
		if (c & 0x80) { HAL_GPIO_WritePin(GPIOE, SDA, GPIO_PIN_SET); }
		else { HAL_GPIO_WritePin(GPIOE, SDA, GPIO_PIN_RESET); }
		HAL_GPIO_WritePin(GPIOE, SCL, GPIO_PIN_SET);
    c <<= 1;
	  }
	 HAL_GPIO_WritePin(GPIOE, SCL, GPIO_PIN_RESET);	
  break;
 }
}
/////////////////////////////////////////////////////////////
void lcd_Clear(){
	for (int index = 0; index < 864 ; index++){
     LCD_RAM[index] = (0x00);
  }
}
/////////////////////////////////////////////////////////////
void lcd_Update(){
	for(int p = 0; p < 9; p++){
    LCD_SendByte(LCD_C, SetYAddr| p); 
    LCD_SendByte(LCD_C, SetXAddr4);
    LCD_SendByte(LCD_C, SetXAddr3);
    for(int col=0; col < LCD_X; col++){
      LCD_SendByte(LCD_D, LCD_RAM[(LCD_X * p) + col]);
    }
  }
}
//////////////////////////////////////////////////////////////
void lcd_SetBL(char mode){
	if(mode == 'O'){
		switch(PORTLED){
		 case 'A':
		  HAL_GPIO_WritePin(GPIOA, LED, GPIO_PIN_SET);
		 break;
		 case 'B':
		  HAL_GPIO_WritePin(GPIOB, LED, GPIO_PIN_SET);
		 break;
		 case 'C':
		  HAL_GPIO_WritePin(GPIOC, LED, GPIO_PIN_SET);
		 break;
		 case 'D':
		  HAL_GPIO_WritePin(GPIOD, LED, GPIO_PIN_SET);
		 break;
		 case 'E':
		  HAL_GPIO_WritePin(GPIOE, LED, GPIO_PIN_SET);
		 break;
	  }
  }
	if(mode == 'F'){
		switch(PORT){
		 case 'A':
		  HAL_GPIO_WritePin(GPIOA, LED, GPIO_PIN_RESET);
		 break;
		 case 'B':
		  HAL_GPIO_WritePin(GPIOB, LED, GPIO_PIN_RESET);
		 break;
		 case 'C':
		  HAL_GPIO_WritePin(GPIOC, LED, GPIO_PIN_RESET);
		 break;
		 case 'D':
		  HAL_GPIO_WritePin(GPIOD, LED, GPIO_PIN_RESET);
		 break;
		 case 'E':
		  HAL_GPIO_WritePin(GPIOE, LED, GPIO_PIN_RESET);
		 break;
	  }
	}
}
///////////////////////////////////////////////////////////
void lcd_DrawPixel (uint8_t x, uint8_t y, uint8_t color) {
  if ((x < 0) || (x >= LCD_X) || (y < 0) || (y >= LCD_Y)) return;

  if (color) LCD_RAM[x+ (y/8)*LCD_X] |= 1<<(y%8);
  else       LCD_RAM[x+ (y/8)*LCD_X] &= ~(1<<(y%8)); 
}
////////////////////////////////////////////////////////////
void lcd_DrawChar(uint8_t x, uint8_t y, uint8_t color, unsigned char c) {
  if((x >= LCD_X) ||(y >= LCD_Y) || ((x + 4) < 0) || ((y + 7) < 0)) return;
  if(c<128)            c = c-32;
  if(c>=144 && c<=175) c = c-48;
  if(c>=128 && c<=143) c = c+16;
  if(c>=176 && c<=191) c = c-48;
  if(c>191)  return;
  for (uint8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) {line = 0x00;}
    else {line = font[(c*5)+i];
    for (uint8_t j = 0; j<8; j++)
			{
				if (line & 0x01) {lcd_DrawPixel(x+i, y+j, color);}
				else {lcd_DrawPixel(x+i, y+j, !color);};
				line >>= 1;
			}
		}
  }
}
////////////////////////////////////////////////////////////
void lcd_print(uint8_t x, uint8_t y, uint8_t color, char *str) {
  unsigned char type = *str;
  if(type>=128) x=x-3;
  while(*str){ 
    lcd_DrawChar(x, y, color, *str++);
    unsigned char type = *str;
    if (type>=128) {x=x+3;}
    else {x=x+6;};
  }
}
///////////////////////////////////////////////////////////
void lcd_write(uint8_t x, uint8_t y, uint8_t color, float num, uint8_t type){
  char c[10];
	if(type){
	 int val = (int)num;
	 sprintf(c, "%5d", val);
	}
	else {
	 sprintf(c, "%5.3f", num);
	}
  lcd_print(x, y, color, c);
}
///////////////////////////////////////////////////////////
void lcd_DrawBitmap(uint8_t x, uint8_t y, const char *bitmap, uint8_t w, uint8_t h, uint8_t color) {
  for (int16_t j=0; j<h; j++) {
    for (int16_t i=0; i<w; i++ ) {
      if (bitmap[i + (j/8)*w] & 1<<(j%8)) { lcd_DrawPixel(x+i, y+j, color); }
    }
  }
}
//////////////////////////////////////////////////////////
void lcd_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
  int steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }
  int dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);
  int err = dx / 2;
  int ystep;
  if (y0 < y1) {ystep = 1;}
  else {ystep = -1;};
  for ( ; x0 <= x1; x0++) {
    if (steep) {lcd_DrawPixel(y0, x0, color);}
    else {lcd_DrawPixel(x0, y0, color);};
		err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
/////////////////////////////////////////////////////////////////////////
void lcd_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color) {
  lcd_DrawLine(x, y, x, y+h-1, color);
}
/////////////////////////////////////////////////////////////////////////
void lcd_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color) {
  lcd_DrawLine(x, y, x+w-1, y, color);
}
/////////////////////////////////////////////////////////////////////////
void lcd_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  lcd_DrawFastHLine(x, y, w, color);
  lcd_DrawFastHLine(x, y+h-1, w, color);
  lcd_DrawFastVLine(x, y, h, color);
  lcd_DrawFastVLine(x+w-1, y, h, color);
}
/////////////////////////////////////////////////////////////////////////
void lcd_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  for (int16_t i=x; i<x+w; i++) {
    lcd_DrawFastVLine(i, y, h, color);
  }
}
/////////////////////////////////////////////////////////////////////////
void lcd_FillScreen(uint8_t color) {
  lcd_FillRect(0, 0, LCD_X, LCD_Y, color);
}
/////////////////////////////////////////////////////////////////////////
void lcd_DrawCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, uint8_t color) {
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      lcd_DrawPixel(x0 + x, y0 + y, color);
      lcd_DrawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      lcd_DrawPixel(x0 + x, y0 - y, color);
      lcd_DrawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      lcd_DrawPixel(x0 - y, y0 + x, color);
      lcd_DrawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      lcd_DrawPixel(x0 - y, y0 - x, color);
      lcd_DrawPixel(x0 - x, y0 - y, color);
    }
  }
}
/////////////////////////////////////////////////////////////////////////
void lcd_DrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color) {
  lcd_DrawFastHLine(x+r , y , w-2*r, color); // Top
  lcd_DrawFastHLine(x+r , y+h-1, w-2*r, color); // Bottom
  lcd_DrawFastVLine( x , y+r , h-2*r, color); // Left
  lcd_DrawFastVLine( x+w-1, y+r , h-2*r, color); // Right
  lcd_DrawCircleHelper(x+r , y+r , r, 1, color);
  lcd_DrawCircleHelper(x+w-r-1, y+r , r, 2, color);
  lcd_DrawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  lcd_DrawCircleHelper(x+r , y+h-r-1, r, 8, color);
}
/////////////////////////////////////////////////////////////////////////
void lcd_FillCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, uint8_t delta, uint8_t color) {
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;
  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x1) {
      lcd_DrawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      lcd_DrawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      lcd_DrawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      lcd_DrawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}
/////////////////////////////////////////////////////////////////////////
void lcd_FillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color) {
  lcd_FillRect(x+r, y, w-2*r, h, color);
  lcd_FillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  lcd_FillCircleHelper(x+r , y+r, r, 2, h-2*r-1, color);
}
/////////////////////////////////////////////////////////////////////////
void lcd_DrawCircle(uint8_t x0, uint8_t y0, int16_t r, uint8_t color) {
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;

  lcd_DrawPixel(x0, y0+r, color);
  lcd_DrawPixel(x0, y0-r, color);
  lcd_DrawPixel(x0+r, y0, color);
  lcd_DrawPixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    lcd_DrawPixel(x0 + x, y0 + y, color);
    lcd_DrawPixel(x0 - x, y0 + y, color);
    lcd_DrawPixel(x0 + x, y0 - y, color);
    lcd_DrawPixel(x0 - x, y0 - y, color);
    lcd_DrawPixel(x0 + y, y0 + x, color);
    lcd_DrawPixel(x0 - y, y0 + x, color);
    lcd_DrawPixel(x0 + y, y0 - x, color);
    lcd_DrawPixel(x0 - y, y0 - x, color);
  }
}
/////////////////////////////////////////////////////////////////////////
void lcd_FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color) {
  lcd_DrawFastVLine(x0, y0-r, 2*r+1, color);
  lcd_FillCircleHelper(x0, y0, r, 3, 0, color);
}
/////////////////////////////////////////////////////////////////////////
void lcd_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
  lcd_DrawLine(x0, y0, x1, y1, color);
  lcd_DrawLine(x1, y1, x2, y2, color);
  lcd_DrawLine(x2, y2, x0, y0, color);
}
/////////////////////////////////////////////////////////////////////////
void lcd_FillTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
  int a, b, y, last;
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if(y0 == y2) { 
    a = b = x0;
    if(x1 < a) a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a) a = x2;
    else if(x2 > b) b = x2;
    lcd_DrawFastHLine(a, y0, b-a+1, color);
    return;
  }
  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa = 0,
    sb = 0;
  if(y1 == y2) last = y1; 
  else last = y1-1; 
  for(y=y0; y<=last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if(a > b) swap(a,b);
    lcd_DrawFastHLine(a, y, b-a+1, color);
  }
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if(a > b) swap(a,b);
    lcd_DrawFastHLine(a, y, b-a+1, color);
  }
}
/////////////////////////////////////////////////////////////////////////
void lcd_DrawNum(uint8_t x, uint8_t y, uint8_t color, uint8_t c, uint8_t size){
	uint8_t line;
  for (uint8_t k=0;k<4;k++){
		
		if(size == 1){
    for (uint8_t i=0;i<10;i++){
       line = mass10x16[c][i+k*10];
      for (uint8_t j = 0; j<8; j++) {
        (line & 0x01)? lcd_DrawPixel(x+i, y+j+k*8, color) : lcd_DrawPixel(x+i, y+j+k*8, !color);
        line >>= 1;
      }
    }
	}
		if(size == 2){
			for (uint8_t i=0;i<15;i++){
       line = Font15x25[c][i+k*15];
      for (uint8_t j = 0; j<8; j++) {
        (line & 0x01)? lcd_DrawPixel(x+i, y+j+k*8, color) : lcd_DrawPixel(x+i, y+j+k*8, !color);
        line >>= 1;
      }
    }
	}
		else {
			for (uint8_t i=0;i<16;i++){
       line = mass16x32[c][i+k*16];
      for (uint8_t j = 0; j<8; j++) {
        (line & 0x01)? lcd_DrawPixel(x+i, y+j+k*8, color) : lcd_DrawPixel(x+i, y+j+k*8, !color);
        line >>= 1;
      }
    }
	}
  }
}
/////////////////////////////////////////////////////////////////////////