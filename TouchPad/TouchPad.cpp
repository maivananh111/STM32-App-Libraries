/*
 * TouchPad.cpp
 *
 * Created: 06/08/2021 4:12:41 CH
 *  Author: A315-56
 */ 

#include "TouchPad.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

extern TIM_HandleTypeDef htim1;

static void _delay_us (uint16_t time){
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim1))<time);
}

TouchPad::TouchPad(GPIO_TypeDef *GPIOx, uint16_t clock_pin, uint16_t data_pin, uint8_t mode) {
	PORT = GPIOx;
	_clock_pin = clock_pin;
	_data_pin = data_pin;
	_mode = mode;
	_initialized = false;
	_disabled = true;
	_enabled = false;
}

void TouchPad::Init() {
	pull_high(_clock_pin);
	pull_high(_data_pin);
	HAL_Delay(20);
	write(0xff); // Send Reset to the mouse
	read_byte();  // Read ack byte
	HAL_Delay(20); // Not sure why this needs the delay
	read_byte();  // blank
	read_byte();  // blank
	HAL_Delay(20); // Not sure why this needs the delay
	if (_mode == REMOTE) {
		set_remote_mode();
		} else {
		enable_data_reporting(); // Tell the mouse to start sending data again
	}
	_delay_us(100);
	_initialized = 1;
}

void TouchPad::set_mode(int16_t data) {
	if (_mode == STREAM) {
		disable_data_reporting(); // Tell the mouse to stop sending data.
	}
	write(data);  // Send Set Mode
	read_byte();  // Read Ack byte
	if (_mode == STREAM) {
		enable_data_reporting(); // Tell the mouse to start sending data again
	}
	if (_initialized) {
		_delay_us(100);
	}
}

void TouchPad::set_remote_mode() {
	set_mode(0xf0);
	_mode = REMOTE;
}

void TouchPad::set_stream_mode() {
	set_mode(0xea);
	_mode = STREAM;
}

void TouchPad::set_sample_rate(int16_t rate) {
	if (_mode == STREAM) {
		disable_data_reporting(); // Tell the mouse to stop sending data.
	}
	write(0xf3); // Tell the mouse we are going to set the sample rate.
	read_byte(); // Read Ack Byte
	write(rate); // Send Set Sample Rate
	read_byte(); // Read ack byte
	if (_mode == STREAM) {
		enable_data_reporting(); // Tell the mouse to start sending data again
	}
	_delay_us(100);
}

void TouchPad::set_scaling_2_1() {
	set_mode(0xe7); // Set the scaling to 2:1
}

void TouchPad::set_scaling_1_1() {
	set_mode(0xe6); // set the scaling to 1:1
}

// This only effects data reporting in Stream mode.
void TouchPad::enable_data_reporting() {
	if (!_enabled) {
		write(0xf4); // Send enable data reporting
		read_byte(); // Read Ack Byte
		_enabled = true;
	}
}

// Disabling data reporting in Stream Mode will make it behave like Remote Mode
void TouchPad::disable_data_reporting() {
	if (!_disabled) {
		write(0xf5); // Send disable data reporting
		read_byte(); // Read Ack Byte
		_disabled = true;
	}
}

void TouchPad::set_resolution(int16_t resolution) {
	if (_mode == STREAM) {
		enable_data_reporting();
	}
	write(0xe8); // Send Set Resolution
	read_byte(); // Read ack Byte
	write(resolution); // Send resolution setting
	read_byte(); // Read ack Byte
	if (_mode == STREAM) {
		disable_data_reporting();
	}
	_delay_us(100);
}

void TouchPad::write(int16_t data) {
	char i;
	char parity = 1;
	pull_high(_data_pin);
	pull_high(_clock_pin);
	_delay_us(300);
	pull_low(_clock_pin);
	_delay_us(300);
	pull_low(_data_pin);
	_delay_us(10);
	pull_high(_clock_pin); // Start Bit
	while (HAL_GPIO_ReadPin(PORT, _clock_pin)); // wait for mouse to take control of clock)
	// clock is low, and we are clear to send data
	for (i=0; i < 8; i++) {
		if (data & 0x01) {
			pull_high(_data_pin);
			} else {
			pull_low(_data_pin);
		}
		// wait for clock cycle
		while (!HAL_GPIO_ReadPin(PORT, _clock_pin));
		while (HAL_GPIO_ReadPin(PORT, _clock_pin));
		parity = parity ^ (data & 0x01);
		data = data >> 1;
	}
	// parity
	if (parity) {
		pull_high(_data_pin);
		} else {
		pull_low(_data_pin);
	}
	while (!HAL_GPIO_ReadPin(PORT, _clock_pin));
	while (HAL_GPIO_ReadPin(PORT, _clock_pin));
	pull_high(_data_pin);
	_delay_us(50);
	while (HAL_GPIO_ReadPin(PORT, _clock_pin));
	while ((!HAL_GPIO_ReadPin(PORT, _clock_pin)) || (!HAL_GPIO_ReadPin(PORT, _data_pin))); // wait for mouse to switch modes
	pull_low(_clock_pin); // put a hold on the incoming data.
}

int16_t * TouchPad::report(int16_t data[]) {
	write(0xeb); // Send Read Data
	read_byte(); // Read Ack Byte
	data[0] = read(); // Status bit
	data[1] = read_movement_x(data[0]); // X Movement Packet
	data[2] = read_movement_y(data[0]); // Y Movement Packet
	return data;
}

int16_t TouchPad::read() {
	return read_byte();
}

int8_t TouchPad::read_byte() {
	int8_t data = 0;
	pull_high(_clock_pin);
	pull_high(_data_pin);
	_delay_us(50);
	while (HAL_GPIO_ReadPin(PORT, _clock_pin));
	_delay_us(5);  // not sure why.
	while (!HAL_GPIO_ReadPin(PORT, _clock_pin)); // eat start bit
	for (int i = 0; i < 8; i++) {
		data |= (read_bit()<<i);
	}
	read_bit(); // Partiy Bit
	read_bit(); // Stop bit should be 1
	pull_low(_clock_pin);
	return data;
}

int8_t TouchPad::read_bit() {
	while (HAL_GPIO_ReadPin(PORT, _clock_pin));
	int bit = HAL_GPIO_ReadPin(PORT, _data_pin);
	while (!HAL_GPIO_ReadPin(PORT, _clock_pin));
	return bit;
}

int16_t TouchPad::read_movement_x(int16_t status) {
	int16_t x = read();
	if (status&(1<<4)) {
		for(int i = 8; i < 16; ++i) {
			x |= (1<<i);
		}
	}
	return x;
}

int16_t TouchPad::read_movement_y(int16_t status) {
	int16_t y = read();
	if (status&(1<<5)) {
		for(int i = 8; i < 16; ++i) {
			y |= (1<<i);
		}
	}
	return y;
}

void TouchPad::pull_low(uint16_t pin) {
	GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(PORT, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(PORT, pin, GPIO_PIN_RESET);
}

void TouchPad::pull_high(uint16_t pin) {
	GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(PORT, &GPIO_InitStruct);
}
