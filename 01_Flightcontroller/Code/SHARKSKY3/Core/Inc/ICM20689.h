/*
 * ICM20689.h
 *
 *  Created on: Aug 29, 2020
 *      Author: Pluscrafter
 */

#ifndef INC_ICM20689_H_
#define INC_ICM20689_H_

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

#define OFFSET_GYRO_X		3
#define OFFSET_GYRO_Y		-46
#define OFFSET_GYRO_Z		-2

#define OFFSET_ACCEL_X		0
#define OFFSET_ACCEL_Y		0
#define OFFSET_ACCEL_Z		0

#define TEMP_OFFSET 		0

#define DLPF_YAW			31.41 //5Hz * 2Pi
#define DLPF_PITCHROLL		942.48//100HZ*2Pi//1256.63 //200Hz * 2Pi

#define FILTER_TAP_NUM 45

#include <stdbool.h>
#include "Instances.hpp"
#include "fast_math.h"
#include <math.h>
#include "FIR.h"
#include "stm32f7xx_hal.h"


namespace SENSORS{
	class IMU{
		public:
			typedef struct {
					float out;
					float *coeff;
					float *buf;
					uint8_t order;
					uint8_t putIndex;
				} FIRFilter ;

			bool 				IMU_init_ok;

			uint8_t				IMU_BUF[14];
			float				IMU_BUF_FIR_Y[FILTER_TAP_NUM];
			float				IMU_BUF_FIR_P[FILTER_TAP_NUM];
			float				IMU_BUF_FIR_R[FILTER_TAP_NUM];
			FIRFilter			IMU_FIR_Y;
			FIRFilter			IMU_FIR_P;
			FIRFilter			IMU_FIR_R;

			static float filter_taps[FILTER_TAP_NUM];

			float 				IMU_ypr[3];

			float 				IMU_t_ypr[3];
			float 				IMU_f_ypr[3];
			float 				IMU_accel[3];
			float				IMU_acclAngle[2];
			float 				IMU_temp;

			float				IMU_alpha2_DLPF;
			float				IMU_alpha_DLPF;

			float				IMU_fullvec;

			int16_t 			IMU_r_accel[3];
			int16_t 			IMU_r_gyro[3];
			int16_t 			IMU_r_temp;



			bool 				IMU_whoami();

			bool 				IMU_Init();
			void				IMU_startINT();

			void				IMU_calcAngle();
			void				IMU_readAngle();

			void 				IMU_calcAccelAngle();
			void				IMU_calcGyroAngle();

			void				IMU_ComplementaryFilter();
			void				IMU_DLPF();
			void				IMU_FIR();

			void				FIRFilter_Init(FIRFilter *filt, float *coeff, float *buf, const uint8_t order);
			float 				FIRFilter_Update(FIRFilter *filt, float in);
		private:
			void 				IMU_writeRegister(uint8_t reg, uint8_t value);
			void 				_IMU_writeRegister(uint8_t reg, uint8_t value, uint8_t delay);

			uint8_t 			IMU_readRegister(uint8_t reg);
			uint8_t 			IMU_readRegisters(uint8_t reg, uint8_t ret[], uint8_t buf);
			uint8_t 			_IMU_readRegister(uint8_t reg, uint8_t delay);

	};
}





#endif /* INC_ICM20689_H_ */
