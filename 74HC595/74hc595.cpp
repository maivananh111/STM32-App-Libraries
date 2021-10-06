/*
 * 74hc595.cpp
 *
 *  Created on: Aug 21, 2021
 *      Author: A315-56
 */

#include "74hc595.h"

Hc595::Hc595(uint16_t Clock, uint16_t Latch, uint16_t Data) {
	_Clock = Clock;
	_Latch = Latch;
	_Data  = Data;
}
void Hc595::Init(GPIO_TypeDef *GPIO_Clock, GPIO_TypeDef *GPIO_Latch, GPIO_TypeDef *GPIO_Data) {
	PORT_Clock = GPIO_Clock;
	PORT_Latch = GPIO_Latch;
	PORT_Data  = GPIO_Data;


	PORT_Clock -> MODER   |=  (1<<(_Clock*2));
	PORT_Clock -> OTYPER  &=~ (1<<_Clock);
	PORT_Clock -> OSPEEDR |=  (1<<(_Clock*2))|(1<<(_Clock*2 + 1));
	PORT_Clock -> PUPDR   &=~ (1<<(_Clock*2))|(1<<(_Clock*2 + 1));

	PORT_Latch -> MODER   |=  (1<<(_Latch*2));
	PORT_Latch -> OTYPER  &=~ (1<<_Latch);
	PORT_Latch -> OSPEEDR |=  (1<<(_Latch*2))|(1<<(_Latch*2 + 1));
	PORT_Latch -> PUPDR   &=~ (1<<(_Latch*2))|(1<<(_Latch*2 + 1));

	PORT_Data  -> MODER   |=  (1<<(_Data*2));
	PORT_Data  -> OTYPER  &=~ (1<<_Data);
	PORT_Data  -> OSPEEDR |=  (1<<(_Data*2))|(1<<(_Data*2 + 1));
	PORT_Data  -> PUPDR   &=~ (1<<(_Data*2))|(1<<(_Data*2 + 1));
}

void Hc595::GPIO_LOW (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	GPIOx -> BSRR |= ((1<<GPIO_Pin)<<16);
}
void Hc595::GPIO_HIGH (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	GPIOx -> BSRR |= (1<<GPIO_Pin);
}

void Hc595::shift(bool dir, uint8_t data) {
	uint8_t i, check;
	if(dir == MSB){
		for(i=0; i<8; i++){
		   check = (data & (0x80>>i));
		   if(check) GPIO_HIGH(PORT_Data, _Data);
		   else GPIO_LOW(PORT_Data, _Data);
		   GPIO_LOW(PORT_Clock, _Clock);
		   GPIO_HIGH(PORT_Clock, _Clock);
	   }
	}
	else if(dir == LSB){
	   for(i=0; i<8; i++){
		   check = (data & (0x01<<i));
		   if(check) GPIO_HIGH(PORT_Data, _Data);
		   else GPIO_LOW(PORT_Data, _Data);
		   GPIO_LOW(PORT_Clock, _Clock);
		   GPIO_HIGH(PORT_Clock, _Clock);
	   }
	}
}

void Hc595::shift_8(bool dir, uint8_t data) {
	GPIO_LOW(PORT_Latch, _Latch);
	 shift(dir, data);
	GPIO_HIGH(PORT_Latch, _Latch);
}

void Hc595::shift_s(bool dir, uint8_t *data) {
	GPIO_LOW(PORT_Latch, _Latch);
	 while(*data) {
		 shift(dir, *data++);
	 }
	GPIO_HIGH(PORT_Latch, _Latch);
}
