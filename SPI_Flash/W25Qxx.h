/*
 * W25Qxx.h
 *
 *  Created on: 2 thg 9, 2021
 *      Author: A315-56
 */

#ifndef INC_W25QXX_H_
#define INC_W25QXX_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"

typedef struct {
	uint16_t    Code;
	uint16_t	PageSize;
	uint32_t	PageCount;
	uint32_t	SectorSize;
	uint32_t	SectorCount;
	uint32_t	BlockSize;
	uint32_t	BlockCount;
	uint32_t	CapacityInKiloByte;
	uint8_t		StatusRegister1;
	uint8_t		StatusRegister2;
	uint8_t		StatusRegister3;
}w25qxx_t;
extern w25qxx_t w25qxx;

class W25Q {
	public:
	  W25Q(SPI_HandleTypeDef *MySPI, GPIO_TypeDef *CS_GPIO, uint16_t CS_Pin);
	  uint32_t ReadID(void);
	  uint16_t Init(void);

	  void EraseSector(uint32_t SectorAddr);
	  void EraseBlock(uint32_t BlockAddr);
	  void EraseChip(void);

	  void ReadByte (uint8_t *Data, uint32_t BytesAddress);
	  void ReadBytes(uint8_t *Data, uint32_t ReadAddr, uint32_t NumByteRead);
      void ReadPage (uint8_t *Data, uint32_t PageAddress, uint32_t OffsetByte, uint32_t NumByteRead);
      void ReadSector(uint8_t *Data, uint32_t SectorAddress, uint32_t OffsetByte, uint32_t NumByteRead);
      void ReadBlock(uint8_t *Data, uint32_t BlockAddress, uint32_t OffsetByte, uint32_t NumByteRead);

      void WriteByte(uint8_t Data, uint32_t WriteAddr);
      void WriteBytes(uint8_t *Data, uint32_t WriteAddr, uint32_t NumByteWrite);
      void WritePage(uint8_t *Data	,uint32_t PageAddress, uint32_t OffsetByte, uint32_t NumByteWrite);
      void WriteSector(uint8_t *Data ,uint32_t SectorAddress, uint32_t OffsetByte ,uint32_t NumByteWrite);
      void WriteBlock(uint8_t *Data ,uint32_t BlockAddress, uint32_t OffsetByte, uint32_t NumByteWrite);
	private:
      SPI_HandleTypeDef *SPI;
      GPIO_TypeDef *GPIOx;
      uint16_t CS_pin;
      uint8_t dummy = 0xA5;

      uint8_t SPI_TR(uint8_t data);
      void CS_Active(void);
      void CS_Idle(void);
      void WriteEnable(void);
      void WriteDisable(void);
      void WaitWriteEnd(void);
      uint8_t  ReadStatusREG(uint8_t StatusRegisterNum);
      uint32_t BlockToPage(uint32_t	BlockAddress);
      uint32_t SectorToPage(uint32_t SectorAddress);
};



#endif /* INC_W25QXX_H_ */
