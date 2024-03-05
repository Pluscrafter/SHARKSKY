/*
 * ICM20689.c
 *
 *  Created on: Aug 29, 2020
 *      Author: Pluscrafter
 */

#include "ICM20689.h"

extern float looptime;

namespace SENSORS{
	float IMU::filter_taps[FILTER_TAP_NUM] = {
			0.0025103462394326925,0.0027449678163975477,0.0032963049598038197,0.004180965945124626,0.005406032782047987,0.00696849450469017,0.008854987099766731,0.011041865684092045,0.013495598919689655,0.01617349497973919,0.019024722278118134,0.021991625428199768,0.025011250749230385,0.028017103672027588,0.030940992757678032,0.033715009689331055,0.03627347573637962,0.03855488449335098,0.04050370678305626,0.04207203909754753,0.04322106018662453,0.04392216354608536,0.04415782541036606,0.04392216354608536,0.04322106018662453,0.04207203909754753,0.04050370678305626,0.03855488449335098,0.03627347573637962,0.033715009689331055,0.030940992757678032,0.028017103672027588,0.025011250749230385,0.021991625428199768,0.019024722278118134,0.01617349497973919,0.013495598919689655,0.011041865684092045,0.008854987099766731,0.00696849450469017,0.005406032782047987,0.004180965945124626,0.0032963049598038197,0.0027449678163975477,0.0025103462394326925
	};


	void IMU::IMU_writeRegister(uint8_t reg, uint8_t value){
		uint8_t tmp[2];

		tmp[0] = reg;
		tmp[1] = value;

		spi3.CS_EN();
		spi3.Transmit(tmp, 2);
		spi3.CS_DIS();
	}

	void IMU::_IMU_writeRegister(uint8_t reg, uint8_t value, uint8_t delay){
		uint8_t tmp[2];

		tmp[0] = reg;
		tmp[1] = value;

		spi3.CS_EN();
		spi3.Transmit(tmp, 2);
		spi3.CS_DIS();
		//HAL_Delay(delay);

	}

	uint8_t IMU::IMU_readRegister(uint8_t reg){
		uint8_t tmp[1];

		tmp[0] = reg|0x80;
		uint8_t ret[1];

		spi3.CS_EN();
		spi3.Transmit(tmp, 1);
		spi3.Receive(ret, 1);
		spi3.CS_DIS();

		return ret[0];
	}

	uint8_t IMU::IMU_readRegisters(uint8_t reg, uint8_t ret[], uint8_t buf){
		uint8_t tmp[1];

		tmp[0] = reg|0x80;

		spi3.CS_EN();
		spi3.Transmit(tmp, 1);
		spi3.Receive(ret, buf);
		spi3.CS_DIS();

		return 0;
	}

	uint8_t IMU::_IMU_readRegister(uint8_t reg, uint8_t delay){
		uint8_t tmp[1];

		tmp[0] = reg|0x80;
		uint8_t ret[1];

		spi3.CS_EN();
		spi3.Transmit(tmp, 1);
		spi3.Receive(ret, 1);
		spi3.CS_DIS();
		//HAL_Delay(delay);

		return ret[0];
	}

	bool IMU::IMU_whoami(){

		if(IMU_readRegister(WHO_AM_I) != 0x98){
			return false;
		}

		return true;
	}

	bool IMU::IMU_Init(){
		IMU_writeRegister(PWR_MGMT_1, 0x40);		// RESET IMU
		IMU_writeRegister(USER_CTRL, 0x10);				// DISABLE I2C
		IMU_writeRegister(PWR_MGMT_1, 0x00);			// START IMU

		if(!IMU_whoami()){
			return false;
		}

		IMU_writeRegister(CONFIG, 0x00);				//set DLPF to 0 306.6 HZ BW
		IMU_writeRegister(GYRO_CONFIG, 0x0C);			//set GYRO to 500dps 08, DISABLE DLPF
		IMU_writeRegister(ACCEL_CONFIG, 0x10);			//set ACCEL to 8g
		IMU_writeRegister(ACCEL_CONFIG_2, 0x08);		//set ACCEL DLPF to 235.0Hz, DISABLE DLPF
		IMU_writeRegister(SMPLRT_DIV, 0x00);			//no divider
		IMU_writeRegister(INT_PIN_CFG, 0x00);			//config Interrupt <- not used Tim based Interrupt
		IMU_writeRegister(INT_ENABLE, 0x01);			//enable Interrupt <- not used Tim based Interrupt

		FIRFilter_Init(&IMU_FIR_Y, filter_taps, IMU_BUF_FIR_Y, FILTER_TAP_NUM);
		FIRFilter_Init(&IMU_FIR_P, filter_taps, IMU_BUF_FIR_P, FILTER_TAP_NUM);
		FIRFilter_Init(&IMU_FIR_R, filter_taps, IMU_BUF_FIR_R, FILTER_TAP_NUM);

		return IMU_whoami();
	}

	void IMU::IMU_startINT(){
		GPIO::EN(GPIOB);
		GPIO::MODE(GPIOB, 9, MODE_INPUT);
		GPIO::PUPD(GPIOB, 9, PULLDOWN);
		GPIO::INTERRUPT_INIT(GPIOB, 9);
		EXTI->IMR = EXTI->IMR | (1 << 9);
		EXTI->RTSR = EXTI->RTSR | (1 << 9);
		EXTI->FTSR = EXTI->FTSR & ~(1 << 9);
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	}

