/*
 * W25Qxx.cpp
 *
 *  Created on: 2 thg 9, 2021
 *      Author: A315-56
 */

#include "W25Qxx.h"

w25qxx_t w25qxx;

W25Q::W25Q(SPI_HandleTypeDef *MySPI, GPIO_TypeDef *CS_GPIO, uint16_t CS_Pin) {
    SPI = MySPI;
    GPIOx = CS_GPIO;
    CS_pin = CS_Pin;
}

void W25Q::CS_Active(void) {
	HAL_GPIO_WritePin(GPIOx, CS_pin, GPIO_PIN_RESET);
}

void W25Q::CS_Idle(void) {
	HAL_GPIO_WritePin(GPIOx, CS_pin, GPIO_PIN_SET);
}

uint8_t W25Q::SPI_TR(uint8_t data) {
	uint8_t	buf;
	HAL_SPI_TransmitReceive(SPI, &data, &buf, 1, 100);
//	HAL_SPI_TransmitReceive_DMA(SPI, &data, &buf, 1);
	return buf;
}

uint8_t W25Q::ReadStatusREG(uint8_t StatusRegisterNum) {
	uint8_t	status=0;
    CS_Active();
      switch(StatusRegisterNum){
        case 1:
			SPI_TR(0x05);
			status = SPI_TR(dummy);
			w25qxx.StatusRegister1 = status;
		break;

        case 2:
        	SPI_TR(0x35);
			status = SPI_TR(dummy);
			w25qxx.StatusRegister2 = status;
		break;

        case 3:
        	SPI_TR(0x15);
			status = SPI_TR(dummy);
			w25qxx.StatusRegister3 = status;
		break;
		}
	 CS_Idle();
     return status;
}

void W25Q::WriteEnable(void) {
	CS_Active();
     SPI_TR(0x06);
    CS_Idle();
	HAL_Delay(1);
}

void W25Q::WriteDisable(void) {
	CS_Active();
     SPI_TR(0x04);
    CS_Idle();
	HAL_Delay(1);
}

void W25Q::WaitWriteEnd(void) {
	HAL_Delay(1);
	CS_Active();
	SPI_TR(0x05);
    do {
      w25qxx.StatusRegister1 = SPI_TR(dummy);
	  HAL_Delay(1);
    } while ((w25qxx.StatusRegister1 & 0x01) == 0x01);
    CS_Idle();
}

uint32_t W25Q::ReadID(void) {
    uint32_t ID = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
    HAL_GPIO_WritePin(GPIOx, CS_pin, GPIO_PIN_SET);
     HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOx, CS_pin, GPIO_PIN_RESET);
     SPI_TR(0x9F);
      Temp0 = SPI_TR(dummy);
      Temp1 = SPI_TR(dummy);
      Temp2 = SPI_TR(dummy);
    HAL_GPIO_WritePin(GPIOx, CS_pin, GPIO_PIN_SET);
    ID = (Temp0<<16)|(Temp1<<8)|Temp2;
    return ID;
}

