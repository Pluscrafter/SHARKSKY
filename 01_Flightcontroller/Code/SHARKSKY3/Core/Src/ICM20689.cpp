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
			0.0001376313011860475,0.0001571818138472736,0.00017914680938702077,0.000204142983420752,0.0002327966649318114,0.0002657401200849563,0.0003036078705918044,0.0003470327064860612,0.0003966419608332217,0.0004530534497462213,0.0005168714560568333,0.0005886830622330308,0.0006690540467388928,0.0007585249259136617,0.0008576075779274106,0.0009667810518294573,0.0010864883661270142,0.001217133249156177,0.0013590763555839658,0.0015126330545172095,0.0016780691221356392,0.0018555998103693128,0.0020453864708542824,0.0022475344594568014,0.002462091390043497,0.0026890458539128304,0.0029283249750733376,0.003179795341566205,0.0034432602114975452,0.003718461375683546,0.00400507589802146,0.004302720073610544,0.0046109482645988464,0.004929251503199339,0.00525706447660923,0.0055937618017196655,0.0059386612847447395,0.006291028577834368,0.0066500757820904255,0.007014967035502195,0.007384822703897953,0.007758717052638531,0.008135688491165638,0.008514737710356712,0.008894839324057102,0.009274936281144619,0.009653950110077858,0.010030788369476795,0.01040433906018734,0.010773487389087677,0.011137112975120544,0.01149409543722868,0.011843322776257992,0.01218369510024786,0.012514125555753708,0.01283354964107275,0.013140929862856865,0.013435260392725468,0.013715566135942936,0.013980916701257229,0.014230421744287014,0.014463243074715137,0.014678589068353176,0.014875727705657482,0.015053988434374332,0.015212753787636757,0.015351480804383755,0.015469690784811974,0.015566971153020859,0.015642987564206123,0.015697475522756577,0.015730243176221848,0.015741178765892982,0.015730243176221848,0.015697475522756577,0.015642987564206123,0.015566971153020859,0.015469690784811974,0.015351480804383755,0.015212753787636757,0.015053988434374332,0.014875727705657482,0.014678589068353176,0.014463243074715137,0.014230421744287014,0.013980916701257229,0.013715566135942936,0.013435260392725468,0.013140929862856865,0.01283354964107275,0.012514125555753708,0.01218369510024786,0.011843322776257992,0.01149409543722868,0.011137112975120544,0.010773487389087677,0.01040433906018734,0.010030788369476795,0.009653950110077858,0.009274936281144619,0.008894839324057102,0.008514737710356712,0.008135688491165638,0.007758717052638531,0.007384822703897953,0.007014967035502195,0.0066500757820904255,0.006291028577834368,0.0059386612847447395,0.0055937618017196655,0.00525706447660923,0.004929251503199339,0.0046109482645988464,0.004302720073610544,0.00400507589802146,0.003718461375683546,0.0034432602114975452,0.003179795341566205,0.0029283249750733376,0.0026890458539128304,0.002462091390043497,0.0022475344594568014,0.0020453864708542824,0.0018555998103693128,0.0016780691221356392,0.0015126330545172095,0.0013590763555839658,0.001217133249156177,0.0010864883661270142,0.0009667810518294573,0.0008576075779274106,0.0007585249259136617,0.0006690540467388928,0.0005886830622330308,0.0005168714560568333,0.0004530534497462213,0.0003966419608332217,0.0003470327064860612,0.0003036078705918044,0.0002657401200849563,0.0002327966649318114,0.000204142983420752,0.00017914680938702077,0.0001571818138472736,0.0001376313011860475
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

		IMU_acclAngle[0] = asin(IMU_accel[0]/IMU_fullvec) * -57.29577951;
		IMU_acclAngle[1] = asin(IMU_accel[1]/IMU_fullvec) * 57.29577951;

	}

	void IMU::IMU_calcGyroAngle(){
		IMU_t_ypr[0] += IMU_ypr[0] * looptime;
		IMU_t_ypr[1] += IMU_ypr[1] * looptime;
		IMU_t_ypr[2] = IMU_ypr[2]; //* looptime;

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
		IMU_f_ypr[0] -= IMU_f_ypr[1] * sin(IMU_f_ypr[2] * 0.017453293 * looptime);
		IMU_f_ypr[1] += IMU_f_ypr[0] * sin(IMU_f_ypr[2] * 0.017453293 * looptime);
	}

	void IMU::IMU_DLPF(){
		IMU_alpha2_DLPF = (DLPF_YAW * looptime) / (1 + DLPF_YAW * looptime);
		IMU_f_ypr[2] = IMU_f_ypr[2] + (IMU_alpha2_DLPF * (IMU_ypr[2] - IMU_f_ypr[2]));

		IMU_alpha_DLPF = (DLPF_PITCHROLL * looptime) / (1 + DLPF_PITCHROLL * looptime);
		IMU_f_ypr[0] = IMU_f_ypr[0] + (IMU_alpha_DLPF * (IMU_t_ypr[0] - IMU_f_ypr[0]));
		IMU_f_ypr[1] = IMU_f_ypr[1] + (IMU_alpha_DLPF * (IMU_t_ypr[1] - IMU_f_ypr[1]));

		//roll and pitch tuning on yaw movement https://www.youtube.com/watch?v=4BoIE8YQwM8 17.10.2019
		IMU_f_ypr[0] -= IMU_f_ypr[1] * sin(IMU_f_ypr[2] * 0.017453293 * looptime);
		IMU_f_ypr[1] += IMU_f_ypr[0] * sin(IMU_f_ypr[2] * 0.017453293 * looptime);

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
		filt->buf[filt->putIndex] = in;

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

		filt->putIndex++;
		if (filt->putIndex == filt->order) {
			filt->putIndex = 0;
		}

		return filt->out;
	}
}
