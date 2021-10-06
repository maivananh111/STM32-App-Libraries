/*
 * Touch.cpp
 *
 *  Created on: Jul 19, 2021
 *      Author: A315-56
 */
#include "TouchScreen.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"

extern ADC_HandleTypeDef hadc1;

static uint16_t X1, Y1, X2, Y2;
static uint16_t ADCX1, ADCY1;
GPIO_TypeDef *ADC_P, *D_P;

TouchScreen::TouchScreen(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
  X1 = x1;
  X2 = x2;
  Y1 = y1;
  Y2 = y2;
  switch(x1){
    case GPIO_PIN_0: ADCX1 = ADC_CHANNEL_0; break;
    case GPIO_PIN_1: ADCX1 = ADC_CHANNEL_1; break;
    case GPIO_PIN_2: ADCX1 = ADC_CHANNEL_2; break;
    case GPIO_PIN_3: ADCX1 = ADC_CHANNEL_3; break;
    case GPIO_PIN_4: ADCX1 = ADC_CHANNEL_4; break;
    case GPIO_PIN_5: ADCX1 = ADC_CHANNEL_5; break;
    case GPIO_PIN_6: ADCX1 = ADC_CHANNEL_6; break;
    case GPIO_PIN_7: ADCX1 = ADC_CHANNEL_7; break;
    case GPIO_PIN_8: ADCX1 = ADC_CHANNEL_8; break;
    case GPIO_PIN_9: ADCX1 = ADC_CHANNEL_9; break;
    case GPIO_PIN_10: ADCX1 = ADC_CHANNEL_10; break;
    case GPIO_PIN_11: ADCX1 = ADC_CHANNEL_11; break;
    case GPIO_PIN_12: ADCX1 = ADC_CHANNEL_12; break;
    case GPIO_PIN_13: ADCX1 = ADC_CHANNEL_13; break;
    case GPIO_PIN_14: ADCX1 = ADC_CHANNEL_14; break;
    case GPIO_PIN_15: ADCX1 = ADC_CHANNEL_15; break;
  }
  switch(y1){
      case GPIO_PIN_0: ADCY1 = ADC_CHANNEL_0; break;
      case GPIO_PIN_1: ADCY1 = ADC_CHANNEL_1; break;
      case GPIO_PIN_2: ADCY1 = ADC_CHANNEL_2; break;
      case GPIO_PIN_3: ADCY1 = ADC_CHANNEL_3; break;
      case GPIO_PIN_4: ADCY1 = ADC_CHANNEL_4; break;
      case GPIO_PIN_5: ADCY1 = ADC_CHANNEL_5; break;
      case GPIO_PIN_6: ADCY1 = ADC_CHANNEL_6; break;
      case GPIO_PIN_7: ADCY1 = ADC_CHANNEL_7; break;
      case GPIO_PIN_8: ADCY1 = ADC_CHANNEL_8; break;
      case GPIO_PIN_9: ADCY1 = ADC_CHANNEL_9; break;
      case GPIO_PIN_10: ADCY1 = ADC_CHANNEL_10; break;
      case GPIO_PIN_11: ADCY1 = ADC_CHANNEL_11; break;
      case GPIO_PIN_12: ADCY1 = ADC_CHANNEL_12; break;
      case GPIO_PIN_13: ADCX1 = ADC_CHANNEL_13; break;
      case GPIO_PIN_14: ADCY1 = ADC_CHANNEL_14; break;
      case GPIO_PIN_15: ADCY1 = ADC_CHANNEL_15; break;
    }
}
void TouchScreen::Init(GPIO_TypeDef *ADC_GPIO, GPIO_TypeDef *DIG_GPIO){
	ADC_P = ADC_GPIO;
	D_P = DIG_GPIO;
}
void TouchScreen::ADC1_Init(void) {
	/* Enable clock */
	__HAL_RCC_ADC1_CLK_ENABLE();
	/* Configure the ADC peripheral */
	hadc1.Instance = ADC1;
	/* Fill settings */
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.NbrOfDiscConversion = 0;
	hadc1.Init.NbrOfConversion = 1;
	/* Init ADC */
	HAL_ADC_Init(&hadc1);
}
void TouchScreen::INPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool PULL){
	GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = GPIO_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = (PULL)? GPIO_PULLUP : GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
void TouchScreen::OUTPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	 GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = GPIO_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
uint16_t TouchScreen::analogRead(uint16_t pin){  // pin is PA0 - PA15
  ADC_ChannelConfTypeDef sConfig;
  __HAL_RCC_ADC1_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC_P, &GPIO_InitStruct);
  ADC1_Init();
  if(pin == Y1) { sConfig.Channel = ADCY1;}
  else if(pin == X1) { sConfig.Channel = ADCX1;}
  sConfig.Rank = 1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) { return 0; }
  if (HAL_ADC_Start(&hadc1) != HAL_OK) { return 0; }
  HAL_ADC_PollForConversion(&hadc1, 10);
  if (HAL_ADC_GetState(&hadc1) != HAL_OK) {
    return ((HAL_ADC_GetValue(&hadc1) + 2) >> 2);
  }
  HAL_ADC_Stop(&hadc1);
  __HAL_RCC_ADC1_CLK_DISABLE();
   HAL_GPIO_DeInit(ADC_P, pin);
  return 0;
}
bool TouchScreen::ReadTouch(void){
  int8_t touch, wait_touch = 1;
  uint16_t samples[10];
  bool valid = false;
  while (wait_touch) {
    INPUT(ADC_P, Y1, false);
    INPUT(D_P, Y2, true);
    OUTPUT(ADC_P, X1);
    OUTPUT(D_P, X2);

    HAL_GPIO_WritePin(ADC_P, X1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D_P, X2, GPIO_PIN_RESET);

    touch = !HAL_GPIO_ReadPin(ADC_P, Y1);
    if (touch) {
      valid = true;
      HAL_GPIO_WritePin(D_P, X2, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(ADC_P, X1, GPIO_PIN_SET);
      for(int i=0; i<10; i++){ samples[i] = analogRead(Y1); }
      if (abs(samples[0] - samples[5]) > 30) {
          valid = false;
          return valid;
      }
      else {
    	for(int i=0; i<10; i++){ row += samples[i]; }
        row = row/10;
      }

      INPUT(ADC_P, X1, false);
      INPUT(D_P, X2, true);
      OUTPUT(ADC_P, Y1);
      OUTPUT(D_P, Y2);
      HAL_GPIO_WritePin(ADC_P, Y1, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(D_P, Y2, GPIO_PIN_SET);
      for(int i=0; i<10; i++){ samples[i] = analogRead(X1); }
      if (abs(samples[0] - samples[5]) > 30) {
          valid = false;
          return valid;
      }
      else {
         for(int i=0; i<10; i++){ col += samples[i]; }
         col = col/10;
      }

      HAL_GPIO_WritePin(D_P, Y2, GPIO_PIN_RESET);
      touch = !HAL_GPIO_ReadPin(ADC_P, X1);
      if (touch) { valid = true; }
      wait_touch=0;
    }
    else valid = false;
  }
	OUTPUT(ADC_P, X1);
	OUTPUT(D_P, X2);
	OUTPUT(ADC_P, Y1);
	OUTPUT(D_P, Y2);
  return valid;
}
void TouchScreen::Calibration(float ax, float bx, float ay, float by){
  Ax = ax;
  Bx = bx;
  Ay = ay;
  By = by;
}
uint16_t TouchScreen::GetX(){ return (Ax * col + Bx); }   //Read X value
uint16_t TouchScreen::GetY(){ return (Ay * row + By); }   //Read Y value

