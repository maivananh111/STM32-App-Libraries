
#include "TouchScreen.h"

#define NUMSAMPLES 2
#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET
#define NO   0
#define UP   1
#define DOWN 2

extern ADC_HandleTypeDef hadc1;

static uint16_t ADCX1, ADCY1;
GPIO_TypeDef *ADC_P, *D_P;

void TouchScreen::ADC1_Init(void) {
	__HAL_RCC_ADC1_CLK_ENABLE();
	hadc1.Instance = ADC1;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.NbrOfDiscConversion = 0;
	hadc1.Init.NbrOfConversion = 1;
	HAL_ADC_Init(&hadc1);
}
void TouchScreen::INPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PULL){
	GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = GPIO_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  if(PULL == NO){
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  }
	  else if(PULL == UP){
		  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  }
	  else{
		  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  }
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
  if(pin == _yp) { sConfig.Channel = ADCY1;}
  else if(pin == _xm) { sConfig.Channel = ADCX1;}
  sConfig.Rank = 1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) { return 0; }
  if (HAL_ADC_Start(&hadc1) != HAL_OK) { return 0; }
  HAL_ADC_PollForConversion(&hadc1, 10);
  if (HAL_ADC_GetState(&hadc1) != HAL_OK) {
    return ((HAL_ADC_GetValue(&hadc1)) >> 2);
  }
  HAL_ADC_Stop(&hadc1);
  __HAL_RCC_ADC1_CLK_DISABLE();
   HAL_GPIO_DeInit(ADC_P, pin);
  return 0;
}
TSPoint::TSPoint(void) { x = y = z = 0; }

