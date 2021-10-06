/*
 * Touch.h
 *
 *  Created on: Jul 19, 2021
 *      Author: A315-56
 */

#ifndef INC_TOUCHSCREEN_H_
#define INC_TOUCHSCREEN_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"

class TouchScreen {
	public:
		TouchScreen(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void Init(GPIO_TypeDef *ADC_GPIO, GPIO_TypeDef *DIG_GPIO);
		bool ReadTouch(void);
		void Calibration(float ax, float bx, float ay, float by);
		uint16_t GetX();
		uint16_t GetY();
	private:
		void ADC1_Init(void);
		void INPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool PULL);
		void OUTPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		uint16_t analogRead(uint16_t pin);
		uint16_t row, col;
		float Ax , Bx , Ay , By;
};

#endif /* INC_TOUCHSCREEN_H_ */
