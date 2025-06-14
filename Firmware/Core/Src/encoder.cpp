/*
 * encoder.cpp
 *
 *  Created on: Jun 4, 2025
 *      Author: nickl
 *
 */

#include "encoder.h"

Encoder::Encoder(SPI_HandleTypeDef* hspi, TIM_HandleTypeDef* htim, GPIO& CSn)
    :
	rawAngle(0),
	hspi(hspi),
	htim(htim),
	CSn(CSn),
	zeroAngle(0),
	resetAngle(0),
	prevT(0),
	prevAngle(0),
	rawPosition(0),
	rawVelocity(0),
	dPos(0),
	dt(0)
{
}

void Encoder::update() {
    // Update raw angle data
	rawAngle = readRawAngle(); // [bits]

	// Update raw position data
	dPos = rawAngle - prevAngle;
	// "Over/underflow" protection
	if(dPos > (resolution)/2){
		dPos -= (resolution);
	}
	else if(dPos < -(resolution)/2){
		dPos += (resolution);
	}
	rawPosition += dPos; // [bits]

	// Update raw velocity data
	// Not implementing overflow protection on timer, should be short enough usage.
	uint32_t t = micros();
	dt = t - prevT;

	rawVelocity = (dPos*1000)/((int32_t)dt); // [bits/ms] - keeping everything as integers.

	// Update previous values
	prevAngle = rawAngle;
	prevT = t;
}

int32_t Encoder::getCommutationAngle() {
	// Access the angle relative to electrical zero reference in integer bits
	int32_t commRawAngle = readRawAngle();
	int32_t angle = (int32_t)commRawAngle - commutationZeroAngle; // cast values to prevent wrap-around instead of negative results.
	if (angle < 0) {
		angle += resolution;
	}
	return angle;
}

float Encoder::getAngle() {
    // Convert rawAngle to [rad]
	int32_t angle = (int32_t)rawAngle - (int32_t)zeroAngle; // cast values to prevent wrap-around instead of negative results.
	if (angle < 0) {
		angle += resolution;
	}
	return (2 * (float)M_PI * (float)angle) / (float)resolution;
}

float Encoder::getPosition() {
	// Convert rawPosition to [rad]
	int32_t position = rawPosition - ((int32_t)zeroAngle - (int32_t)resetAngle); // cast values to prevent wrap-around instead of negative results.
	return (2 * (float)M_PI * (float)position) / (float)resolution;
}

float Encoder::getVelocity() {
    //Convert rawVelocity to [rad/s]
    return (2000 * (float)M_PI * (float)rawVelocity) / (float)resolution;
}

void Encoder::zero() {
    // Sets 0deg datum, must reset rawPosition to avoid inconsistency
	update();
	zeroAngle = rawAngle;
	// do resetPosition stuff without updating the encoder again.
	rawPosition = 0;
	resetAngle = rawAngle;
}

void Encoder::resetPosition() {
    // Resets the accumulating position measurement,
	// records the rawAngle where position was reset so it can be offset from zero and reference aligns with rawAngle.
	update();
	rawPosition = 0;
	resetAngle = rawAngle;
}

uint16_t Encoder::readRawAngle() {
	//Return raw SPI bit value of encoder

	__disable_irq();

	uint16_t encPosAddr = 0x3FFF;
	uint16_t encPosBuf;

	//Go go SPI!
	CSn.low(); // logic-low on CSn pin, custom functions
	HAL_SPI_TransmitReceive(hspi, (uint8_t*)&encPosAddr, (uint8_t*)&encPosBuf, 1, HAL_MAX_DELAY);
	CSn.high(); // logic-high on CSn pin, custom functions

	__enable_irq();

	//return encPosBuf & 0x3FFF;
    return (16384 - (encPosBuf & 0x3FFF)) % 16384;
    // masking first two bits to get 14-bit position data as integer
    // reversing the direction that angle accumulates as encoder rotates to align CCW+
}

uint32_t Encoder::micros() const {
    //Return time in microseconds
	return __HAL_TIM_GET_COUNTER(htim);
}