	void IMU::IMU_readAngle(){
		IMU_readRegisters(ACCEL_XOUT_H, IMU_BUF, 14);

		IMU_r_gyro[0] = (IMU_BUF[8] << 8) | IMU_BUF[9];
		IMU_r_gyro[1] = (IMU_BUF[10] << 8) | IMU_BUF[11];
		IMU_r_gyro[2] = (IMU_BUF[12] << 8) | IMU_BUF[13];

		for(int i = 0; i<3; i++){
			IMU_ypr[i] = IMU_r_gyro[i] / 65.5;
		}

		IMU_r_accel[0] = (IMU_BUF[0] << 8) | IMU_BUF[1];
		IMU_r_accel[1] = (IMU_BUF[2] << 8) | IMU_BUF[3];
		IMU_r_accel[2] = (IMU_BUF[4] << 8) | IMU_BUF[5];

		for(int i = 0; i<3; i++){
			IMU_accel[i] =  IMU_r_accel[i] / 4096.0;
		}

		IMU_calcAccelAngle();
		IMU_calcGyroAngle();
	}

	void IMU::IMU_calcAngle(){

		IMU_ComplementaryFilter();
		IMU_FIR();
		//IMU_DLPF();
	}

	void IMU::IMU_calcAccelAngle(){
		IMU_fullvec = sqrt(IMU_accel[0]*IMU_accel[0] + IMU_accel[1]*IMU_accel[1] + IMU_accel[2]*IMU_accel[2]);
		if(IMU_fullvec == 0) {
			IMU_fullvec = 1;
		}

		IMU_acclAngle[0] = ff_fastASin(IMU_accel[0]/IMU_fullvec) * -57.29577951;
		IMU_acclAngle[1] = ff_fastASin(IMU_accel[1]/IMU_fullvec) * 57.29577951;

	}

	void IMU::IMU_calcGyroAngle(){
		IMU_t_ypr[0] += IMU_ypr[0] * looptime;
		IMU_t_ypr[1] += IMU_ypr[1] * looptime;
		IMU_t_ypr[2] += IMU_ypr[2] * looptime;

	}

	void IMU::IMU_ComplementaryFilter(){
		IMU_t_ypr[0] = IMU_t_ypr[0] * 0.9996 + IMU_acclAngle[1] * 0.0004;	// angle is mixed up WHY I DON'T KNOW?
		IMU_t_ypr[1] = IMU_t_ypr[1] * 0.9996 + IMU_acclAngle[0] * 0.0004;



		FIRFilter_Update(&IMU_FIR_Y, IMU_t_ypr[2]);
		FIRFilter_Update(&IMU_FIR_P, IMU_t_ypr[0]);
		FIRFilter_Update(&IMU_FIR_R, IMU_t_ypr[1]);


	}

	void IMU::IMU_FIR(){
		IMU_f_ypr[2] = IMU_FIR_Y.out;
		IMU_f_ypr[0] = IMU_FIR_P.out;
		IMU_f_ypr[1] = IMU_FIR_R.out;
		//roll and pitch tuning on yaw movement https://www.youtube.com/watch?v=4BoIE8YQwM8 17.10.2019
		IMU_f_ypr[0] -= IMU_f_ypr[1] * ff_fastSin(IMU_f_ypr[2] * 0.017453293 * looptime);
		IMU_f_ypr[1] += IMU_f_ypr[0] * ff_fastSin(IMU_f_ypr[2] * 0.017453293 * looptime);
	}

	void IMU::IMU_DLPF(){
		IMU_alpha2_DLPF = (DLPF_YAW * looptime) / (1 + DLPF_YAW * looptime);
		IMU_f_ypr[2] = IMU_f_ypr[2] + (IMU_alpha2_DLPF * (IMU_ypr[2] - IMU_f_ypr[2]));

		IMU_alpha_DLPF = (DLPF_PITCHROLL * looptime) / (1 + DLPF_PITCHROLL * looptime);
		IMU_f_ypr[0] = IMU_f_ypr[0] + (IMU_alpha_DLPF * (IMU_t_ypr[0] - IMU_f_ypr[0]));
		IMU_f_ypr[1] = IMU_f_ypr[1] + (IMU_alpha_DLPF * (IMU_t_ypr[1] - IMU_f_ypr[1]));

		//roll and pitch tuning on yaw movement https://www.youtube.com/watch?v=4BoIE8YQwM8 17.10.2019
		IMU_f_ypr[0] -= IMU_f_ypr[1] * ff_fastSin(IMU_f_ypr[2] * 0.017453293 * looptime);
		IMU_f_ypr[1] += IMU_f_ypr[0] * ff_fastSin(IMU_f_ypr[2] * 0.017453293 * looptime);

	}

	void IMU::FIRFilter_Init(FIRFilter *filt, float *coeff, float *buf, const uint8_t order) {
		filt->out = 0.0f;

		filt->coeff = coeff;
		filt->buf   = buf;
		filt->order = order;
		filt->putIndex = 0;

		for (uint8_t n = 0; n < order; n++) {
			filt->buf[n] = 0.0f;
		}
	}

	float IMU::FIRFilter_Update(FIRFilter *filt, float in) {
		/* Store newest input value in circular buffer */
		filt->buf[filt->putIndex] = in;

		/* Compute filter output */
		uint8_t getIndex = filt->putIndex;

		filt->out = 0.0f;
		for (uint8_t n = 0; n < filt->order; n++) {
			filt->out = filt->out + filt->coeff[n] * filt->buf[getIndex];

			if (getIndex == 0) {
				getIndex = filt->order - 1;
			} else {
			   getIndex--;
			}
		}

		/* Increment buffer index */
		filt->putIndex++;
		if (filt->putIndex == filt->order) {
			filt->putIndex = 0;
		}

		/* Return output */
		return filt->out;
	}
}
