#ifndef __FONT_H
#define __FONT_H

#include <stdio.h>
#include "stdint.h"
#define swap(a, b) {uint16_t t = a; a = b; b = t; }

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

typedef struct {
	uint8_t  *bitmap;     ///< Pointer into GFXfont->bitmap
	uint8_t  w;            ///< Bitmap dimensions in pixels
    uint8_t  h;           ///< Bitmap dimensions in pixels
	uint8_t  byte;         ///< Distance to advance cursor (x axis)
} G_Font;
extern const G_Font Font_Gothic9x16;
extern const G_Font Font_Terminal8x12;
extern const G_Font Font_Terminal11x18;
extern const G_Font Font_Terminal18x24;
extern const G_Font Font_Terminal25x32;
#endif
