/*
 * encoder.h
 *
 *  Created on: Jun 4, 2025
 *      Author: nickl
 *
 *      UPDATED PROJECT VERSION
 *
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "math.h"

#include "gpio.h"

class Encoder {
public:
    Encoder(SPI_HandleTypeDef* hspi, TIM_HandleTypeDef* htim, GPIO& CSn);

    uint16_t rawAngle; 	    	// angle [int bits]

    void update(); 			// Call regularly to update internal state
    int32_t getCommutationAngle(); // [integer bits], for commutation
    float getAngle();       // [rad], absolute angle within one turn
    float getPosition();    // [rad], cumulative multi-turn position
    float getVelocity();    // [rad/s], estimated from last update
    void zero();            // Sets current raw angle as zero
    void resetPosition();   // Resets multi-turn position accumulator
    uint16_t readRawAngle();   	// SPI read helper function

private:
    SPI_HandleTypeDef* hspi;
    TIM_HandleTypeDef* htim;
    GPIO& CSn;

    static constexpr uint16_t resolution = 16384; 	// 14-bit encoder
    static constexpr int32_t commutationZeroAngle = (1722 - (16384/44)); //233 locked rotor angle + 90deg electric (16384/44)
    uint16_t zeroAngle;        						// zero reference angle
    uint16_t resetAngle;							// position where accumulation was zeroed to eliminate offset

    uint32_t prevT;       		// last update time [us]
    uint16_t prevAngle;			// last update angle [int bits]
    int32_t rawPosition; 		// rotational position [int bits]
    int32_t rawVelocity;       	// velocity [int bits]
	int32_t dPos;
	uint32_t dt;

    uint32_t micros() const;  	// us timing helper function
};

#endif /* INC_ENCODER_H_ */
