/*
 * gpio.cpp
 *
 *  Created on: Jun 4, 2025
 *      Author: nickl
 *
 *      UPDATED PROJECT VERSION
 *
 */

#include "gpio.h"

GPIO::GPIO(GPIO_TypeDef* port, uint16_t pin)
    : port(port), pin(pin)
{}

void GPIO::write(GPIO_PinState state) const {
    HAL_GPIO_WritePin(port, pin, state);
}

void GPIO::high() const {
    write(GPIO_PIN_SET);
}

void GPIO::low() const {
    write(GPIO_PIN_RESET);
}




