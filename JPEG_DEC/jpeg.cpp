/*
 * jpeg.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: A315-56
 */

#include "stm32f4xx_hal.h"
#include "stdio.h"
#include <cstdlib>
#include "string.h"
#include "jpeg.h"
#include "SD.h"
#include "TFT_8Bit.h"


extern TFT_HandleTypeDef lcd;

FATFS SDC;
struct jpeg_decompress_struct cinfo;
typedef struct RGB{
  uint8_t B;
  uint8_t G;
  uint8_t R;
}RGB_typedef;
struct jpeg_error_mgr jerr;
RGB_typedef *RGB_matrix;
static uint16_t RGB16PixelColor;

void jpeg_decode(char *path, uint16_t posx, uint16_t posy, uint16_t w, uint16_t h){
//	f_mount(&SDC, "/", 1);
	  FIL file;
	  uint32_t line_counter = 0;
	  uint32_t i = 0, xc=0, ratio;
	  uint8_t offset=1;
	  JSAMPROW buffer[2] = {0};
	  UINT lcdWidth, lcdHeight;
	  uint8_t *rowBuff = (uint8_t *)malloc(2048);
	  buffer[0] = rowBuff;
	  lcdWidth = w;
	  lcdHeight = h;
      if(f_open(&file, path, FA_READ) == FR_OK){
		  cinfo.err = jpeg_std_error(&jerr);
		  jpeg_create_decompress(&cinfo);
		  jpeg_stdio_src (&cinfo, &file);
		  jpeg_read_header(&cinfo, TRUE);
		  if (cinfo.image_width > lcdWidth){
			  ratio=cinfo.image_width/lcdWidth;
			  cinfo.scale_num = 1;
			  if (ratio <= 8) {
				  cinfo.scale_denom = 1;
				  for(int s = 0x8; s > 0x01; s /=2){
					  if (ratio & s){
						  cinfo.scale_denom=s;
						  break;
					  }
				  }
			  }
			  else{cinfo.scale_denom = 8;}
			}
		    cinfo.dct_method = JDCT_IFAST;
			jpeg_start_decompress(&cinfo);
			if (cinfo.output_width > lcdWidth){
				offset = cinfo.output_width / lcdWidth;
				if (cinfo.output_width % lcdWidth > lcdWidth/4) offset++;
			}
//			*iw = cinfo.image_width;
//			*ih = cinfo.image_height;
			lcd.setAddrWindow(posx, posy, w-1, h-1);
			lcd.Enable();
			lcd.WriteEnable();
			while (cinfo.output_scanline < cinfo.output_height && line_counter < lcdHeight-posy){
			  (void) jpeg_read_scanlines(&cinfo, buffer, 1);
			  RGB_matrix=(RGB_typedef*)buffer[0];
			  for(i = 0, xc=0; i < cinfo.output_width && xc < (lcdWidth -posx); i+=offset, xc++){
				RGB16PixelColor = (uint16_t)(
				 ((RGB_matrix[i].R & 0x00F8) >> 3)|
				 ((RGB_matrix[i].G & 0x00FC) << 3)|
				 ((RGB_matrix[i].B & 0x00F8) << 8)
				); lcd.DataWrite(RGB16PixelColor);
			  }
			  for(i=0; i < offset-1 && cinfo.output_scanline < cinfo.output_height; i++)
				  (void) jpeg_read_scanlines(&cinfo, buffer, 1);
			  line_counter++;
			}
		  jpeg_finish_decompress(&cinfo);
		  jpeg_destroy_decompress(&cinfo);
		  lcd.Disable();
		  lcd.setAddrWindow(0, 0, lcd.width(), lcd.height());
		  f_close(&file);
		  free(rowBuff);
	  }else{
		  lcd.Viet_print(141, 149, &Font_Viet_Terminal8x22, true, GREEN, BLACK, (char *)"File Lỗi!");
	  }
}
