/*
 * 74hc595.h
 *
 *  Created on: Aug 21, 2021
 *      Author: A315-56
 */

#ifndef INC_74HC595_H_
#define INC_74HC595_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"


#define MSB true
#define LSB false

class Hc595 {
	public:
		Hc595(uint16_t Clock, uint16_t Latch, uint16_t Data);
		void Init(GPIO_TypeDef *GPIO_Clock, GPIO_TypeDef *GPIO_Latch, GPIO_TypeDef *GPIO_Data);
		void shift_8(bool dir, uint8_t data);
		void shift_s(bool dir, uint8_t *data);
	private:
		void shift(bool dir, uint8_t data);
		void GPIO_LOW (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		void GPIO_HIGH (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		GPIO_TypeDef *PORT_Clock, *PORT_Latch, *PORT_Data;
		uint16_t _Clock, _Latch, _Data;
};



#endif /* INC_74HC595_H_ */
