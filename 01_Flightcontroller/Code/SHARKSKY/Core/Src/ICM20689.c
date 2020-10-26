/*
 * ICM20689.c
 *
 *  Created on: Aug 29, 2020
 *      Author: Pluscrafter
 */

#include "ICM20689.h"

extern float looptime;

void IMU_writeRegister(uint8_t reg, uint8_t value){
	uint8_t tmp[2];

	tmp[0] = reg;
	tmp[1] = value;

	HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, tmp, 2, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}

void _IMU_writeRegister(uint8_t reg, uint8_t value, uint8_t delay){
	uint8_t tmp[2];

	tmp[0] = reg;
	tmp[1] = value;

	HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, tmp, 2, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_SET);
	HAL_Delay(delay);
}

uint8_t IMU_readRegister(uint8_t reg){
	uint8_t tmp[1];

	tmp[0] = reg|0x80;
	uint8_t ret[1] ;
	HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,(uint8_t *)tmp, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi3, (uint8_t *)ret, 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	return ret[0];
}

uint8_t _IMU_readRegister(uint8_t reg, uint8_t delay){
	uint8_t tmp[1];

	tmp[0] = reg|0x80;
	uint8_t ret[1] ;
	HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,(uint8_t *)tmp, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi3, (uint8_t *)ret, 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_SET);
	HAL_Delay(delay);

	return ret[0];
}

bool IMU_whoami(){

	if(IMU_readRegister(WHO_AM_I) != 0x98){
		return false;
	}

	return true;
}

bool IMU_Init(){
	_IMU_writeRegister(PWR_MGMT_1, 0x40, 100);		// RESET IMU
	IMU_writeRegister(USER_CTRL, 0x10);				// DISABLE I2C
	IMU_writeRegister(PWR_MGMT_1, 0x00);			// START IMU

	if(!IMU_whoami()){
		return false;
	}

	IMU_writeRegister(CONFIG, 0x02);				//set DLPF to 2 108.6 HZ BW
	IMU_writeRegister(GYRO_CONFIG, 0x08);			//set GYRO to 500dps 08
	IMU_writeRegister(ACCEL_CONFIG, 0x10);			//set ACCEL to 8g
	IMU_writeRegister(ACCEL_CONFIG_2, 0x02);		//set ACCEL DLPF to 121.3Hz
	IMU_writeRegister(SMPLRT_DIV, 0x00);			//no divider
	IMU_writeRegister(INT_PIN_CFG, 0x00);			//config Interrupt <- not used Tim based Interrupt
	IMU_writeRegister(INT_ENABLE, 0x01);			//enable Interrupt <- not used Tim based Interrupt

	return true;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){
	//read IMU GYRO ACCEL alternating

}

void IMU_calcAngle(){
	IMU_calcAccelAngle();
	IMU_calcGyroAngle();

	IMU_ComplementaryFilter();
	IMU_DLPF();
}

void IMU_calcAccelAngle(){
	IMU_fullvec = ff_fastSqrt(IMU_accel[0]*IMU_accel[0] + IMU_accel[1]*IMU_accel[1] + IMU_accel[2]*IMU_accel[2]);
	if(IMU_fullvec == 0) {
		IMU_fullvec = 1;
	}

	IMU_acclAngle[0] = ff_fastASin(IMU_accel[0]/IMU_fullvec) * -57.29577951;
	IMU_acclAngle[1] = ff_fastASin(IMU_accel[1]/IMU_fullvec) * 57.29577951;

}

void IMU_calcGyroAngle(){
	IMU_t_ypr[0] += IMU_ypr[0] * looptime;
	IMU_t_ypr[1] += IMU_ypr[1] * looptime;
	IMU_t_ypr[2] += IMU_ypr[2] * looptime;
}

void IMU_ComplementaryFilter(){
	IMU_t_ypr[0] = IMU_t_ypr[0] * 0.9996 + IMU_acclAngle[1] * 0.0004;	// angle is mixed up WHY I DON'T KNOW?
	IMU_t_ypr[1] = IMU_t_ypr[1] * 0.9996 + IMU_acclAngle[0] * 0.0004;
}

void IMU_DLPF(){
	IMU_alpha2_DLPF = (DLPF_YAW * looptime) / (1 + DLPF_YAW * looptime);
	IMU_f_ypr[2] = IMU_f_ypr[2] + (IMU_alpha2_DLPF * (IMU_ypr[2] - IMU_f_ypr[2]));

	IMU_alpha_DLPF = (DLPF_PITCHROLL * looptime) / (1 + DLPF_PITCHROLL * looptime);
	IMU_f_ypr[0] = IMU_f_ypr[0] + (IMU_alpha_DLPF * (IMU_t_ypr[0] - IMU_f_ypr[0]));
	IMU_f_ypr[1] = IMU_f_ypr[1] + (IMU_alpha_DLPF * (IMU_t_ypr[1] - IMU_f_ypr[1]));

	//roll and pitch tuning on yaw movement https://www.youtube.com/watch?v=4BoIE8YQwM8 17.10.2019
	IMU_f_ypr[0] -= IMU_f_ypr[1] * ff_fastSin(IMU_f_ypr[2] * 0.017453293 * looptime);
	IMU_f_ypr[1] += IMU_f_ypr[0] * ff_fastSin(IMU_f_ypr[2] * 0.017453293 * looptime);

}
