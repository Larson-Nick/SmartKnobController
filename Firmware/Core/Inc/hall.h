/*
 * ADC.h
 *
 *  Created on: Jun 9, 2025
 *      Author: nickl
 */

#ifndef INC_HALL_H_
#define INC_HALL_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

class Hall{
public:
    Hall(ADC_HandleTypeDef* hadc);

    int16_t getReading(uint16_t angle);     // Returns corrected ADC value

private:
    static const uint16_t offsetTable[16384];

    ADC_HandleTypeDef* hadc;

    uint16_t readADC();                 // Internal ADC read
};

#endif /* INC_ADC_H_ */
