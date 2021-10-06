/*
 * JoyStick.cpp
 *
 *  Created on: 13 thg 8, 2021
 *      Author: A315-56
 */
#include "JoyStick.h"


extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

JoyStick::JoyStick(uint16_t PoiX_pin, uint16_t PoiY_pin, uint16_t Button_pin){
	X_Pin = PoiX_pin;
	Y_Pin = PoiY_pin;
	BT_Pin = Button_pin;
	switch(X_Pin){
	      case GPIO_PIN_0: ADCX = ADC_CHANNEL_0; break;
	      case GPIO_PIN_1: ADCX = ADC_CHANNEL_1; break;
	      case GPIO_PIN_2: ADCX = ADC_CHANNEL_2; break;
	      case GPIO_PIN_3: ADCX = ADC_CHANNEL_3; break;
	      case GPIO_PIN_4: ADCX = ADC_CHANNEL_4; break;
	      case GPIO_PIN_5: ADCX = ADC_CHANNEL_5; break;
	      case GPIO_PIN_6: ADCX = ADC_CHANNEL_6; break;
	      case GPIO_PIN_7: ADCX = ADC_CHANNEL_7; break;
	      case GPIO_PIN_8: ADCX = ADC_CHANNEL_8; break;
	      case GPIO_PIN_9: ADCX = ADC_CHANNEL_9; break;
	      case GPIO_PIN_10: ADCX = ADC_CHANNEL_10; break;
	      case GPIO_PIN_11: ADCX = ADC_CHANNEL_11; break;
	      case GPIO_PIN_12: ADCX = ADC_CHANNEL_12; break;
	      case GPIO_PIN_13: ADCX = ADC_CHANNEL_13; break;
	      case GPIO_PIN_14: ADCX = ADC_CHANNEL_14; break;
	      case GPIO_PIN_15: ADCX = ADC_CHANNEL_15; break;
	}
	switch(Y_Pin){
	      case GPIO_PIN_0: ADCY = ADC_CHANNEL_0; break;
	      case GPIO_PIN_1: ADCY = ADC_CHANNEL_1; break;
	      case GPIO_PIN_2: ADCY = ADC_CHANNEL_2; break;
	      case GPIO_PIN_3: ADCY = ADC_CHANNEL_3; break;
	      case GPIO_PIN_4: ADCY = ADC_CHANNEL_4; break;
	      case GPIO_PIN_5: ADCY = ADC_CHANNEL_5; break;
	      case GPIO_PIN_6: ADCY = ADC_CHANNEL_6; break;
	      case GPIO_PIN_7: ADCY = ADC_CHANNEL_7; break;
	      case GPIO_PIN_8: ADCY = ADC_CHANNEL_8; break;
	      case GPIO_PIN_9: ADCY = ADC_CHANNEL_9; break;
	      case GPIO_PIN_10: ADCY = ADC_CHANNEL_10; break;
	      case GPIO_PIN_11: ADCY = ADC_CHANNEL_11; break;
	      case GPIO_PIN_12: ADCY = ADC_CHANNEL_12; break;
	      case GPIO_PIN_13: ADCX = ADC_CHANNEL_13; break;
	      case GPIO_PIN_14: ADCY = ADC_CHANNEL_14; break;
	      case GPIO_PIN_15: ADCY = ADC_CHANNEL_15; break;
	    }
}
void JoyStick::INPUT (GPIO_TypeDef *GPIOx, uint16_t pin){
	GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
void JoyStick::DMA2_Init(void){
  __HAL_RCC_DMA2_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}
void JoyStick::ADC_Init(void) {
	ADC_ChannelConfTypeDef sConfig = {0};
		  hadc1.Instance = ADC1;
		  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
		  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
		  hadc1.Init.ScanConvMode = ENABLE;
		  hadc1.Init.ContinuousConvMode = DISABLE;
		  hadc1.Init.DiscontinuousConvMode = DISABLE;
		  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		  hadc1.Init.NbrOfConversion = 2;
		  hadc1.Init.DMAContinuousRequests = DISABLE;
		  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
		  HAL_ADC_Init(&hadc1);
		  sConfig.Channel = ADCX;
		  sConfig.Rank = 1;
		  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  sConfig.Channel = ADCY;
		  sConfig.Rank = 2;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}
void JoyStick::Init(GPIO_TypeDef *Joy_GPIO, GPIO_TypeDef *Button_GPIO){
	ADC_P = Joy_GPIO;
	BUT_P = Button_GPIO;
	DMA2_Init();
	ADC_Init();
	INPUT(Button_GPIO, BT_Pin);
}
uint8_t JoyStick::Button_State(uint8_t (*One_Click_CallBack)(void), uint8_t (*Long_Click_CallBack)(void)){
	if(HAL_GPIO_ReadPin(BUT_P, BT_Pin) == GPIO_PIN_RESET) {
		for(BT_Counter=0; BT_Counter<Time_Check; BT_Counter++) {
			HAL_Delay(1);
			if(HAL_GPIO_ReadPin(BUT_P, BT_Pin) == GPIO_PIN_SET) {
				BT_Counter = 0;
				if(One_Click_CallBack() ==0) return One_Click;
			}
		}
		if(BT_Counter == Time_Check){
			BT_Counter = 0;
			while(HAL_GPIO_ReadPin(BUT_P, BT_Pin) == GPIO_PIN_RESET);
			if(Long_Click_CallBack() ==0) return Long_Click;
		}
	}
		BT_Counter = 0;
		return No_Click;
}
void JoyStick::Read(void){
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc, 2);
	HAL_Delay(10);
	HAL_ADC_Stop_DMA(&hadc1);
}
uint16_t JoyStick::GetX(void){
//	uint16_t x = analogRead(X_Pin);
//	OUTPUT(ADC_P, X_Pin);
	return adc[1];
}
uint16_t JoyStick::GetY(void){
//	uint16_t y = analogRead(Y_Pin);
//	OUTPUT(ADC_P, Y_Pin);
	return adc[0];
}
int16_t JoyStick::Y_Tick(int16_t bottom, int16_t top){
	Read();
	if(GetY()<=(Y_Balance-1500) || GetY()>=(Y_Balance+1500)){
		Read();
			if(GetY() > Y_Balance) {
				CounterY--;
				HAL_Delay(200);
			}
			else if(GetY() < Y_Balance) {
				CounterY++;
				HAL_Delay(200);
			}
			if(CounterY < bottom) CounterY = bottom;
	    	if(CounterY > top)    CounterY = top;
			return CounterY;
	}
	return CounterY;
}
int16_t JoyStick::X_Tick(int16_t bottom, int16_t top){
	Read();
	if(GetX()<(X_Balance-1500) || GetX()>(X_Balance+1500)){
		Read();
			if(GetX() > X_Balance) {
				CounterX++;
				HAL_Delay(200);
			}
			else if(GetX() < X_Balance) {
				CounterX--;
				HAL_Delay(200);
			}
			if(CounterX < bottom) CounterX = bottom;
			if(CounterX > top)    CounterX = top;
			return CounterX;
	}
	return CounterX;
}