uint16_t W25Q::Init(void){
	uint32_t id = ReadID();
	switch(id & 0x000000FF)
	  {
			case 0x20:	// 	w25q512
				w25qxx.Code = 512;
				w25qxx.BlockCount = 1024;
			break;
			case 0x19:	// 	w25q256
				w25qxx.Code = 256;
				w25qxx.BlockCount = 512;
			break;
			case 0x18:	// 	w25q128
				w25qxx.Code = 128;
				w25qxx.BlockCount = 256;
			break;
			case 0x17:	//	w25q64
				w25qxx.Code = 64;
				w25qxx.BlockCount = 128;
			break;
			case 0x16:	//	w25q32
				w25qxx.Code = 32;
				w25qxx.BlockCount = 64;
			break;
			case 0x15:	//	w25q16
				w25qxx.Code = 16;
				w25qxx.BlockCount = 32;
			break;
			case 0x14:	//	w25q80
				w25qxx.Code = 80;
				w25qxx.BlockCount = 16;
			break;
			case 0x13:	//	w25q40
				w25qxx.Code = 40;
				w25qxx.BlockCount = 8;
			break;
			case 0x12:	//	w25q20
				w25qxx.Code = 20;
				w25qxx.BlockCount = 4;
			break;
			case 0x11:	//	w25q10
				w25qxx.Code = 10;
				w25qxx.BlockCount = 2;
			break;
			default:
				w25qxx.Code = 0;
			return 0;

		}
		w25qxx.PageSize=256;
		w25qxx.SectorSize=0x1000;
		w25qxx.SectorCount=w25qxx.BlockCount*16;
		w25qxx.PageCount=(w25qxx.SectorCount*w25qxx.SectorSize)/w25qxx.PageSize;
		w25qxx.BlockSize=w25qxx.SectorSize*16;
		w25qxx.CapacityInKiloByte=(w25qxx.SectorCount*w25qxx.SectorSize)/1024;
		ReadStatusREG(1);
		ReadStatusREG(2);
		ReadStatusREG(3);
		return id;
}

void W25Q::EraseSector(uint32_t SectorAddr){
	WaitWriteEnd();
	SectorAddr = SectorAddr * w25qxx.SectorSize;
    WriteEnable();
    CS_Active();
    SPI_TR(0x20);
	if(w25qxx.Code >= 256) SPI_TR((SectorAddr & 0xFF000000) >> 24);
	    SPI_TR((SectorAddr & 0xFF0000) >> 16);
	    SPI_TR((SectorAddr & 0xFF00) >> 8);
	    SPI_TR(SectorAddr & 0xFF);
	CS_Idle();
    WaitWriteEnd();
	HAL_Delay(1);
}

void W25Q::EraseBlock(uint32_t BlockAddr) {
	WaitWriteEnd();
	BlockAddr = BlockAddr * w25qxx.SectorSize * 16;
    WriteEnable();
    CS_Active();
    SPI_TR(0xD8);
	if(w25qxx.Code >= 256) SPI_TR((BlockAddr & 0xFF000000) >> 24);
		SPI_TR((BlockAddr & 0xFF0000) >> 16);
		SPI_TR((BlockAddr & 0xFF00) >> 8);
		SPI_TR(BlockAddr & 0xFF);
	CS_Idle();
   WaitWriteEnd();
	HAL_Delay(1);
}

void W25Q::EraseChip(void){
	WriteEnable();
	CS_Active();
     SPI_TR(0xC7);
    CS_Idle();
	WaitWriteEnd();
	HAL_Delay(10);
}

uint32_t W25Q::SectorToPage(uint32_t SectorAddress) {
	return (SectorAddress * w25qxx.SectorSize) / w25qxx.PageSize;
}
uint32_t W25Q::BlockToPage(uint32_t	BlockAddress){
	return (BlockAddress * w25qxx.BlockSize) / w25qxx.PageSize;
}

void W25Q::ReadByte(uint8_t *Data,uint32_t BytesAddress) {
	CS_Active();
    SPI_TR(0x0B);
	if(w25qxx.Code >= 256) SPI_TR((BytesAddress & 0xFF000000) >> 24);
		SPI_TR((BytesAddress & 0xFF0000) >> 16);
		SPI_TR((BytesAddress& 0xFF00) >> 8);
		SPI_TR(BytesAddress & 0xFF);
		SPI_TR(0);
	*Data = SPI_TR(dummy);
	CS_Idle();
}

void W25Q::ReadBytes(uint8_t *Data, uint32_t ReadAddr, uint32_t NumByteRead) {
	CS_Active();
	SPI_TR(0x0B);
	if(w25qxx.Code >= 256) SPI_TR((ReadAddr & 0xFF000000) >> 24);
		SPI_TR((ReadAddr & 0xFF0000) >> 16);
		SPI_TR((ReadAddr& 0xFF00) >> 8);
	    SPI_TR(ReadAddr & 0xFF);
	    SPI_TR(0);
	HAL_SPI_Receive(SPI, Data, NumByteRead, 2000);
	CS_Idle();
//	HAL_Delay(1);
}

