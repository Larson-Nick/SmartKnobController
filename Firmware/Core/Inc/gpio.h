/*
 * gpio.h
 *
 *  Created on: Jun 4, 2025
 *      Author: nickl
 *
 *      UPDATED PROJECT VERSION
 *
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32f4xx_hal.h"

class GPIO {
public:
    GPIO(GPIO_TypeDef* port, uint16_t pin);

    void write(GPIO_PinState state) const;
    void high() const;
    void low() const;

private:
    GPIO_TypeDef* port;
    uint16_t pin;
};

#endif /* INC_GPIO_H_ */
