/*
 * ICM20689.cpp
 *
 *  Created on: 21.04.2019
 *      Author: Pluscrafter
 */

#include <ICM20689.h>

namespace Sensor {
	ICM20689::ICM20689() {
		// TODO Auto-generated constructor stub

	}

	ICM20689::~ICM20689() {
		// TODO Auto-generated destructor stub
	}

	void ICM20689::Initalize(){
		spi1.WriteRegister(PWR_MGMT_1, 0x40); // RESET IMU
		HAL_Delay(100);
		spi1.WriteRegister(USER_CTRL, 0x10); // DISABLE I2C
		HAL_Delay(10);
		spi1.WriteRegister(PWR_MGMT_1, 0x00);	// START IMU
		HAL_Delay(10);
		uint8_t whoami = spi1.ReadRegister(WHO_AM_I); //CONTROL ID
		if (whoami == 0x98){
			init_ok = true;
		}
		HAL_Delay(10);
		spi1.WriteRegister(CONFIG, 0x02); //set DLPF to 2 108.6 HZ BW
		HAL_Delay(10);
		spi1.WriteRegister(GYRO_CONFIG, 0x00); //set GYRO to 250dps
		HAL_Delay(10);
		spi1.WriteRegister(ACCEL_CONFIG, 0x00); //set ACCEL to 2g
		HAL_Delay(10);
		spi1.WriteRegister(ACCEL_CONFIG_2, 0x02); //set ACCEL DLPF to 121.3Hz
		HAL_Delay(10);
		spi1.WriteRegister(SMPLRT_DIV, 0x00); //no divider

	}

	void ICM20689::SelfTest(){

	}

	void ICM20689::SetGyroOffset(){

	}

	void ICM20689::SetAccelOffset(){

	}

	void ICM20689::DMPIntStatus(){

	}

	void ICM20689::IntStatus(){

	}

	void ICM20689::SigReset(){

	}

	void ICM20689::FIFOCount(){

	}

	void ICM20689::FIFORW(){

	}

	void ICM20689::ReadGyro(){
		uint8_t temp[6];
		uint8_t reg[6]={GYRO_XOUT_H,GYRO_XOUT_L,GYRO_YOUT_H,GYRO_YOUT_L,GYRO_ZOUT_H,GYRO_ZOUT_L};
		spi1.ReadRegisters(reg, temp, 6);

		r_gyro[0] = (temp[0] << 8) | temp[1];
		r_gyro[1] = (temp[2] << 8) | temp[3];
		r_gyro[2] = (temp[4] << 8) | temp[5];

		for(int i = 0; i<3; i++){
			ypr[i] =   r_gyro[i] / 131.0 ;
		}

	}

	void ICM20689::ReadAccel(){
		uint8_t temp[6];
		uint8_t reg[6]={ACCEL_XOUT_H,ACCEL_XOUT_L,ACCEL_YOUT_H,ACCEL_YOUT_L,ACCEL_ZOUT_H,ACCEL_ZOUT_L};
		spi1.ReadRegisters(reg, temp, 6);

		r_accel[0] = (temp[0] << 8) | temp[1];
		r_accel[1] = (temp[2] << 8) | temp[3];
		r_accel[2] = (temp[4] << 8) | temp[5];

		for(int i = 0; i<3; i++){
			accel[i] =  r_accel[i] / 16384.0;
		}

	}

	void ICM20689::ReadTemp(){

	}
} /* namespace Sensor */
