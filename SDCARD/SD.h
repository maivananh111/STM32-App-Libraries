/*
 * SD.h
 *
 *  Created on: Sep 24, 2021
 *      Author: A315-56
 */

#ifndef SD_H_
#define SD_H_

#include "fatfs.h"
#include "TFT_8Bit.h"

#define BUFFER_SIZE 30

void SD_Connect(void);
void SD_Infor(void);
FRESULT SD_Scanfiles (char* path);
char *SD_ReadFile(char *path, uint8_t Num_Of_File);


#endif /* SD_H_ */
