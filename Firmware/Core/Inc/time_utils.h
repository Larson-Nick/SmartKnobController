/*
* time_utils.h
*
* Created on: May 27, 2025
* Author: jacka
*/

#ifndef INC_TIME_UTILS_H_
#define INC_TIME_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f4xx_hal.h>
#include <stdint.h>

uint32_t micros(void);
void delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* INC_TIME_UTILS_H_ */
