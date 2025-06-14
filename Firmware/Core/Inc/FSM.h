/*
 * FSM.h
 *
 *  Created on: Jun 8, 2025
 *      Author: nickl
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "hall.h"

#include "stm32f4xx_hal.h"

class FSM {
public:
    enum class Mode {
        Elastic,
        FreeSpin
    };

    enum class Function {
        Translate,
        Rotate
    };

    FSM(Motor& motor, Encoder& encoder, PID& pid, Hall& hall, UART_HandleTypeDef* huart);

    void run(uint32_t freq);
    void advanceState(bool longPress);
    //void readAdc();

private:
    Motor& motor;
	Encoder& encoder;
	PID& pid;
	Hall& hall;
	UART_HandleTypeDef* huart;

    int stateIndex;
    Mode currentMode;
    Function currentFunction;
    uint32_t prevT;

    char command[50];  // Buffer for formatting UART output

    void stateFreeSpin();
	void stateElastic();
	void sendCommand(char* msg);
};

#endif /* INC_FSM_H_ */