void W25Q::ReadPage(uint8_t *Data, uint32_t PageAddress, uint32_t OffsetByte, uint32_t NumByteRead) {
	if((NumByteRead > w25qxx.PageSize)||(NumByteRead == 0)) NumByteRead = w25qxx.PageSize;
	if((OffsetByte + NumByteRead) > w25qxx.PageSize)
		NumByteRead = w25qxx.PageSize - OffsetByte;
	PageAddress = PageAddress * w25qxx.PageSize + OffsetByte;
	CS_Active();
	SPI_TR(0x0B);
	if(w25qxx.Code >= 256) SPI_TR((PageAddress & 0xFF000000) >> 24);
		SPI_TR((PageAddress & 0xFF0000) >> 16);
		SPI_TR((PageAddress& 0xFF00) >> 8);
		SPI_TR( PageAddress & 0xFF );
	    SPI_TR(0);
	HAL_SPI_Receive(SPI, Data, NumByteRead, 100);
	CS_Idle();
//	HAL_Delay(1);
}

void W25Q::ReadSector(uint8_t *Data, uint32_t SectorAddress, uint32_t OffsetByte, uint32_t NumByteRead){
	if((NumByteRead > w25qxx.SectorSize)||(NumByteRead == 0)) NumByteRead = w25qxx.SectorSize;
	if(OffsetByte >= w25qxx.SectorSize) return;
	uint32_t	StartPage;
	int32_t		BytesToRead;
	uint32_t	LocalOffset;
	if((OffsetByte + NumByteRead) > w25qxx.SectorSize) BytesToRead = w25qxx.SectorSize - OffsetByte;
	else BytesToRead = NumByteRead;
	StartPage = SectorToPage(SectorAddress) + (OffsetByte / w25qxx.PageSize);
	LocalOffset = OffsetByte % w25qxx.PageSize;
	do {
		ReadPage(Data, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead -= w25qxx.PageSize-LocalOffset;
		Data += w25qxx.PageSize - LocalOffset;
		LocalOffset=0;
	} while(BytesToRead > 0);
}

void W25Q::ReadBlock(uint8_t* Data, uint32_t BlockAddress, uint32_t OffsetByte, uint32_t NumByteRead) {
	if((NumByteRead > w25qxx.BlockSize)||(NumByteRead == 0)) NumByteRead = w25qxx.BlockSize;
	if(OffsetByte >= w25qxx.BlockSize) return;
	uint32_t	StartPage;
	int32_t		BytesToRead;
	uint32_t	LocalOffset;
	if((OffsetByte + NumByteRead) > w25qxx.BlockSize) BytesToRead = w25qxx.BlockSize - OffsetByte;
	else BytesToRead = NumByteRead;
	StartPage = BlockToPage(BlockAddress)+(OffsetByte / w25qxx.PageSize);
	LocalOffset = OffsetByte % w25qxx.PageSize;
	do {
		ReadPage(Data, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead -= w25qxx.PageSize-LocalOffset;
		Data += w25qxx.PageSize - LocalOffset;
		LocalOffset=0;
	}while (BytesToRead > 0);
}

void W25Q::WriteByte(uint8_t Data, uint32_t WriteAddr) {
    WaitWriteEnd();
    WriteEnable();
    CS_Active();
    SPI_TR(0x02);
	if(w25qxx.Code >= 256) SPI_TR((WriteAddr & 0xFF000000) >> 24);
		SPI_TR((WriteAddr & 0xFF0000) >> 16);
		SPI_TR((WriteAddr & 0xFF00) >> 8);
		SPI_TR(WriteAddr & 0xFF);
		SPI_TR(Data);
	CS_Idle();
    WaitWriteEnd();
}

void W25Q::WriteBytes(uint8_t *Data, uint32_t WriteAddr, uint32_t NumByteWrite) {
    WaitWriteEnd();
    WriteEnable();
    CS_Active();
    SPI_TR(0x02);
	if(w25qxx.Code >= 256) SPI_TR((WriteAddr & 0xFF000000) >> 24);
		SPI_TR((WriteAddr & 0xFF0000) >> 16);
		SPI_TR((WriteAddr & 0xFF00) >> 8);
		SPI_TR(WriteAddr & 0xFF);
		HAL_SPI_Transmit(SPI, Data, NumByteWrite ,100);
	CS_Idle();
    WaitWriteEnd();
}

void W25Q::WritePage(uint8_t *Data	,uint32_t PageAddress, uint32_t OffsetByte, uint32_t NumByteWrite) {
	if(((NumByteWrite + OffsetByte) > w25qxx.PageSize)||(NumByteWrite == 0)) NumByteWrite = w25qxx.PageSize - OffsetByte;
	if((OffsetByte + NumByteWrite) > w25qxx.PageSize) NumByteWrite = w25qxx.PageSize - OffsetByte;
	WaitWriteEnd();
    WriteEnable();
    CS_Active();
    SPI_TR(0x02);
	PageAddress = (PageAddress * w25qxx.PageSize) + OffsetByte;
	if(w25qxx.Code >= 256) SPI_TR((PageAddress & 0xFF000000) >> 24);
		SPI_TR((PageAddress & 0xFF0000) >> 16);
		SPI_TR((PageAddress & 0xFF00) >> 8);
		SPI_TR(PageAddress&0xFF);
	HAL_SPI_Transmit(SPI, Data, NumByteWrite ,100);
	CS_Idle();
    WaitWriteEnd();
	HAL_Delay(1);
}

void W25Q::WriteSector(uint8_t *Data ,uint32_t SectorAddress, uint32_t OffsetByte ,uint32_t NumByteWrite) {
	if((NumByteWrite > w25qxx.SectorSize)||(NumByteWrite == 0)) NumByteWrite = w25qxx.SectorSize;
	if(OffsetByte >= w25qxx.SectorSize) return;
	uint32_t	StartPage;
	int32_t		BytesToWrite;
	uint32_t	LocalOffset;
	if((OffsetByte + NumByteWrite) > w25qxx.SectorSize) BytesToWrite = w25qxx.SectorSize - OffsetByte;
	else BytesToWrite = NumByteWrite;
	StartPage = SectorToPage(SectorAddress) + (OffsetByte/w25qxx.PageSize);
	LocalOffset = OffsetByte % w25qxx.PageSize;
	do {
		WritePage(Data, StartPage, LocalOffset, BytesToWrite);
		StartPage++;
		BytesToWrite -= w25qxx.PageSize - LocalOffset;
		Data += w25qxx.PageSize - LocalOffset;
		LocalOffset=0;
	} while(BytesToWrite>0);
}

void W25Q::WriteBlock(uint8_t *Data ,uint32_t BlockAddress, uint32_t OffsetByte, uint32_t NumByteWrite) {
	if((NumByteWrite > w25qxx.BlockSize)||(NumByteWrite == 0)) NumByteWrite = w25qxx.BlockSize;
	if(OffsetByte >= w25qxx.BlockSize) return;
	uint32_t	StartPage;
	int32_t		BytesToWrite;
	uint32_t	LocalOffset;
	if((OffsetByte + NumByteWrite) > w25qxx.BlockSize) BytesToWrite = w25qxx.BlockSize - OffsetByte;
	else BytesToWrite = NumByteWrite;
	StartPage = BlockToPage(BlockAddress) + (OffsetByte / w25qxx.PageSize);
	LocalOffset = OffsetByte % w25qxx.PageSize;
	do {
		WritePage(Data, StartPage, LocalOffset, BytesToWrite);
		StartPage++;
		BytesToWrite -= w25qxx.PageSize - LocalOffset;
		Data += w25qxx.PageSize - LocalOffset;
		LocalOffset=0;
	} while(BytesToWrite>0);
}
