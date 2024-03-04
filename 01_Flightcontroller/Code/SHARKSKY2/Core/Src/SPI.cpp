/*
 * SPI.cpp
 *
 *  Created on: May 11, 2022
 *      Author: charpoank
 */

#include "SPI.hpp"

namespace COM {
	void SPI::Init(){
		if(spi == SPI1){
			RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_SPI1EN;
			IRQn = SPI1_IRQn;
		}else if(spi == SPI2){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_SPI2EN;
			IRQn = SPI2_IRQn;
		}else if(spi == SPI3){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_SPI3EN;
			IRQn = SPI3_IRQn;
		}else if(spi == SPI4){
			RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_SPI4EN;
			IRQn = SPI4_IRQn;
		}else if(spi == SPI5){
			RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_SPI5EN;
			IRQn = SPI5_IRQn;
		}

		spi->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_1 | SPI_CR1_SSI | SPI_CR1_SSM;
		spi->CR2 = SPI_CR2_DS_1 | SPI_CR2_FRXTH | SPI_CR2_RXNEIE;

		GPIO::EN(mosi);
		GPIO::EN(miso);
		GPIO::EN(sck);
		GPIO::EN(cs);

		GPIO::MODE(mosi, mosi_pin, MODE_ALT);
		GPIO::MODE(miso, miso_pin, MODE_ALT);
		GPIO::MODE(sck, sck_pin, MODE_ALT);
		GPIO::MODE(cs, cs_pin, MODE_OUTPUT);

		GPIO::PUPD(cs, cs_pin, PULLUP);

		GPIO::SPEED(mosi, mosi_pin, SPEED_VHIGH);
		GPIO::SPEED(miso, miso_pin, SPEED_VHIGH);
		GPIO::SPEED(sck, sck_pin, SPEED_VHIGH);
		GPIO::SPEED(cs, cs_pin, SPEED_VHIGH);

		GPIO::AFR(mosi, mosi_pin, mosi_afr);
		GPIO::AFR(miso, miso_pin, miso_afr);
		GPIO::AFR(sck, sck_pin, sck_afr);

		cs->BSRR = cs->BSRR | 1 << cs_pin;

		spi->CR1 = spi->CR1 | SPI_CR1_SPE;

		RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_SYSCFGEN;
		NVIC_EnableIRQ(IRQn);
	}


	void SPI::Transmit(const uint8_t *data, std::size_t size){
		done = false;
		mode = SPI_Mode::TX;
		tx_current = data;
		rx_current = 0;

		tx_end = data + size;
		rx_end = reinterpret_cast<uint8_t*>(size);

		__DMB();
		spi->CR2 = spi->CR2 | (SPI_CR2_TXEIE | SPI_CR2_RXNEIE);

		while(!done);
	}
	void SPI::Receive(uint8_t *data, std::size_t size){
		done = false;
		mode = SPI_Mode::RX;
		rx_current = data;
		tx_current = 0;

		rx_end = data + size;
		tx_end = reinterpret_cast<const uint8_t*>(size);

		__DMB();
		spi->CR2 = spi->CR2 | (SPI_CR2_TXEIE | SPI_CR2_RXNEIE);

		while(!done);
	}

	void SPI::TransmitReceive(uint8_t *txdata, uint8_t *rxdata , std::size_t size){
		done = false;
		mode = SPI_Mode::TRX;
		rx_current = rxdata;
		tx_current = txdata;

		rx_end = rxdata + size;
		tx_end = txdata + size;

		__DMB();
		spi->CR2 = spi->CR2 | (SPI_CR2_TXEIE | SPI_CR2_RXNEIE);

		while(!done);
	}

	void SPI::Interrupt_Handler(){
		auto* tmp_tx_current = tx_current;
		auto* tmp_tx_end = tx_end;

		auto* tmp_rx_current = rx_current;
		auto* tmp_rx_end = rx_end;
		switch(mode){
		case SPI_Mode::TX:
			while (((spi->SR&SPI_SR_FTLVL_Msk) != SPI_SR_FTLVL_Msk) && tmp_tx_current < tmp_tx_end){
				*reinterpret_cast<volatile uint8_t*>(&spi->DR) = *tmp_tx_current++;
			}
			while(((spi->SR)&(SPI_SR_RXNE_Msk)) && tmp_rx_current < tmp_rx_end){
				*reinterpret_cast<volatile uint8_t*>(&spi->DR);
				tmp_rx_current++;
			}
			break;

		case SPI_Mode::RX:
			while (((spi->SR&SPI_SR_FTLVL_Msk) != SPI_SR_FTLVL_Msk) && tmp_tx_current < tmp_tx_end){
				*reinterpret_cast<volatile uint8_t*>(&spi->DR) = 0;
				tmp_tx_current++;
			}
			while(((spi->SR)&(SPI_SR_RXNE_Msk)) && tmp_rx_current < tmp_rx_end){
				*tmp_rx_current++ = *reinterpret_cast<volatile uint8_t*>(&spi->DR);
			}
			break;

		default: //Mode_TRX
			while (((spi->SR&SPI_SR_FTLVL_Msk) != SPI_SR_FTLVL_Msk) && tmp_tx_current < tmp_tx_end){
				*reinterpret_cast<volatile uint8_t*>(&spi->DR) = *tmp_tx_current++;
			}
			while(((spi->SR)&(SPI_SR_RXNE_Msk)) && tmp_rx_current < tmp_rx_end){
				*tmp_rx_current++ = *reinterpret_cast<volatile uint8_t*>(&spi->DR);
			}
			break;
		}

		if(tmp_tx_current == tmp_tx_end){
			spi->CR2 = spi->CR2 & ~SPI_CR2_TXEIE;
		}

		if(tmp_rx_current == tmp_rx_end){
			spi->CR2 = spi->CR2 & ~SPI_CR2_RXNEIE;
			done = true;
			return;
		}

		tx_current = tmp_tx_current;
		tx_end = tmp_tx_end;
		rx_current = tmp_rx_current;
		rx_end = tmp_rx_end;
	}

	void SPI::CS_EN(){
		cs->BSRR = cs->BSRR | (1 << (cs_pin + 16));
	}

	void SPI::CS_DIS(){
		cs->BSRR = cs->BSRR | 1 << cs_pin;
	}

} /* namespace COM */
