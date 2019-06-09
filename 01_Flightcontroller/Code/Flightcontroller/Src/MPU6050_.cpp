/*
 * MPU6050.cpp
 *
 *  Created on: 29.01.2019
 *      Author: Pluscrafter
 */

#include <MPU6050_.h>

namespace Sensors {

uint16_t MPU6050::address;
bool MPU6050::IntEnable;

bool MPU6050::dataRDY = false;

uint16_t MPU6050::rawGYRO[3] ,MPU6050::rawACCL[3];
uint16_t  MPU6050::errorDL=0, MPU6050::DL=0, MPU6050::peak=0;
uint8_t MPU6050::TxData[6];

MPU6050::MPU6050(uint16_t address, bool IntEnable) {
	this->address = address << 0x01;
	this->IntEnable = IntEnable;


}

MPU6050::~MPU6050() {
	// TODO Auto-generated destructor stub
}


void MPU6050::Init(){
	//reset Sensor
	uint8_t buffer[3] = {0x80,0x07,0x00};
	HAL_I2C_Mem_Write(&hi2c1,address,PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&buffer[0], 1,DELAY);
	HAL_Delay(50);
	HAL_I2C_Mem_Write(&hi2c1,address,SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&buffer[1], 1,DELAY);
	HAL_Delay(50);
	HAL_I2C_Mem_Write(&hi2c1,address,PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&buffer[2], 1,DELAY);
	HAL_Delay(50);
	//config
	Config();
	HAL_Delay(50);
	//set res to 500�/s
	SetGyroResolution();
	HAL_Delay(50);
	//set res to 4g/s
	SetACCLResolution();
	HAL_Delay(50);
}

void MPU6050::Config(){
	//Set DigitalLowPassFilter
	uint8_t buffer[2] ={ DLPF_CFG_260, DATA_RDY_EN };
	HAL_I2C_Mem_Write(&hi2c1,address,CONFIG, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&buffer[0], 1,DELAY);
	HAL_I2C_Mem_Write(&hi2c1,address,INT_ENABLE, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&buffer[1], 1,DELAY);

}

void MPU6050::SetGyroResolution(){
	uint8_t buffer = FS_SEL_500<<3;
	HAL_I2C_Mem_Write(&hi2c1,address,GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&buffer, 1,DELAY);
}

void MPU6050::SetACCLResolution(){
	uint8_t buffer = AFS_SEL_4<<3;
	HAL_I2C_Mem_Write(&hi2c1,address,ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&buffer, 1,DELAY);
}

void MPU6050::readGyroRAW(){
	uint8_t buffer[6];

	if(HAL_I2C_Mem_Read(&hi2c1,address,GYRO_XOUT_L,I2C_MEMADD_SIZE_8BIT,buffer,6,DELAY) != HAL_OK){
		MX_I2C1_Init();
	}

	DL++;

	rawGYRO[0] = buffer[1] << 8|buffer[0];
	rawGYRO[1] = buffer[3] << 8|buffer[2];
	rawGYRO[2] = buffer[5] << 8|buffer[4];

}

void MPU6050::readAcclRAW(){
	uint8_t buffer[6];

	HAL_I2C_Mem_Read(&hi2c1,address,ACCEL_XOUT_L,I2C_MEMADD_SIZE_8BIT,buffer,6,DELAY);

	rawACCL[0] = buffer[1] << 8|buffer[0];
	rawACCL[1] = buffer[3] << 8|buffer[2];
	rawACCL[2] = buffer[5] << 8|buffer[4];
}

} /* namespace Sensors */
