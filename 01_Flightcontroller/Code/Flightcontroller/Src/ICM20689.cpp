/*
 * ICM20689.cpp
 *
 *  Created on: 21.04.2019
 *      Author: Pluscrafter
 */

#include "ICM20689.h"

namespace Sensor {
	ICM20689::ICM20689(){

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
		uint8_t whoami = spi1.ReadRegister(WHO_AM_I|0x80); //CONTROL ID
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

		SetGyroOffset();

	}

	void ICM20689::SelfTest(){

	}

	void ICM20689::SetGyroOffset(){
		HAL_Delay(100);
		setXGyroOffset(OFFSET_GYRO_X);
		HAL_Delay(100);
		setYGyroOffset(OFFSET_GYRO_Y);
		HAL_Delay(100);
		setZGyroOffset(OFFSET_GYRO_Z);
		HAL_Delay(100);
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
		uint8_t tmp[6];
		uint8_t reg[6]={GYRO_XOUT_H|0x80,GYRO_XOUT_L|0x80,GYRO_YOUT_H|0x80,GYRO_YOUT_L|0x80,GYRO_ZOUT_H|0x80,GYRO_ZOUT_L|0x80};
		spi1.ReadRegisters(reg, tmp, 6);

		r_gyro[0] = (tmp[0] << 8) | tmp[1];
		r_gyro[1] = (tmp[2] << 8) | tmp[3];
		r_gyro[2] = (tmp[4] << 8) | tmp[5];

		for(int i = 0; i<3; i++){
			ypr[i] =   r_gyro[i] / 131.0 ;
		}

	}

	void ICM20689::ReadAccel(){
		uint8_t tmp[6];
		uint8_t reg[6]={ACCEL_XOUT_H|0x80,ACCEL_XOUT_L|0x80,ACCEL_YOUT_H|0x80,ACCEL_YOUT_L|0x80,ACCEL_ZOUT_H|0x80,ACCEL_ZOUT_L|0x80};
		spi1.ReadRegisters(reg, tmp, 6);

		r_accel[0] = (tmp[0] << 8) | tmp[1];
		r_accel[1] = (tmp[2] << 8) | tmp[3];
		r_accel[2] = (tmp[4] << 8) | tmp[5];

		int16_t a = r_accel[1];
		r_accel[1] = r_accel[2];
		r_accel[2] = a;

		for(int i = 0; i<3; i++){
			accel[i] =  r_accel[i] / 16384.0;
		}



	}

	void ICM20689::ReadTemp(){
		uint8_t tmp[2];
		uint8_t reg[2];

		spi1.ReadRegisters(reg, tmp, 2);

		r_temp = (tmp[0] << 8) | tmp[1];

		temp = ((r_temp - TEMP_OFFSET) / 326.8) + 25;
	}

//from I2cdevlib Calib
	void ICM20689::FindOffset(){
		//InitalizeCalibration();
		for (int i = iGx; i <= iGz; i++)
		  { // set targets and initial guesses
			Target[i] = 0; // must fix for ZAccel
			HighOffset[i] = 0;
			LowOffset[i] = 0;
		  } // set targets and initial guesses
		//Target[iAz] = 16384;
		SetAveraging(NFast);

		PullBracketsOut();
		PullBracketsIn();

		HAL_UART_Transmit(&huart1, (uint8_t *)"-------------- done --------------", sizeof("-------------- done --------------"), 100);

	}

	void ICM20689::ForceHeader(){
		LinesOut = 99;
	}

	void ICM20689::GetSmoothed(){
		int16_t RawValue[3];
		int i;
		long Sums[3];
		for (i = iGx; i <= iGz; i++){
			Sums[i] = 0;
		}

		for (i = 1; i <= N; i++)
		{ // get sums
			getMotion3(RawValue[iGx], RawValue[iGy], RawValue[iGz]);
			if ((i % 500) == 0)
				HAL_UART_Transmit(&huart1, (uint8_t *)".",sizeof("."),100);

			HAL_Delay(5);

			for (int j = iGx; j <= iGz; j++)
			  Sums[j] = Sums[j] + RawValue[j];
		} // get sums

		for (i = iGx; i <= iGz; i++){
			Smoothed[i] = (Sums[i] + N/2) / N ;
		}
	} // GetSmoothed

