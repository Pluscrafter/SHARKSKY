/*
 * ICM20689.cpp
 *
 *  Created on: 24.12.2019
 *      Author: Pluscrafter
 */

#include "ICM20689.h"

namespace SENSOR {

	ICM20689::ICM20689(SPI_HandleTypeDef&hspi , GPIO_TypeDef *GPIOx, uint8_t pin) :
			spi(hspi), GPIOx(GPIOx), pin(pin) {

	}

	ICM20689::~ICM20689() {
		// TODO Auto-generated destructor stub
	}

	bool ICM20689::initICM(){

		writeRegister(PWR_MGMT_1, 0x40, 100);		// RESET IMU
		writeRegister(USER_CTRL, 0x10);				// DISABLE I2C
		writeRegister(PWR_MGMT_1, 0x00);			// START IMU

		if(!whoami()){
			return false;
		}

		writeRegister(CONFIG, 0x02);				//set DLPF to 2 108.6 HZ BW
		writeRegister(GYRO_CONFIG, 0x08);			//set GYRO to 500dps 08
		writeRegister(ACCEL_CONFIG, 0x10);			//set ACCEL to 8g
		writeRegister(ACCEL_CONFIG_2, 0x02);		//set ACCEL DLPF to 121.3Hz
		writeRegister(SMPLRT_DIV, 0x00);			//no divider
		//writeRegister(INT_PIN_CFG, 0x00);			//config Interrupt <- not used Tim based Interrupt
		//writeRegister(INT_ENABLE, 0x01);			//enable Interrupt <- not used Tim based Interrupt

		initOK = true;

		return true;
	}

	bool ICM20689::whoami(){

		if(readRegister(WHO_AM_I) != 0x98){
			return false;
		}

		return true;
	}

	void ICM20689::writeRegister(uint8_t reg, uint8_t value){
		uint8_t tmp[2];

		tmp[0] = reg;
		tmp[1] = value;

		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&spi, tmp, 2, HAL_MAX_DELAY);
		while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_SET);
		HAL_Delay(10);
	}

	void ICM20689::writeRegister(uint8_t reg, uint8_t value, uint8_t delay){
		uint8_t tmp[2];

		tmp[0] = reg;
		tmp[1] = value;

		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&spi, tmp, 2, HAL_MAX_DELAY);
		while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_SET);
		HAL_Delay(delay);
	}

	uint8_t ICM20689::readRegister(uint8_t reg){
		uint8_t tmp[1];

		tmp[0] = reg|0x80;
		uint8_t ret[1] ;
		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&spi,(uint8_t *)tmp, 1, HAL_MAX_DELAY);
		HAL_SPI_Receive(&spi, (uint8_t *)ret, 1, HAL_MAX_DELAY);
		while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_SET);
		HAL_Delay(10);

		return ret[0];
	}

	uint8_t ICM20689::readRegister(uint8_t reg, uint8_t delay){
		uint8_t tmp[1];

		tmp[0] = reg|0x80;
		uint8_t ret[1] ;
		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&spi,(uint8_t *)tmp, 1, HAL_MAX_DELAY);
		HAL_SPI_Receive(&spi, (uint8_t *)ret, 1, HAL_MAX_DELAY);
		while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY);
		HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_SET);
		HAL_Delay(delay);

		return ret[0];
	}

} /* namespace SENSOR */
