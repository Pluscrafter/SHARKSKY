/*
 * MPU6500.h
 *
 *  Created on: 21.04.2019
 *      Author: Pluscrafter
 */

#ifndef MPU6500_H_
#define MPU6500_H_

#include "I_SPI.h"


extern SPI_HandleTypeDef hspi1;

#define SELF_TEST_X_GYRO	0x00
#define SELF_TEST_Y_GYRO	0x01
#define SELF_TEST_Z_GYRO	0x02
#define SELF_TEST_X_ACCEL	0x0D
#define SELF_TEST_Y_ACCEL	0x0E
#define SELF_TEST_Z_ACCEL	0x0F
#define XG_OFFSET_H			0x13
#define XG_OFFSET_L			0x14
#define YG_OFFSET_H			0x15
#define YG_OFFSET_L			0x16
#define ZG_OFFSET_H			0x17
#define ZG_OFFSET_L			0x18
#define SMPLRT_DIV			0x19
#define CONFIG				0x1A
#define GYRO_CONFIG			0x1B
#define ACCEL_CONFIG		0x1C
#define ACCEL_CONFIG_2		0x1D
#define LP_ACCEL_ODR		0x1E
#define WOM_THR				0x1F
#define FIFO_EN				0x23
#define INT_PIN_CFG			0x37
#define INT_ENABLE			0x38
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
#define I2C_MST_DELAY_CTRL	0x67
#define SIGNAL_PATH_RESET	0x68
#define ACCEL_INTEL_CTRL	0x69
#define USER_CTRL			0x6A
#define PWR_MGMT_1			0x6B
#define PWR_MGMT_2			0x6C
#define FIFO_COUNT_H		0x72
#define FIFO_COUNT_L		0x73
#define FIFO_R_W			0x74
#define WHO_AM_I			0x75
#define XA_OFFSET_H			0x77
#define XA_OFFSET_L			0x78
#define YA_OFFSET_H			0x7A
#define YA_OFFSET_L			0x7B
#define ZA_OFFSET_H			0x7D
#define ZA_OFFSET_L			0x7E


namespace Sensor {

class MPU6500 {
public:
	MPU6500();
	virtual ~MPU6500();

	float ypr[3];
	float accel[3];
	float gyro[3];

	uint8_t id;

	void Initalize();
	void SelfTest();
	void DMPIntStatus();
	void IntStatus();
	void SigReset();
	void FIFOCount();
	void FIFORW();

	void ReadGyro();
	void ReadAccel();
	void ReadTemp();

private:
	Interface::I_SPI spi1 = Interface::I_SPI(hspi1);

	void SetGyroOffset();
	void SetAccelOffset();
	void Config();
	void ConfigGyro();
	void ConfigAccel();
	void FIFOEnable();
	void IntConfig();
	void IntEnable();
	void AccelIntCtr();
	void UserCtl();
	void PWRMgmt();
	void WhoAmI();
};

} /* namespace Sensor */

#endif /* MPU6500_H_ */
