/*
 * PID.h
 *
 *  Created on: Jun 8, 2025
 *      Author: nickl
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

class PID {
public:
    PID(float kp, float ki, float kd, TIM_HandleTypeDef* htim);

    float update(float error);  // Returns control effort
    void reset();               // Clears integral term

private:
    float kp, ki, kd;
    TIM_HandleTypeDef* htim;

    float integral;
    float prevErr;
    uint32_t prevT;

    uint32_t micros() const;    // Returns current time in microseconds
};

#endif /* INC_PID_H_ */