	void ICM20689::SetOffsets(int16_t TheOffsets[3]){
	    setXGyroOffset (TheOffsets [iGx]);
	    setYGyroOffset (TheOffsets [iGy]);
	    setZGyroOffset (TheOffsets [iGz]);
	}

	void ICM20689::ShowProgress(){
		if (LinesOut >= LinesBetweenHeaders){
			// show header
			HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", sizeof("\r\n"),100);
			HAL_UART_Transmit(&huart1, (uint8_t *)"\tXGyro\t\t\tYGyro\t\t\tZGyro\r\n",sizeof("\tXGyro\t\t\tYGyro\t\t\tZGyro\r\n"),100);
			LinesOut = 0;

		} // show header

		HAL_UART_Transmit(&huart1, (uint8_t *)" ",sizeof(" "),100);
		HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", sizeof("\r\n"),100);
		for (int i = iGx; i <= iGz; i++){
			char txt[32];
			HAL_UART_Transmit(&huart1, (uint8_t *)"[",sizeof("["),100);
			HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "%i", LowOffset[i]),100);
			HAL_UART_Transmit(&huart1, (uint8_t *)",",sizeof(","),100);
			HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "%i", HighOffset[i]),100);
			HAL_UART_Transmit(&huart1, (uint8_t *)"] --> [",sizeof("] --> ["),100);
			HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "%i", LowValue[i]),100);
			HAL_UART_Transmit(&huart1, (uint8_t *)",",sizeof(","),100);
			HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "%i", HighValue[i]),100);
			if (i == iGz){
				HAL_UART_Transmit(&huart1, (uint8_t *)"]",sizeof("]"),100);
			}
			else{
				HAL_UART_Transmit(&huart1, (uint8_t *)"]\t", sizeof("]\t"),100);
			}
		}
		HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", sizeof("\r\n"),100);
		LinesOut++;
	} // ShowProgress

	void ICM20689::PullBracketsIn(){
		bool AllBracketsNarrow;
		bool StillWorking;
		int16_t NewOffset[3];

		HAL_UART_Transmit(&huart1, (uint8_t *)"\nclosing in:", sizeof("\nclosing in:"),100);
		AllBracketsNarrow = false;
		ForceHeader();
		StillWorking = true;

		while (StillWorking) {
			StillWorking = false;

			if (AllBracketsNarrow && (N == NFast)){
				SetAveraging(NSlow);
			}

			else{
				AllBracketsNarrow = true;
			}// tentative

			for (int i = iGx; i <= iGz; i++){
				if (HighOffset[i] <= (LowOffset[i]+1)){
					NewOffset[i] = LowOffset[i];
				}
				else{ // binary search
					StillWorking = true;
					NewOffset[i] = (LowOffset[i] + HighOffset[i]) / 2;
					if (HighOffset[i] > (LowOffset[i] + 10)){
						AllBracketsNarrow = false;
					}
				} // binary search
			}

			SetOffsets(NewOffset);
			GetSmoothed();
			for (int i = iGx; i <= iGz; i++){ // closing in
				if (Smoothed[i] > Target[i]){
					// use lower half
					HighOffset[i] = NewOffset[i];
					HighValue[i] = Smoothed[i];
				} // use lower half
				else{
					// use upper half
					LowOffset[i] = NewOffset[i];
					LowValue[i] = Smoothed[i];
				} // use upper half
			} // closing in
			ShowProgress();
		} // still working

	} // PullBracketsIn

	void ICM20689::PullBracketsOut(){
		bool Done = false;
		int NextLowOffset[3];
		int NextHighOffset[3];

		HAL_UART_Transmit(&huart1, (uint8_t *)"expanding:\r\n",sizeof("expanding:\r\n"),100);
		ForceHeader();

		while (!Done){
			Done = true;
			SetOffsets(LowOffset);
			GetSmoothed();

			for (int i = iGx; i <= iGz; i++){
				// got low values
				LowValue[i] = Smoothed[i];
				if (LowValue[i] >= Target[i]){
					Done = false;
					NextLowOffset[i] = LowOffset[i] - 1000;
				}
				else{
					NextLowOffset[i] = LowOffset[i];
				}
			} // got low values

			SetOffsets(HighOffset);
			GetSmoothed();

			for (int i = iGx; i <= iGz; i++){
				// got high values
				HighValue[i] = Smoothed[i];
				if (HighValue[i] <= Target[i]){
					Done = false;
					NextHighOffset[i] = HighOffset[i] + 1000;
				}
				else{
					NextHighOffset[i] = HighOffset[i];
				}
			} // got high values

			ShowProgress();

			for (int i = iGx; i <= iGz; i++){
				LowOffset[i] = NextLowOffset[i];   // had to wait until ShowProgress done
				HighOffset[i] = NextHighOffset[i]; // ..
			}
		} // keep going
	} // PullBracketsOut

	void ICM20689::SetAveraging(int NewN){
		N = NewN;
		char txt[32];
		HAL_UART_Transmit(&huart1, (uint8_t *)"averaging ", sizeof("averaging "),100);
		HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "%i", N),100);
		HAL_UART_Transmit(&huart1, (uint8_t *)" readings each time", sizeof(" readings each time"),100);
	} // SetAveraging

	void ICM20689::getMotion3(int16_t &gx, int16_t &gy, int16_t &gz){
		uint8_t buffer[6];
		uint8_t reg[6] = {GYRO_XOUT_H|0x80,GYRO_XOUT_L|0x80,GYRO_YOUT_H|0x80,GYRO_YOUT_L|0x80,GYRO_ZOUT_H|0x80,GYRO_ZOUT_L|0x80};

		spi1.ReadRegisters(reg, buffer, 6);
		gx = (((int16_t)buffer[0]) << 8) | buffer[1];
		gy = (((int16_t)buffer[2]) << 8) | buffer[3];
		gz = (((int16_t)buffer[4]) << 8) | buffer[5];
	}

	void ICM20689::setXGyroOffset(int16_t offs){
		uint8_t regs[2] = {XG_OFFS_USRH,XG_OFFS_USRL};
		uint8_t highval = (uint8_t)(offs >> 8) & 0xFF, lowval =  (uint8_t)offs & 0xFF;
		uint8_t val[2] = {highval, lowval};
		spi1.WriteRegister(regs[0], val[0]);
		spi1.WriteRegister(regs[1], val[1]);
	}

	void ICM20689::setYGyroOffset(int16_t offs){
		uint8_t regs[2] = {YG_OFFS_USRH,YG_OFFS_USRL};
		uint8_t highval = (uint8_t)(offs >> 8) & 0xFF, lowval =  (uint8_t)offs & 0xFF;
		uint8_t val[2] = {highval, lowval};
		spi1.WriteRegister(regs[0], val[0]);
		spi1.WriteRegister(regs[1], val[1]);
	}

	void ICM20689::setZGyroOffset(int16_t offs){
		uint8_t regs[2] = {ZG_OFFS_USRH,ZG_OFFS_USRL};
		uint8_t highval = (uint8_t)(offs >> 8) & 0xFF, lowval =  (uint8_t)offs & 0xFF;
		uint8_t val[2] = {highval, lowval};
		spi1.WriteRegister(regs[0], val[0]);
		spi1.WriteRegister(regs[1], val[1]);
	}

} /* namespace Sensor */
