/*
 * delay.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: A315-56
 */

#include "delay.h"


static TIM_HandleTypeDef *Timer;

void delay_Init(TIM_HandleTypeDef *Tim_for_delay){
	Timer = Tim_for_delay;
	HAL_TIM_Base_Start(Timer);
}
void delay_us(uint16_t time){
	__HAL_TIM_SET_COUNTER(Timer, 0);
	while ((__HAL_TIM_GET_COUNTER(Timer))<time);
}
void delay_ms(uint16_t time){
	for(uint16_t i=0; i<time; i++)
		delay_us(1000);
}


