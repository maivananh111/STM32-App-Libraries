/*
 * JoyStick.h
 *
 *  Created on: 13 thg 8, 2021
 *      Author: A315-56
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"

#define No_Click   0
#define One_Click  1
#define Long_Click 2

class JoyStick {
	public:
		JoyStick(uint16_t PoiX_pin, uint16_t PoiY_pin, uint16_t Button_pin);
		void     Init(GPIO_TypeDef *Joy_GPIO, GPIO_TypeDef *Button_GPIO);
		void     Read(void);
		uint16_t GetX(void);
		uint16_t GetY(void);
		uint8_t  Button_State(uint8_t (*One_Click_CallBack)(void), uint8_t (*Long_Click_CallBack)(void));
		int16_t     Y_Tick(int16_t bottom, int16_t top);
		int16_t     X_Tick(int16_t bottom, int16_t top);
//		bool     Tick(void);
		uint16_t X_Balance = 1950, Y_Balance = 2010;
	private:
		GPIO_TypeDef *ADC_P, *BUT_P;
	    void     INPUT(GPIO_TypeDef *GPIOx, uint16_t pin);
		void     ADC_Init(void);
		void    DMA2_Init(void);
//		uint16_t analogRead(uint16_t pin);
		uint16_t adc[2];
		uint16_t X_Pin, Y_Pin, BT_Pin;
		uint16_t ADCX, ADCY;
        uint16_t BT_Counter = 0;
        uint16_t Time_Check = 700;
        int16_t   CounterX = 0;
        int16_t   CounterY = 0;
};



#endif /* INC_JOYSTICK_H_ */
