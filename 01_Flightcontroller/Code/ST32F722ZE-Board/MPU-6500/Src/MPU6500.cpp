/*
 * MPU6500.cpp
 *
 *  Created on: 21.04.2019
 *      Author: Pluscrafter
 */

#include "MPU6500.h"

namespace Sensor {

	MPU6500::MPU6500() {
	// TODO Auto-generated constructor stub

	}

	MPU6500::~MPU6500() {
	// TODO Auto-generated destructor stub
	}

	void MPU6500::Initalize(){
		PWRMgmt();
		SigReset();
		UserCtl();

		HAL_Delay(1);
		id = spi1.ReadRegister(WHO_AM_I);
		if(id == 0x70) {
			HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
		}
		HAL_Delay(1);
	}

	void MPU6500::SelfTest(){

	}

	void MPU6500::SetGyroOffset(){

	}

	void MPU6500::SetAccelOffset(){

	}

	void MPU6500::DMPIntStatus(){

	}

	void MPU6500::IntStatus(){

	}

	void MPU6500::SigReset(){
		spi1.WriteRegister(SIGNAL_PATH_RESET,0x05);
		HAL_Delay(100);
	}

	void MPU6500::FIFOCount(){

	}

	void MPU6500::FIFORW(){

	}

	void MPU6500::ReadGyro(){

	}

	void MPU6500::ReadAccel(){

	}

	void MPU6500::ReadTemp(){

	}

	void MPU6500::Config(){

	}

	void MPU6500::ConfigGyro(){

	}

	void MPU6500::ConfigAccel(){

	}

	void MPU6500::FIFOEnable(){

	}

	void MPU6500::IntConfig(){

	}

	void MPU6500::IntEnable(){

	}

	void MPU6500::AccelIntCtr(){

	}

	void MPU6500::UserCtl(){
		spi1.WriteRegister(USER_CTRL, 0x10|0x08|0x04|0x01);
		HAL_Delay(100);
	}

	void MPU6500::PWRMgmt(){
		spi1.WriteRegister(PWR_MGMT_1,0x80);
		HAL_Delay(100);
	}

	void MPU6500::WhoAmI(){

	}

} /* namespace Sensor */
