/*
 * UART.cpp
 *
 *  Created on: May 14, 2022
 *      Author: charpoank
 */

#include "UART.hpp"

namespace COM {
	void UART::Init(){
		if(uart == USART1){
			RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_USART1EN;
			IRQn = USART1_IRQn;
		}else if(uart == USART2){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_USART2EN;
			IRQn = USART2_IRQn;
		}else if(uart == USART3){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_USART3EN;
			IRQn = USART3_IRQn;
		}else if(uart == UART4){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_UART4EN;
			IRQn = UART4_IRQn;
		}else if(uart == UART5){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_UART5EN;
			IRQn = UART5_IRQn;
		}else if(uart == USART6){
			RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_USART6EN;
			IRQn = USART6_IRQn;
		}else if(uart == UART7){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_UART7EN;
			IRQn = UART7_IRQn;
		}else if(uart == UART8){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_UART8EN;
			IRQn = UART8_IRQn;
		}



		GPIO::EN(tx);
		GPIO::EN(rx);

		GPIO::SPEED(tx, tx_pin, SPEED_VHIGH);
		GPIO::SPEED(rx, rx_pin, SPEED_VHIGH);

		GPIO::MODE(tx, tx_pin, tx_afr);
		GPIO::MODE(rx, rx_pin, rx_afr);

		uart->BRR = 0x57E2;
		uart->CR1 = uart->CR1 | USART_CR1_TCIE;
		uart->CR1 = uart->CR1 | USART_CR1_TE;

		uart->CR1 = uart->CR1 | USART_CR1_UE;

		RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_SYSCFGEN;
		//NVIC_EnableIRQ(IRQn);
	}

	void UART::Transmit(const char *data, std::size_t size){
		/*done = false;
		mode = UART_Mode::TX;
		tx_current = data;
		rx_current = 0;

		tx_end = data + size;
		rx_end = reinterpret_cast<char*>(size);*/


		int i = 0;
		while(*data){
			rpt:
			if((uart->ISR&USART_ISR_TC)){
				uart->TDR = data[i];
				i++;
				data++;
			}else{
				goto rpt;
			}
		}

		//done = true;
		//while(!done);
	}

	void UART::Interrupt_Handler(){
		auto* tmp_tx_current = tx_current;
		auto* tmp_tx_end = tx_end;

		auto* tmp_rx_current = rx_current;
		auto* tmp_rx_end = rx_end;

		switch(mode){
		case UART_Mode::TX:
			while(((uart->ISR&USART_ISR_TXE_Msk) != USART_ISR_TXE_Msk) && tmp_tx_current < tmp_tx_end){
				*reinterpret_cast<volatile uint8_t*>(&uart->TDR) = *tmp_tx_current++;
			}
			break;

		case UART_Mode::RX:
			break;

		default:
			break;
		}

		if(tmp_tx_current == tmp_tx_end){
			done = true;
			return;
		}

		if(tmp_rx_current == tmp_rx_end){

		}

		tx_current = tmp_tx_current;
		tx_end = tmp_tx_end;
		rx_current = tmp_rx_current;
		rx_end = tmp_rx_end;
	}
}
