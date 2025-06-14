/*
* time_utils.c
*
* Created on: May 27, 2025
* Author: jacka
*/

#include "time_utils.h"

extern TIM_HandleTypeDef htim5;

uint32_t micros(void) {
	return __HAL_TIM_GET_COUNTER(&htim5);
}

void delay_us(uint32_t us) {
	uint32_t start = micros();
	while ((micros() - start) < us) {
		// Tight loop until target time is reached
	}
}



