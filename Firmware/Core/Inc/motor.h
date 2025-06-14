/*
 * motor.h
 *
 *  Created on: Jun 4, 2025
 *      Author: nickl
 *
 *      UPDATED PROJECT VERSION
 *
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "encoder.h"

class Motor {
public:
    Motor(TIM_HandleTypeDef* htim,
          uint32_t chA, uint32_t chB, uint32_t chC,
          GPIO& enA, GPIO& enB, GPIO& enC,
          Encoder& encoder);

    void setEffort(int effort);  // -100 to 100
    void commutate();
    void enable();
    void disable();
    void testCommutation(int32_t theta, float Vq);
    void applyPWM(float Va, float Vb, float Vc);

private:
    static const float commTable[16384];

    TIM_HandleTypeDef* htim;
    uint32_t chA, chB, chC;
    GPIO& enA;
    GPIO& enB;
    GPIO& enC;
    Encoder& encoder;

    int targetEffort;

    static constexpr float V_max = 12;
    static constexpr uint16_t polePairs = 11;
    static constexpr uint16_t resolution = 16384; // Associated encoder resolution, maybe change so it is directly taken from encoder

};

#endif /* INC_MOTOR_H_ */
