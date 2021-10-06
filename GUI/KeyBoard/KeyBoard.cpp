/*
 * KeyBoard.cpp
 *
 *  Created on: 25 thg 9, 2021
 *      Author: A315-56
 */

#include "KeyBoard.h"


static const G_Font *KeyFont;
static uint8_t W, H;
static uint16_t Key_Y, Key_Height;

static uint8_t Key_Char_HOA[29] = {
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		   'Z', 'X', 'C', 'V', 'B', 'N', 'M',
					 ',', ' ', '.'
};
static uint8_t Key_Char_THUONG[29] = {
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
		   'z', 'x', 'c', 'v', 'b', 'n', 'm',
					 ',', ' ', '.'
};
static uint8_t Key_Num_THUONG[29] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	  '^', '/', '*', '@', '#', '$', '%', '&', '=',
		   ':', '-', '+', ';', '?', '(', ')',
					 ',', ' ', '.'
};
static uint8_t Key_Num_HOA[29] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	  '~', '|', '!', '"', '[', ']', '`', '&', '=',
		   '<', '_', ';', '>', '?', '{', '}',
					 ',', ' ', '.'
};

static const uint8_t Enter_Bitmap[] = {
		0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe,
		0x02, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t BackSpace_Bitmap[] = {
		0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0x0e, 0x7e, 0x7e, 0x7e, 0x7e, 0xfe, 0xce, 0x8e, 0x1e, 0x3e, 0x7e, 0xfe, 0x7e, 0x3e, 0x1e, 0x8e, 0xce, 0xfe, 0xfc, 0xf8, 0x00,
		0x00, 0x07, 0x0f, 0x1f, 0x3d, 0x78, 0xf0, 0xe0, 0xc0, 0x80, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0x9f, 0x8f, 0xc7, 0xe2, 0xf0, 0xf8, 0xf0, 0xe2, 0xc7, 0x8f, 0x9f, 0xff, 0xff, 0xff, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00
};
static const uint8_t Capslock_Bitmap[] = {
		0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};
static const uint8_t UnCapslock_Bitmap[] = {
		0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0x60, 0x20, 0x90, 0x50, 0x30, 0x00,
		0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x81, 0x7f, 0x3f, 0x9f, 0xcf, 0xe7, 0xf3, 0xf9, 0x04, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x18, 0x14, 0x12, 0x09, 0x04, 0x1a, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const uint8_t Space_Bitmap48x12[] = {
        0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0,
        0x0f, 0x0f, 0x0f, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f
};
const uint8_t Left_Bitmap[] = {
	    0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0c, 0x0c, 0x0c, 0x00,
	    0x0e, 0x1f, 0x3f, 0x7f, 0xee, 0xce, 0x8e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x06, 0x06, 0x06, 0x00
};
const uint8_t Right_Bitmap[] = {
	    0x00, 0x0c, 0x0c, 0x0c, 0x1c, 0x38, 0x70, 0xe0, 0xc0, 0x80, 0x00, 0x00,
	    0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x8e, 0xce, 0xee, 0x7f, 0x3f, 0x1f, 0x0e,
	    0x00, 0x06, 0x06, 0x06, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};
void Draw_Key_Board_Button(uint16_t Key_location, uint8_t Height_of_Key) {
	   Key_Y = Key_location;
	   Key_Height = Height_of_Key;
	   /*Các Phím Cơ Bản*/
		for(uint16_t j=0; j<10; j++) {
		  lcd.fillRoundRect(Button_Row1_Start + (j*Button_Hori_Print), Key_Y, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Back_Color);
		  lcd.drawRoundRect(Button_Row1_Start + (j*Button_Hori_Print), Key_Y, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Bord_Color);
		}
		for(uint16_t j=0; j<9; j++) {
		  lcd.fillRoundRect(Button_Row2_Start + (j*Button_Hori_Print), Key_Y+Key_Height+Button_Vert_Distance, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Back_Color);
		  lcd.drawRoundRect(Button_Row2_Start + (j*Button_Hori_Print), Key_Y+Key_Height+Button_Vert_Distance, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Bord_Color);
		}
		for(uint16_t j=0; j<10; j++) {
		  lcd.fillRoundRect(Button_Row1_Start + (j*Button_Hori_Print), Key_Y+Key_Height*2+2*Button_Vert_Distance, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Back_Color);
		  lcd.drawRoundRect(Button_Row1_Start + (j*Button_Hori_Print), Key_Y+Key_Height*2+2*Button_Vert_Distance, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Bord_Color);
		}
       /* 2 Phím dịch chuyển vị trí con trỏ ký tự */
		lcd.fillRoundRect(Button_Row1_Start, Key_Y+Key_Height+Button_Vert_Distance, (Button_Hori_Print - Button_Hori_Distance)/2-1, Key_Height, Button_Curv_Bord, Back_Color);
	    lcd.drawRoundRect(Button_Row1_Start, Key_Y+Key_Height+Button_Vert_Distance, (Button_Hori_Print - Button_Hori_Distance)/2-1, Key_Height, Button_Curv_Bord, Bord_Color);

	    lcd.fillRoundRect(Button_Row2_Start + (9*Button_Hori_Print), Key_Y+Key_Height+Button_Vert_Distance, (Button_Hori_Print - Button_Hori_Distance)/2-1, Key_Height, Button_Curv_Bord, Back_Color);
	    lcd.drawRoundRect(Button_Row2_Start + (9*Button_Hori_Print), Key_Y+Key_Height+Button_Vert_Distance, (Button_Hori_Print - Button_Hori_Distance)/2-1, Key_Height, Button_Curv_Bord, Bord_Color);
       /* Nút chọn kiểu phím và Enter */
		lcd.fillRoundRect(Button_Row1_Start, Key_Y+Key_Height*3+3*Button_Vert_Distance, Button_Row3_Start - Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance)/2), Key_Height, Button_Curv_Bord, Back_Color);
		lcd.drawRoundRect(Button_Row1_Start, Key_Y+Key_Height*3+3*Button_Vert_Distance, Button_Row3_Start - Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance)/2), Key_Height, Button_Curv_Bord, Char_Color);

		lcd.fillRoundRect(Button_Row3_Start + (7*Button_Hori_Print) + ((Button_Hori_Print - Button_Hori_Distance)/2-1), Key_Y+Key_Height*3+3*Button_Vert_Distance, Button_Row3_Start - Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance)/2), Key_Height, Button_Curv_Bord, Back_Color);
		lcd.drawRoundRect(Button_Row3_Start + (7*Button_Hori_Print) + ((Button_Hori_Print - Button_Hori_Distance)/2-1), Key_Y+Key_Height*3+3*Button_Vert_Distance, Button_Row3_Start - Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance)/2), Key_Height, Button_Curv_Bord, Char_Color);
		/* Nút '.' và ',' */
		lcd.fillRoundRect(Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + Button_Hori_Distance, Key_Y+Key_Height*3+3*Button_Vert_Distance, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Back_Color);
		lcd.drawRoundRect(Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + Button_Hori_Distance, Key_Y+Key_Height*3+3*Button_Vert_Distance, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Bord_Color);

		lcd.fillRoundRect(Button_Row3_Start + (6*Button_Hori_Print)+ ((Button_Hori_Print - Button_Hori_Distance)/2), Key_Y+Key_Height*3+3*Button_Vert_Distance, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Back_Color);
		lcd.drawRoundRect(Button_Row3_Start + (6*Button_Hori_Print)+ ((Button_Hori_Print - Button_Hori_Distance)/2), Key_Y+Key_Height*3+3*Button_Vert_Distance, Button_Hori_Print - Button_Hori_Distance, Key_Height, Button_Curv_Bord, Bord_Color);
       /* Dấu cách */
		lcd.fillRoundRect(Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + 2*Button_Hori_Distance + Button_Hori_Print - Button_Hori_Distance, Key_Y+Key_Height*3+3*Button_Vert_Distance,
				(Button_Row3_Start + (6*Button_Hori_Print)+ ((Button_Hori_Print - Button_Hori_Distance)/2) - Button_Hori_Distance) - (Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + Button_Hori_Distance + Button_Hori_Print), Key_Height, Button_Curv_Bord, Back_Color);
		lcd.drawRoundRect(Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + 2*Button_Hori_Distance + Button_Hori_Print - Button_Hori_Distance, Key_Y+Key_Height*3+3*Button_Vert_Distance,
				(Button_Row3_Start + (6*Button_Hori_Print)+ ((Button_Hori_Print - Button_Hori_Distance)/2) - Button_Hori_Distance) - (Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + Button_Hori_Distance + Button_Hori_Print), Key_Height, Button_Curv_Bord, Bord_Color);
}
void Draw_Key_Board_Char(KEYBOARD_TYPE type, KEYBOARD_STATE State, KEYBOARD_LANGUAGE Language, const G_Font *font) {
	KeyFont = font;
	W = KeyFont -> w;
	H = KeyFont -> h;
	uint16_t Char_y = Key_Y+((Key_Height-H)/2);
	if(type == CHARACTER) {
		for(uint16_t j=0; j<10; j++) {
		  lcd.drawChar(Button_Row1_Start + (j*Button_Hori_Print)+((Button_Hori_Print - Button_Hori_Distance - W)/2), Char_y, KeyFont, true, Char_Color, Back_Color, (State == HOA)? Key_Char_HOA[j] : Key_Char_THUONG[j]);
		}
		for(uint16_t j=0; j<9; j++) {
		  lcd.drawChar(Button_Row2_Start + (j*Button_Hori_Print)+((Button_Hori_Print - Button_Hori_Distance - W)/2), Char_y+Button_Vert_Distance+Key_Height, KeyFont, true, Char_Color, Back_Color, (State == HOA)? Key_Char_HOA[j+10] : Key_Char_THUONG[j+10]);
		}
		for(uint16_t j=0; j<7; j++) {
		  lcd.drawChar(Button_Row3_Start + (j*Button_Hori_Print)+((Button_Hori_Print - Button_Hori_Distance - W)/2), Char_y+(Button_Vert_Distance+Key_Height)*2, KeyFont, true, Char_Color, Back_Color, (State == HOA)? Key_Char_HOA[j+10+9] : Key_Char_THUONG[j+10+9]);
		}
		lcd.print(Button_Row1_Start + ((Button_Row3_Start - Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) - 4*W)/2), Key_Y+Key_Height*3+3*Button_Vert_Distance + ((Key_Height - H)/2), KeyFont, true, Char_Color, Back_Color, (char *)"123?");
	}
	else if(type == NUMBER) {
		for(uint16_t j=0; j<10; j++) {
		  lcd.drawChar(Button_Row1_Start + (j*Button_Hori_Print)+((Button_Hori_Print - Button_Hori_Distance - W)/2), Char_y, KeyFont, true, Char_Color, Back_Color, (State == HOA)? Key_Num_HOA[j] : Key_Num_THUONG[j]);
		}
		for(uint16_t j=0; j<9; j++) {
		  lcd.drawChar(Button_Row2_Start + (j*Button_Hori_Print)+((Button_Hori_Print - Button_Hori_Distance - W)/2), Char_y+Button_Vert_Distance+Key_Height, KeyFont, true, Char_Color, Back_Color, (State == HOA)? Key_Num_HOA[j+10] : Key_Num_THUONG[j+10]);
		}
		for(uint16_t j=0; j<7; j++) {
		  lcd.drawChar(Button_Row3_Start + (j*Button_Hori_Print)+((Button_Hori_Print - Button_Hori_Distance - W)/2), Char_y+(Button_Vert_Distance+Key_Height)*2, KeyFont, true, Char_Color, Back_Color, (State == HOA)? Key_Num_HOA[j+10+9] : Key_Num_THUONG[j+10+9]);
		}
		lcd.print(Button_Row1_Start + ((Button_Row3_Start - Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) - 4*W)/2), Key_Y+Key_Height*3+3*Button_Vert_Distance + ((Key_Height - H)/2), KeyFont, true, Char_Color, Back_Color, (char *)"ABcd");
    }

   /* Nút '<-' và '->' */
	lcd.drawBitmap(Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance)/2-Logo_Move_W)/2, Key_Y+Key_Height+Button_Vert_Distance + ((Key_Height - Logo_Move_H)/2), Left_Bitmap, true, Char_Color, Back_Color, Logo_Move_W, Logo_Move_H);
	lcd.drawBitmap(Button_Row2_Start + (9*Button_Hori_Print) + ((Button_Hori_Print - Button_Hori_Distance)/2-Logo_Move_W)/2, Key_Y+Key_Height+Button_Vert_Distance + ((Key_Height - Logo_Move_H)/2), Right_Bitmap, true, Char_Color, Back_Color, Logo_Move_W, Logo_Move_H);
   /* Nút '.' và ',' */
	lcd.drawChar(Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + Button_Hori_Distance + ((Button_Hori_Print - Button_Hori_Distance - W)/2), Key_Y+Key_Height*3+3*Button_Vert_Distance + ((Key_Height - H)/2), KeyFont, true, Char_Color, Back_Color, ',');
	lcd.drawChar(Button_Row3_Start + (6*Button_Hori_Print)+ ((Button_Hori_Print - Button_Hori_Distance)/2) + ((Button_Hori_Print - Button_Hori_Distance - W)/2), Key_Y+Key_Height*3+3*Button_Vert_Distance + ((Key_Height - H)/2), KeyFont, true, Char_Color, Back_Color, '.');
   /* Nút trạng thái phím */
	if(State == HOA) {
		lcd.drawBitmap(Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance - Logo_Capslock_W)/2),  Key_Y+Key_Height*2+2*Button_Vert_Distance + ((Key_Height - Logo_Capslock_H)/2), UnCapslock_Bitmap,  true, Char_Color, Back_Color, Logo_Capslock_W, Logo_Capslock_H);
	} else {
		lcd.drawBitmap(Button_Row1_Start + ((Button_Hori_Print - Button_Hori_Distance - Logo_Capslock_W)/2),  Key_Y+Key_Height*2+2*Button_Vert_Distance + ((Key_Height - Logo_Capslock_H)/2), Capslock_Bitmap,  true, Char_Color, Back_Color, Logo_Capslock_W, Logo_Capslock_H);
	}
	/* Nút Space */
    lcd.print(Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + 2*Button_Hori_Distance + Button_Hori_Print - Button_Hori_Distance + (((Button_Row3_Start + (6*Button_Hori_Print)+ ((Button_Hori_Print - Button_Hori_Distance)/2) - Button_Hori_Distance) - (Button_Row3_Start + ((Button_Hori_Print - Button_Hori_Distance)/2) + Button_Hori_Distance + Button_Hori_Print)- 5*W)/2),
    			Key_Y+Key_Height*3+3*Button_Vert_Distance + ((Key_Height - H)/2), KeyFont, true, Char_Color, Back_Color, (char *)"Space");
    /* Nút Language */
    lcd.print(Button_Row1_Start + (8*Button_Hori_Print) + ((Button_Hori_Print - Button_Hori_Distance- 2*W)/2), Key_Y+Key_Height*2 + 2*Button_Vert_Distance + ((Key_Height - H)/2), KeyFont, true, Char_Color, Back_Color, (char *)"VI");
//
//
//	lcd.drawBitmap(Button_Row3_Start + (7*Button_Hori_Print) + ((lcd.width()-(Button_Row3_Start + (7*Button_Hori_Print)) - Button_Row1_Start - Logo_W)/2),  Key_Y+Key_Height*2+2*Button_Vert_Distance + ((Key_Height - Logo_H)/2), BackSpace_Bitmap, true, Char_Color, Back_Color, Logo_W, Logo_H);
//	lcd.drawBitmap(Button_Row3_Start + (7*Button_Hori_Print) + ((lcd.width()-(Button_Row3_Start + (7*Button_Hori_Print)) - Button_Row1_Start - Logo_W)/2),  Key_Y+Key_Height*3+3*Button_Vert_Distance + ((Key_Height - Logo_H)/2), Enter_Bitmap, true, Char_Color, Back_Color, Logo_W, Logo_H);
}


