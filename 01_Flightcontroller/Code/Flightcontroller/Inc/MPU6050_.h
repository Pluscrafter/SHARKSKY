/*
 * MPU6050.h
 *
 *  Created on: 29.01.2019
 *      Author: Pluscrafter
 */

#ifndef MPU6050__H_
#define MPU6050__H_

#define SELF_TEST_X 	0x0D
#define SELF_TEST_Y 	0x0E
#define SELF_TEST_Z 	0x0F
#define SELF_TEST_A 	0x10
#define SMPLRT_DIV 		0x19
#define CONFIG			0x1A
#define GYRO_CONFIG 	0x1B
#define ACCEL_CONFIG 	0x1C
#define FIFO_EN 		0x23
#define INT_PIN_CFG		0x37
#define INT_ENABLE		0x38
#define INT_STATUS		0x3A
#define ACCEL_XOUT_H	0x3B
#define ACCEL_XOUT_L	0x3C
#define ACCEL_YOUT_H	0x3D
#define ACCEL_YOUT_L	0x3E
#define ACCEL_ZOUT_H	0x3F
#define ACCEL_ZOUT_L	0x40
#define TEMP_OUT_H		0x41
#define TEMP_OUT_L		0x42
#define GYRO_XOUT_H		0x43
#define GYRO_XOUT_L		0x44
#define GYRO_YOUT_H		0x45
#define GYRO_YOUT_L		0x46
#define GYRO_ZOUT_H		0x47
#define GYRO_ZOUT_L		0x48
#define USER_CTRL		0x6A
#define PWR_MGMT_1		0x6B
#define PWR_MGMT_2		0x6C
#define FIFO_COUNTH		0x72
#define FIFO_COUNTL		0x73
#define	FIFO_R_W		0x74
#define WHO_AM_I		0x75

#define DLPF_CFG_260 	0x00
#define DLPF_CFG_184 	0x01
#define DLPF_CFG_94 	0x02
#define DLPF_CFG_44 	0x03
#define DLPF_CFG_21 	0x04
#define DLPF_CFG_10 	0x05
#define DLPF_CFG_5  	0x06

#define FS_SEL_250		0x00
#define FS_SEL_500		0x01
#define FS_SEL_1000		0x02
#define FS_SEL_2000		0x03

#define AFS_SEL_2		0x00
#define AFS_SEL_4		0x01
#define AFS_SEL_8		0x02
#define AFS_SEL_16		0x03

#define DATA_RDY_EN		0x01

#define DELAY 			1

#include "main.h"
#include "i2c.h"

namespace Sensors {

class MPU6050 {
public:
	MPU6050(uint16_t address, bool IntEnable);

	virtual 				~MPU6050();

	static void 			Selftest();
	static void 			Config();
	static void 			Init();
	static void 			SetGyroResolution();
	static void 			SetACCLResolution();

	static void 			readGyroRAW();
	static void 			readAcclRAW();

	static uint16_t 		rawGYRO[3] ,rawACCL[3], errorDL, DL, peak;
	static uint8_t 			TxData[6];

	static bool 			dataRDY;



private:
	static uint16_t 		address;
	static bool 			IntEnable;

};

} /* namespace Sensors */

#endif /* MPU6050__H_ */
