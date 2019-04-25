/*
 * I_SPI.cpp
 *
 *  Created on: 22.04.2019
 *      Author: Pluscrafter
 */

#include <I_SPI.h>



namespace Interface {

	I_SPI::I_SPI(SPI_HandleTypeDef &hspi) : spi(hspi) {

	}

	I_SPI::~I_SPI() {
		// TODO Auto-generated destructor stub
	}

	void I_SPI::Transfer(uint8_t msg){
		HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_RESET);
		HAL_SPI_Transmit_DMA(&spi, &msg, 1);
		while( spi.State == HAL_SPI_STATE_BUSY );
		HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET);
	}

	void I_SPI::Transfer(uint8_t msg[], uint8_t bytes){
		HAL_SPI_Transmit_DMA(&spi, msg, bytes);
	}

	uint8_t I_SPI::TransferReceive(uint8_t msg){
		uint8_t rcv;
		HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_RESET);
		HAL_SPI_Transmit_DMA(&spi,&msg,1);
		HAL_SPI_Receive_DMA(&spi,&rcv,1);
		while( spi.State == HAL_SPI_STATE_BUSY );
		HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET);
		return rcv;
	}

	uint8_t* I_SPI::TransferReceive(uint8_t msg[], uint8_t bytes){
		uint8_t rcv[bytes];
		HAL_SPI_TransmitReceive_DMA(&spi,msg,rcv,bytes);

		return rcv;
	}

	void I_SPI::WriteRegister(uint8_t reg, uint8_t val){
		uint8_t msg[2] = {reg, val};
		Transfer(msg,2);
	}

	void I_SPI::WriteRegisters(uint8_t reg[], uint8_t val[], uint8_t bytes){
		uint8_t msg[bytes*2];

		int i = 0, j = 0, k = 0;

		while (i < bytes*2){
			msg[i++] = reg[j++];
			msg[i++] = val[k++];
		}

		Transfer(msg, 2*bytes);
	}

	uint8_t I_SPI::ReadRegister(uint8_t reg){
		return TransferReceive(reg|0x80);
	}

	uint8_t* I_SPI::ReadRegisters(uint8_t reg[], uint8_t bytes){
		uint8_t regs[bytes];

		for (int i = 0; i < bytes; i++){
			regs[i] = reg[i] | 0x80;
		}

		return TransferReceive(regs, bytes);
	}

} /* namespace Interface */
