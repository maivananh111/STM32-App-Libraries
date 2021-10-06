/*
 * TouchPad.h
 *
 * Created: 06/08/2021 4:12:26 CH
 *  Author: A315-56
 */ 


#ifndef TOUCHPAD_H_
#define TOUCHPAD_H_


#define REMOTE 1
#define STREAM 2

#include <stdint.h>
#include "stm32f4xx_hal.h"

class TouchPad{
	public:
	    TouchPad(GPIO_TypeDef *GPIOx, uint16_t clock_pin, uint16_t data_pin, uint8_t mode);
		void Init();
		int16_t read();
		int16_t* report(int16_t data[]);
		void write(int16_t data);
		void enable_data_reporting();
		void disable_data_reporting();
		void set_remote_mode();
		void set_stream_mode();
		void set_resolution(int16_t resolution);
		void set_scaling_2_1();
		void set_scaling_1_1();
		void set_sample_rate(int16_t rate);
	
	private:
		GPIO_TypeDef *PORT;
		uint16_t _clock_pin;
		uint16_t _data_pin;
		uint8_t _mode;
		bool _initialized;
		bool _enabled;
		bool _disabled;
		int8_t read_byte();
		int8_t read_bit();
		int16_t read_movement_x(int16_t status);
		int16_t read_movement_y(int16_t status);
		void pull_high(uint16_t pin);
		void pull_low(uint16_t pin);
		void set_mode(int16_t data);
};


#endif /* TOUCHPAD_H_ */
