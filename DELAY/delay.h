/*
 * delay.h
 *
 *  Created on: Sep 24, 2021
 *      Author: A315-56
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx_hal.h"
#include "stdio.h"

void delay_Init(TIM_HandleTypeDef *Tim_for_delay);
void delay_us(uint16_t time);
void delay_ms(uint16_t time);

#endif /* DELAY_H_ */
