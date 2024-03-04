/*
 * UART.hpp
 *
 *  Created on: May 14, 2022
 *      Author: charpoank
 */

#ifndef SRC_UART_HPP_
#define SRC_UART_HPP_

#include "GPIO.hpp"
#include <cstddef>
#include "stm32f722xx.h"
#include "stm32f7xx_it.h"
#include "core_cm7.h"

namespace COM{

class UART {
public:
	UART(USART_TypeDef* uart, GPIO_TypeDef* tx, uint8_t tx_pin, uint8_t tx_afr ,
							  GPIO_TypeDef* rx, uint8_t rx_pin,	uint8_t rx_afr) :
							  uart(uart), tx(tx), tx_pin(tx_pin), tx_afr(tx_afr),
							  rx(rx), rx_pin(rx_pin), rx_afr(rx_afr){};

	void Init();
	void Transmit(const char *data, std::size_t size);
	void Interrupt_Handler() noexcept;

private:
	USART_TypeDef* uart;
	IRQn_Type IRQn;

	GPIO_TypeDef* tx, *rx;
	uint8_t tx_pin, rx_pin, tx_afr, rx_afr;

	enum class UART_Mode:uint8_t{
		TX,
		RX
	};
	UART_Mode mode;

	const char* tx_current;
	char* rx_current;

	const char* tx_end;
	char* rx_end;

	volatile bool done;
};

}
#endif /* SRC_UART_HPP_ */
