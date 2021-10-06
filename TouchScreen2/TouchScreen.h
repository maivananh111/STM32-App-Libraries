
#include <stdint.h>
#include "stm32f4xx_hal.h"


typedef volatile uint32_t RwReg;

class TSPoint {
	public:
	  TSPoint(void);
	  TSPoint(int16_t x, int16_t y, int16_t z);
	  bool operator==(TSPoint);
	  bool operator!=(TSPoint);

	  int16_t x, y, z;
};

class TouchScreen {
	public:
	  TouchScreen(uint16_t xp, uint16_t yp, uint16_t xm, uint16_t ym, uint16_t rx);
	  void ADC1_Init(void);
	  void INPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PULL);
	  void OUTPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	  uint16_t analogRead(uint16_t pin);
	  void Init(GPIO_TypeDef *ADC_GPIO, GPIO_TypeDef *DIG_GPIO);
	  bool Touch(void);
	  uint16_t pressure(void);
	  uint16_t Get_X(void);
	  uint16_t Get_Y(void);
	  void Calibration(float ax, float bx, float ay, float by);
	  TSPoint ReadPoint();
	  int16_t Check;

	private:
	  uint16_t _x, _y;
	  float A=1, B=0, C=1, D=0;
	  uint16_t _yp, _ym, _xm, _xp;
	  uint16_t _rxplate;


};