TSPoint::TSPoint(int16_t x0, int16_t y0, int16_t z0) {
  x = x0;
  y = y0;
  z = z0;
}
bool TSPoint::operator==(TSPoint p1) {
  return ((p1.x == x) && (p1.y == y) && (p1.z == z));
}
bool TSPoint::operator!=(TSPoint p1) {
  return ((p1.x != x) || (p1.y != y) || (p1.z != z));
}
TouchScreen::TouchScreen(uint16_t xp, uint16_t yp, uint16_t xm, uint16_t ym, uint16_t rxplate = 0) {
  _yp = yp;
  _xm = xm;
  _ym = ym;
  _xp = xp;
  _rxplate = rxplate;
  Check = 10;
    switch(_xm){
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
  switch(_yp){
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
void TouchScreen::Calibration(float ax, float bx, float ay, float by){
	A = ax;
	B = bx;
	C = ay;
	D = by;
}
TSPoint TouchScreen::ReadPoint(void) {
  bool valid = true;
  int z;
  uint16_t samples[NUMSAMPLES];
  uint8_t i;
  INPUT(ADC_P, _yp, NO);//////////////////////////////////////////////////////////
  INPUT(D_P, _ym, NO);
  OUTPUT(D_P, _xp);
  OUTPUT(ADC_P, _xm);///////////////////////////////////////////////////////
  HAL_GPIO_WritePin(D_P, _xp, HIGH);
  HAL_GPIO_WritePin(ADC_P,_xm, LOW);///////////////////////////////////////////////////

  HAL_Delay(1);
  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = analogRead(_yp);////////////////////////////////////////////
  }
#if NUMSAMPLES == 2
  if (samples[0] - samples[1] < -20 || samples[0] - samples[1] > 20) {
    valid = false;
  } else {
    samples[1] = (samples[0] + samples[1]) >> 1;
    valid = true;
  }
#endif
  _y = (1023 - samples[NUMSAMPLES / 2]);

  INPUT(D_P, _xp, NO);
  INPUT(ADC_P, _xm, NO);////////////////////////////////////////////
  OUTPUT(ADC_P, _yp);///////////////////////////////////////////////////////////
  OUTPUT(D_P, _ym);
  HAL_GPIO_WritePin(D_P, _ym, LOW);
  HAL_GPIO_WritePin(ADC_P, _yp, HIGH);////////////////////////////////////////////

  HAL_Delay(1);
  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = analogRead(_xm);////////////////////////////////////////////
  }
#if NUMSAMPLES == 2
  if (samples[0] - samples[1] < -20 || samples[0] - samples[1] > 20) {
    valid = false;
  } else {
    samples[1] = (samples[0] + samples[1]) >> 1;
    valid = true;
  }
#endif
//  y = (1023 - samples[NUMSAMPLES / 2]);
  _x = (samples[NUMSAMPLES / 2]);

  OUTPUT(D_P, _xp);
  INPUT(ADC_P, _yp, NO);////////////////////////////////////////////

  HAL_GPIO_WritePin(D_P, _xp, LOW);
  HAL_GPIO_WritePin(D_P, _ym, HIGH);
  HAL_Delay(1);

  int z1 = analogRead(_xm);////////////////////////////////////////////
  int z2 = analogRead(_yp);////////////////////////////////////////////

  if (_rxplate != 0) {
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= _x;
    rtouch *= _rxplate;
    rtouch /= 1024;
    z = rtouch;
  } else {
    z = (1023 - (z2 - z1));
  }
  if (!valid) {
    z = 0;
  }
  if(z>3500) z=0;
	OUTPUT(ADC_P, _xm);
	OUTPUT(D_P, _xp);
	OUTPUT(ADC_P, _yp);
	OUTPUT(D_P, _ym);
	_x = A*_x+B;
	_y = C*_y+D;
  return TSPoint(_x, _y, z);
}
uint16_t TouchScreen::Get_X(void) {
	INPUT(ADC_P, _yp, DOWN);
	INPUT(D_P, _ym, DOWN);

    OUTPUT(D_P, _xp);
    HAL_GPIO_WritePin(D_P, _xp, HIGH);
    OUTPUT(ADC_P, _xm);
    HAL_GPIO_WritePin(ADC_P, _xm, LOW);

    OUTPUT(ADC_P, _xm);
  	OUTPUT(D_P, _xp);
  	OUTPUT(ADC_P, _yp);
  	OUTPUT(D_P, _ym);
  return (analogRead(_yp));
}
uint16_t TouchScreen::Get_Y(void) {
	INPUT(D_P, _xp, DOWN);
	INPUT(ADC_P, _xm, DOWN);

    OUTPUT(ADC_P, _yp);
    HAL_GPIO_WritePin(ADC_P, _yp, HIGH);
    OUTPUT(D_P, _ym);
    HAL_GPIO_WritePin(D_P, _ym, LOW);

    OUTPUT(ADC_P, _xm);
	OUTPUT(D_P, _xp);
	OUTPUT(ADC_P, _yp);
	OUTPUT(D_P, _ym);

  return (1023 - analogRead(_xm));
}
uint16_t TouchScreen::pressure(void) {
	OUTPUT(D_P, _xp);
  HAL_GPIO_WritePin(D_P, _xp, LOW);

  OUTPUT(D_P, _ym);
  HAL_GPIO_WritePin(D_P, _ym, HIGH);

  HAL_GPIO_WritePin(ADC_P, _xm, LOW);
  INPUT(ADC_P, _xm, NO);
  HAL_GPIO_WritePin(ADC_P, _yp, LOW);
  INPUT(ADC_P, _yp, NO);

  int z1 = analogRead(_xm);
  int z2 = analogRead(_yp);

    OUTPUT(ADC_P, _xm);
  	OUTPUT(D_P, _xp);
  	OUTPUT(ADC_P, _yp);
  	OUTPUT(D_P, _ym);

  if (_rxplate != 0) {
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= Get_X();
    rtouch *= _rxplate;
    rtouch /= 1024;
    return rtouch;
  } else {
    return (1023 - (z2 - z1));
  }
}
