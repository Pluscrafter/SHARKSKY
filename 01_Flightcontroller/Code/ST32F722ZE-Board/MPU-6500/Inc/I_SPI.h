/*
 * I_SPI.h
 *
 *  Created on: 22.04.2019
 *      Author: Pluscrafter
 */

//https://visualgdb.com/tutorials/arm/stm32/spi/ 23.04.2019
//http://www.cs.cmu.edu/~cga/arduino/mpu6500-spi.ino 23.04.2019

#ifndef I_SPI_H_
#define I_SPI_H_

#include "stm32f7xx_hal.h"
#include "dma.h"
#include "spi.h"

namespace Interface {

class I_SPI {
public:
	I_SPI(SPI_HandleTypeDef&hspi);
	virtual ~I_SPI();

	SPI_HandleTypeDef &spi;

	void Start();

	void Transfer(uint8_t msg);
	void Transfer(uint8_t msg[], uint8_t bytes);

	uint8_t TransferReceive(uint8_t msg);
	void TransferReceive(uint8_t msg[],uint8_t rec[],uint8_t bytes);

	void WriteRegister(uint8_t reg, uint8_t val);
	void WriteRegisters(uint8_t reg[], uint8_t val[], uint8_t bytes);

	uint8_t ReadRegister(uint8_t reg);
	void ReadRegisters(uint8_t reg[],uint8_t rec[], uint8_t bytes);
};

} /* namespace Interface */

#endif /* I_SPI_H_ */
