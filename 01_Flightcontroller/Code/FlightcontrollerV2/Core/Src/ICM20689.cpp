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

	float ICM20689::fastsqrt(float val) { //https://bits.stephan-brumme.com/squareRoot.html
		unsigned int i = *(unsigned int*) &val;
		// adjust bias
		i  += 127 << 23;
		// approximation of square root
		i >>= 1;
		return *(float*) &i;
	}

	float ICM20689::fastsin(float val){ //sinus polynom
		return val - ((val*val*val)/6) + ((val*val*val*val*val)/120) - ((val*val*val*val*val*val*val)/5040);
	}

	float ICM20689::fastasin(float x){//https://en.wikipedia.org/wiki/Inverse_trigonometric_functions#Infinite_series
		return x+(0.5)*((x*x*x)/3)+(0.375)*((x*x*x*x*x)/5)+(0.3125)*((x*x*x*x*x*x*x)/7);
	}

	void ICM20689::calculateICM(){
		  fullvec = fastsqrt(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]); //calculate full vector with Pythagoras' theorem
		  if(fullvec == 0) {
			  fullvec = 1;
		  }

		  acangle[0] = fastasin(accel[0]/fullvec) * -57.29577951;	//calculate acceleration sin-1(angle/fullvec)
		  acangle[1] = fastasin(accel[1]/fullvec) * 57.29577951;

		  //calculate angle with angular motion
		  t_ypr[0] += ypr[0]*lptime;
		  t_ypr[1] += ypr[1]*lptime;
		  t_ypr[2] += ypr[2]*lptime;


		  alpha2 = (DLPF_FREQ_YAW * lptime) / (1 + DLPF_FREQ_YAW * lptime);
		  f_ypr[2] = f_ypr[2] - (alpha2 * (f_ypr[2] - ypr[2]));

		  //roll and pitch tuning on yaw movement https://www.youtube.com/watch?v=4BoIE8YQwM8 17.10.2019
		  t_ypr[0] -= t_ypr[1] * fastsin(ypr[2] * 0.017453293 * lptime);
		  t_ypr[1] += t_ypr[0] * fastsin(ypr[2] * 0.017453293 * lptime);

		  //complementary filter
		  t_ypr[0] = t_ypr[0] * 0.9996 + acangle[1] * 0.0004;	// angle is mixed up
		  t_ypr[1] = t_ypr[1] * 0.9996 + acangle[0] * 0.0004;

		  //is nan error
		  if (isnan(t_ypr[0])){
			  t_ypr[0] = 0;
		  }

		  if (isnan(t_ypr[1])){
			  t_ypr[1] = 0;
		  }

		  if (t_ypr[0] < -60){
			  t_ypr[0] = -60;
		  }

		  if (t_ypr[1] < -60){
			  t_ypr[1] = -60;
		  }

		  if (t_ypr[0] > 60){
			  t_ypr[0] = 60;
		  }

		  if (t_ypr[1] > 60){
			  t_ypr[1] = 60;
		  }



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
