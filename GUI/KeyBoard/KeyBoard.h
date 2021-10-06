/*
 * KeyBoard.h
 *
 *  Created on: Sep 24, 2021
 *      Author: A315-56
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_


#include "stdio.h"
#include "TFT_8Bit.h"

extern TFT_HandleTypeDef lcd;

#define Button_Hori_Distance 4 ///// Khoảng cách bề ngang giữa các phím
#define Button_Vert_Distance 3 ///// Khoảng cách bề dọcngang giữa các phím
#define Button_Hori_Print    47///// Khoảng cách vị trí bắt đầu giữa các nút
#define Button_Curv_Bord     7 ///// Bán kính đường bo cong
#define Button_Row1_Start    7
#define Button_Row2_Start    Button_Row1_Start + (Button_Hori_Print)/2
#define Button_Row3_Start    Button_Row1_Start + Button_Hori_Print
#define Char_Color           BLACK
#define Back_Color           WHITE
#define Bord_Color           BLACK

#define Logo_Capslock_W 21
#define Logo_Capslock_H 21

#define Logo_Move_W 12
#define Logo_Move_H 21

typedef enum {
	CHARACTER,
	NUMBER
} KEYBOARD_TYPE;
typedef enum {
	HOA,
	THUONG
} KEYBOARD_STATE;
typedef enum {
	VI,
	EN
} KEYBOARD_LANGUAGE;

void Draw_Key_Board_Button(uint16_t Key_location, uint8_t Height_of_Key);
void Draw_Key_Board_Char(KEYBOARD_TYPE type, KEYBOARD_STATE State, KEYBOARD_LANGUAGE Language, const G_Font *font);

#endif /* KEYBOARD_H_ */
