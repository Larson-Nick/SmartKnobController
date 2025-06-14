/*
 * FSM.cpp
 *
 *  Created on: Jun 8, 2025
 *      Author: nickl
 */

#include "fsm.h"
#include "stdio.h"
#include "string.h"

FSM::FSM(Motor& motor, Encoder& encoder, PID& pid, Hall& hall, UART_HandleTypeDef* huart)
    : motor(motor),
      encoder(encoder),
      pid(pid),
      hall(hall),
      huart(huart),
      stateIndex(0),
      currentMode(Mode::Elastic),
      currentFunction(Function::Translate),
	  prevT(0)
{
    // Initializes the FSM with references and starting state
}

void FSM::run(uint32_t freq) {
    // Dispatches to the current state

	uint32_t period = 1000/freq;
	uint32_t t = HAL_GetTick();

	if ((t - prevT) < period) {return;}
	prevT = t;

    switch (currentMode) {
    	case Mode::Elastic:
			stateElastic();
			break;
        case Mode::FreeSpin:
            stateFreeSpin();
            break;
    }
}

void FSM::advanceState(bool longPress) {
	if (longPress) {
		currentMode = Mode::FreeSpin;
		currentFunction = Function::Rotate;
		stateIndex = 3;
	} else {
		// Toggle between Elastic-Translate and Elastic-Rotate
		if (currentMode == Mode::Elastic) {
			if (currentFunction == Function::Translate) {
				currentFunction = Function::Rotate;
				stateIndex = 1;
			} else {
				currentFunction = Function::Translate;
				stateIndex = 0;
			}
		} else {
			currentMode = Mode::Elastic;
			currentFunction = Function::Translate;
			stateIndex = 0;
		}
	}
}

void FSM::stateFreeSpin() {
    // Controls the motor in free-spin (bearing-like) mode
    // Builds UART message based on currentFunction

	// "Sense"
	encoder.update();
	int16_t adcVal = hall.getReading(encoder.rawAngle);

	// "Control"
	float K_ff = 1.144f;
	float bias = 2.08f;

	float position = encoder.getPosition();
	float velocity = encoder.getVelocity();
	float effort;
	if (velocity >= 0) {effort = K_ff * velocity + bias;}
	else if (velocity < 0) {effort = K_ff * velocity - bias;}

	// "Actuate"
	motor.setEffort((int)effort);

	// "Command"
	// MODE C
	if(currentFunction == Function::Translate) {

	}
	// MODE D
	else if(currentFunction == Function::Rotate) {
		// PLACEHOLDER - Read and scale ADC for translational effort
		int16_t transEff = 0;
		if(adcVal >= 15) {
			transEff = 25;
		}
		else if(adcVal <= -15) {
			transEff = -25;
		}

		// Clamp translational effort
		if (transEff > 100) {transEff = 100;}
		else if (transEff < -100) {transEff = -100;}

		char hexEff[4];
		sprintf(hexEff, "%02X", (uint8_t)transEff);

		// Scale position to preserve 1 decimal point of precision [0.1 rad]
		int16_t intPos = (int16_t)(-position * 10.0f);
		char hexPos[6];  // 4 chars + null terminator + buffer in case.
		sprintf(hexPos, "%04X", (uint16_t)intPos);

		char mode = 'D'; // 'A' for Elastic Translation Mode

		// Concatenate final command
		sprintf(command, "%c%s%s\r\n", mode, hexPos, hexEff);
		sendCommand(command);
	}
}

void FSM::stateElastic() {
    // Controls the motor with PI spring-to-center behavior
    // Builds UART message based on currentFunction

	// "Sense"
	encoder.update();
	int16_t adcVal = hall.getReading(encoder.rawAngle);

	// "Control"
	float position = encoder.getPosition();
	float effort = pid.update(-position);

	// "Actuate"
	motor.setEffort((int)effort);

	// "Command"
	// MODE A
	if(currentFunction == Function::Translate) {
		// Read and scale position from motor for translational effort
		float scale = 50.0f / 2.356f; // 50% effort at ±135°
		int16_t transEff = (int16_t)(-position * scale); // Spin CW to go forward

		// Clamp to translation effort
		if (transEff > 100) {transEff = 100;}
		else if (transEff < -100) {transEff = -100;}

		// PLACEHOLDER - Read and scale ADC for rotation effort
		int16_t rotEff = 0;
		//if(abs(adcVal) > 10) {
			//rotEff = adcVal;
		//}

		// Clamp rotation effort
		if (rotEff > 100) {rotEff = 100;}
		else if (rotEff < -100) {rotEff = -100;}

		// Combine
		int16_t cmdEffL = transEff + rotEff;
		int16_t cmdEffR = transEff - rotEff;

		// Clamping total
		if (cmdEffL > 100) {cmdEffL = 100;}
		else if (cmdEffL < -100) {cmdEffL = -100;}

		if (cmdEffR > 100) {cmdEffR = 100;}
		else if (cmdEffR < -100) {cmdEffR = -100;}

		// Convert to 2's complement hex
		char hexEffL[4]; // Needs length 3 for null terminator
		char hexEffR[4];
		sprintf(hexEffL, "%02X", (uint8_t)cmdEffL);
		sprintf(hexEffR, "%02X", (uint8_t)cmdEffR);

		char mode = 'A'; // 'A' for Elastic Translation Mode

		// Concatenate final command
		sprintf(command, "%c%s%s\r\n", mode, hexEffL, hexEffR);
		sendCommand(command);
	}
	// MODE B
	else if(currentFunction == Function::Rotate) {
		// Read and scale position from motor for rotational effort
		float scale = 30.0f / 2.356f; // 30% effort at ±135°
		int16_t rotEff = (int16_t)(position * scale);

		// Clamp to rotation effort
		if (rotEff > 100) {rotEff = 100;}
		else if (rotEff < -100) {rotEff = -100;}

		// PLACEHOLDER - Read and scale ADC for translational effort
		int16_t transEff = 0;

		// Clamp translation effort
		if (transEff > 100) {transEff = 100;}
		else if (transEff < -100) {transEff = -100;}

		// Combine
		int16_t cmdEffL = transEff + rotEff;
		int16_t cmdEffR = transEff - rotEff;

		// Clamping total
		if (cmdEffL > 100) {cmdEffL = 100;}
		else if (cmdEffL < -100) {cmdEffL = -100;}

		if (cmdEffR > 100) {cmdEffR = 100;}
		else if (cmdEffR < -100) {cmdEffR = -100;}

		// Convert to 2's complement hex
		char hexEffL[4]; // Needs length 3 for null terminator
		char hexEffR[4];
		sprintf(hexEffL, "%02X", (uint8_t)cmdEffL);
		sprintf(hexEffR, "%02X", (uint8_t)cmdEffR);

		char mode = 'B'; // 'B' for Elastic Rotation Mode

		// Concatenate final command
		sprintf(command, "%c%s%s\r\n", mode, hexEffL, hexEffR);
		sendCommand(command);
	}

}

void FSM::sendCommand(char* msg) {
    // Sends command string over UART
    HAL_UART_Transmit(huart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}




