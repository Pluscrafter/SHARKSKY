/*
 * ICM20689.h
 *
 *  Created on: 21.04.2019
 *      Author: Pluscrafter
 */

#ifndef ICM20689_H_
#define ICM20689_H_

#define SELF_TEST_X_GYRO	0x00
#define SELF_TEST_Y_GYRO	0x01
#define SELF_TEST_Z_GYRO	0x02
#define SELF_TEST_X_ACCEL	0x0D
#define SELF_TEST_Y_ACCEL	0x0E
#define SELF_TEST_Z_ACCEL	0x0F
#define XG_OFFS_USRH		0x13
#define XG_OFFS_USRL		0x14
#define YG_OFFS_USRH		0x15
#define YG_OFFS_USRL		0x16
#define ZG_OFFS_USRH		0x17
#define ZG_OFFS_USRL		0x18
#define SMPLRT_DIV			0x19
#define CONFIG				0x1A
#define GYRO_CONFIG			0x1B
#define ACCEL_CONFIG		0x1C
#define ACCEL_CONFIG_2		0x1D
#define LP_MODE_CFG			0x1E
#define ACCEL_WOM_X_THR		0x20
#define ACCEL_WOM_Y_THR		0x21
#define ACCEL_WOM_Z_THR		0x22
#define FIFO_EN				0x23
#define FSYNC_INT			0x36
#define INT_PIN_CFG			0x37
#define INT_ENABLE			0x38
#define DMP_INT_STATUS		0x39
#define INT_STATUS			0x3A
#define ACCEL_XOUT_H		0x3B
#define ACCEL_XOUT_L		0x3C
#define ACCEL_YOUT_H		0x3D
#define ACCEL_YOUT_L		0x3E
#define ACCEL_ZOUT_H		0x3F
#define ACCEL_ZOUT_L		0x40
#define TEMP_OUT_H			0x41
#define TEMP_OUT_L			0x42
#define GYRO_XOUT_H			0x43
#define GYRO_XOUT_L			0x44
#define GYRO_YOUT_H			0x45
#define GYRO_YOUT_L			0x46
#define GYRO_ZOUT_H			0x47
#define GYRO_ZOUT_L			0x48
#define SIGNAL_PATH_RESET	0x68
#define ACCEL_INTEL_CTRL	0x69
#define USER_CTRL			0x6A
#define PWR_MGMT_1			0x6B
#define PWR_MGMT_2			0x6C
#define FIFO_COUNTH			0x72
#define FIFO_COUNTL			0x73
#define FIFO_R_W			0x74
#define WHO_AM_I			0x75
#define XA_OFFSET_H			0x77
#define XA_OFFSET_L			0x78
#define YA_OFFSET_H			0x7A
#define YA_OFFSET_L			0x7B
#define ZA_OFFSET_H			0x7D
#define ZA_OFFSET_L			0x7E

#define OFFSET_GYRO_X		15
#define OFFSET_GYRO_Y		-49
#define OFFSET_GYRO_Z		-2

#define OFFSET_ACCEL_X		0
#define OFFSET_ACCEL_Y		0
#define OFFSET_ACCEL_Z		0

#define TEMP_OFFSET 		0

#include "I_SPI.h"
#include "usart.h"
#include "stdio.h"

namespace Sensor {

class ICM20689 {
public:
	ICM20689();
	virtual 			~ICM20689();

	bool 				init_ok = false;

	float 				ypr[3];
	float 				t_ypr[3];
	float 				accel[3];
	float 				temp;

	int16_t 			r_accel[3];
	int16_t 			r_gyro[3];
	int16_t 			r_temp;

	void 				Initalize();
	void 				SelfTest();
	void 				DMPIntStatus();
	void 				IntStatus();
	void 				SigReset();
	void 				FIFOCount();
	void 				FIFORW();

	void 				ReadGyro();
	void 				ReadAccel();
	void 				ReadTemp();

	void 				FindOffset();

private:
	Interface::I_SPI 	spi1 = Interface::I_SPI(hspi3, GPIOA, 20);

	void 				SetGyroOffset();
	void 				SetAccelOffset();

	void 				InitalizeCalibration();
	void 				PullBracketsOut();
	void 				PullBracketsIn();
	void 				ForceHeader();
	void 				GetSmoothed();
	void 				SetOffsets(int16_t TheOffsets[3]);
	void 				ShowProgress();
	void 				SetAveraging(int NewN);
	void 				getMotion3(int16_t& gx, int16_t& gy, int16_t& gz);

	void 				setXGyroOffset(int16_t offs);
	void 				setYGyroOffset(int16_t offs);
	void 				setZGyroOffset(int16_t offs);


	const char 			LBRACKET = '[';
	const char 			RBRACKET = ']';
	const char 			COMMA    = ',';
	const char 			BLANK    = ' ';
	const char 			PERIOD   = '.';

	const int 			iGx = 0;
	const int 			iGy = 1;
	const int 			iGz = 2;

	const int 			usDelay = 3150;   // empirical, to hold sampling to 200 Hz
	const int 			NFast =  1000;    // the bigger, the better (but slower)
	const int 			NSlow = 10000;    // ..
	const int 			LinesBetweenHeaders = 2;
	int16_t 			LowValue[3];
	int16_t 			HighValue[3];
	int 				Smoothed[3];
	int16_t 			LowOffset[3];
	int16_t 			HighOffset[3];
	int 				Target[3];
	int 				LinesOut;
	int 				N 		;


};

} /* namespace Sensor */

#endif /* ICM20689_H_ */
