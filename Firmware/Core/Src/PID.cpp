/*
 * PID.cpp
 *
 *  Created on: Jun 8, 2025
 *      Author: nickl
 */

#include "PID.h"

PID::PID(float kp, float ki, float kd, TIM_HandleTypeDef* htim)
    : kp(kp), ki(ki), kd(kd), htim(htim),
      integral(0.0f), prevErr(0.0f), prevT(micros())
{
}

float PID::update(float error) {
    // compute PID response
	uint32_t t = micros();
	float dt = (t - prevT) / 1e6f;

	if (dt == 0) {return 0.0f;}

	float derivative = (error - prevErr) / (float)dt;
	integral += error * dt;

	prevT = t;
	prevErr = error;

    return kp*error + ki*integral + kd*derivative;
}

void PID::reset() {
	// reset Accumulation
    integral = 0.0f;
    prevErr = 0.0f;
    prevT = micros();
}

uint32_t PID::micros() const {
	// helper function to get current count
    return __HAL_TIM_GET_COUNTER(htim);
}




