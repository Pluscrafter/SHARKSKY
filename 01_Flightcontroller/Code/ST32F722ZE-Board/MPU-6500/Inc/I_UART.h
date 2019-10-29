/*
 * I_UART.h
 *
 *  Created on: 22.04.2019
 *      Author: Pluscrafter
 */

#ifndef I_UART_H_
#define I_UART_H_

#include "stm32f7xx_hal.h"
#include "dma.h"
#include "usart.h"

namespace Interface {

class I_UART {
public:
	I_UART();
	virtual ~I_UART();
};

} /* namespace Interface */

#endif /* I_UART_H_ */
