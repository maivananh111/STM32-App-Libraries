/*
 * SD.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: A315-56
 */
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "SD.h"
#include "delay.h"


extern TFT_HandleTypeDef lcd;

static char buffer[BUFFER_SIZE];
static FATFS    SD;
static FRESULT  stt;

static void clear_buffer(void);
static void clear_buffer(void){
	for (int i=0; i<BUFFER_SIZE; i++) buffer[i] = '\0';
}
void SD_Connect(void){
	  delay_ms(500);
	  stt = f_mount(&SD, "/", 1);
	  switch(stt){
		  case FR_OK:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
			  lcd.print(102, 149, &Font_Terminal11x18, false, GREEN, BLACK, (char *)"SD connected succesfully!");
			  delay_ms(3000);
		  break;
		  case FR_DISK_ERR:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			  lcd.print(141, 149, &Font_Terminal11x18, false, RED, BLACK, (char *)"SD connected fail!");
			  while(stt == FR_DISK_ERR){stt = f_mount(&SD, "/", 1);}
			  delay_ms(3000);
		  break;
		  case FR_NOT_READY:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			  lcd.print(141, 149, &Font_Terminal11x18, false, RED, BLACK, (char *)"Not find SD Card!");
			  while(stt == FR_NOT_READY){stt = f_mount(&SD, "/", 1);}
			  delay_ms(3000);
		  break;
		  case FR_NO_FILESYSTEM:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			  lcd.print(141, 149, &Font_Terminal11x18, false, RED, BLACK, (char *)"Not find File System!");
			  while(stt == FR_NO_FILESYSTEM){stt = f_mount(&SD, "/", 1);}
			  delay_ms(3000);
		  break;
		  default:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			  lcd.print(190, 149, &Font_Terminal11x18, false, RED, BLACK, (char *)"SD ERROR!");
			  while(stt == FR_NO_FILESYSTEM){stt = f_mount(&SD, "/", 1);}
		  break;
	  }
	  lcd.fillScreen(BLACK);
}
void SD_Infor(void){
	FATFS *pfs;
	DWORD fre_clust;
	uint32_t total, free_space;
	    lcd.fillScreen(BLACK);
	  	f_getfree("", &fre_clust, &pfs);
	  	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	  	sprintf (buffer, "SD CARD Total Size: %lu",total);
	  	lcd.print(5, 20, &Font_Terminal8x12, false, YELLOW, BLACK, (char *)buffer);
	  	clear_buffer();
	  	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	  	sprintf (buffer, "SD CARD Free Space: %lu",free_space);
	  	lcd.print(5, 40, &Font_Terminal8x12, false, BLUE, BLACK, (char *)buffer);
	  	clear_buffer();
	  	delay_ms(1000);

}
FRESULT SD_Scanfiles (char* path){
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;
    char str[30];
    uint8_t y = 1;
    sprintf(str, "%s/...", path);
    lcd.print(5, 10, &Font_Terminal8x12, false, YELLOW, BLACK, str);
    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
//                i = strlen(path);
//                sprintf(&path[i], "/%s", fno.fname);
//                res = SD_Scanfiles(path);                    /* Enter the directory */
//                if (res != FR_OK) break;
//                path[i] = 0;
//            } else {                                       /* It is a file. */
            	strlwr(fno.fname);
                strcpy(str, fno.fname);
                lcd.print(5, 10+15*y, &Font_Terminal8x12, false, YELLOW, BLACK, (char *)str);
                y = y+1;
//        }
        }
        f_closedir(&dir);
    }
    return res;
}
char *SD_ReadFile(char *path, uint8_t Num_Of_File){
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;
    uint8_t num;
    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (num = 0; num<Num_Of_File; num++) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = SD_Scanfiles(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            }                                     /* It is a file. */
        }
        strlwr(fno.fname);
        f_closedir(&dir);
    }
    return fno.fname;
}
