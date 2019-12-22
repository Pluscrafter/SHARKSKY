/*
 * Flightcontroller.h
 * Author: Pluscrafter
 *
 * Flightcontroller class only static metohdes because there can only be one instance of the FC
 */

#ifndef FLIGHTCONTROLLER_H_
#define FLIGHTCONTROLLER_H_

/**
 * Defines for disable or enable various features.
 */

#define ICM20689_ENABLE 		1

#define SD_CARD					0

#define ICM20689_OFFSET_FIND  	0

#define PID_TRUE_ANGLE			1
#define PID_ANGLE_MOTION		0

#define USB_MODE 				true
#define OSD_MODE 				false

#define DLPF_FREQ_YAW			5.0
#define DLPF_FREQ_PITCHROLL		200.0

#define CUTOFF_ANGLE			40.0

#define MAX_CORRECTION			600

/************************
 * Includes of HALCubeMX
 ************************/

#include "main.h"
#include "stdio.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/******************
 * Includes of C++
 ******************/

#include <math.h>
#include <string>
#include "stdio.h"

/**********************
 * Includes of Sensors
 **********************/


/**
 * NAMESPACE for FC
 */
namespace FLIGHTCONTROLLER {

	class Flightcontroller {
	public:

		/*******************
		 * PUBLIC FUNCTIONS
		 *******************/

		Flightcontroller();
		/**
		 *	Constructor. Not used because this is a 'static' class
		 */

		virtual 				~Flightcontroller();
		/**
		 *	Destructor. Not used because this is a 'static' class
		 */

		void					Init();
		/**
		 * This function initialize the components of the FC.
		 *
		 * 	- Basic initialisation:
		 * 		-# Init timer for driving the ESC's and Gimbal, triggering the IMU readout and for the DAC
		 * 			-## Init timer 2, 3, 4 and 6
		 * 	- Sensor initialisation:
		 * 		-# Init IMU
		 * 		-# Init nRF24
		 * 		-# Init SDCard
		 */

		void					Loop();
		/**
		 * This function is the loop function of the FC
		 *
		 *  - Read clock for loop time
		 *  - Read IMU
		 *  	-# Zeroing in the first 5 second on startup
		 *  - Read PID from receieved data
		 *  	-# Read apply only if throttle < 100
		 *  - Write logging data to SDCard
		 *  - Write data to ACK Message
		 *	- Set error to previous error
		 *	- Read radio data
		 *	- calculate error
		 *	- calculate PID
		 *	- Read clock for loop time
		 *	- calculate loop time
		 */


		/*******************
		 * PUBLIC VARIABLES
		 *******************/


	private:

		/********************
		 * PRIVATE FUNCTIONS
		 ********************/

		void	 				PID_TrueAngle();
		/**
		 * Calculate true angle pid.
		 */

		void	 				PID_AngleMotion();
		/**
		 * Calculate angle motion pid.
		 */

		void	 				MotorCalibration();
		/**
		 * Calibrate ESC
		 *
		 * 	- 100% throttle
		 * 	- wait 5s
		 * 	- 0% throttle
		 * 	- wait 5s
		 * 	- 100% throttle
		 * 	- wait 5s
		 * 	- 0% throttle
		 * 	- wait 10s
		 * 	- 50% throttle
		 * 	- wait 2s
		 * 	- 0% throttle
		 * 	- calibration complete
		 */

		void	 				LoopRadio();
		/**
		 * read radio if data is aviable
		 */

		void	 				SetMotorSpeed();
		/**
		 * set motor speed according to PID-Values
		 *
		 * 	- calculate PID to motor speed
		 * 	- set PWM for ESC
		 */

		void					SetPIDGain();

		void 					CalculateError();

		void					InitMotors();

		void					InitGimbal();

		void					InitRadio();

		void					InitDAC();

		void					InitCounter();

		void					InitSDCard();

		/********************
		 * PRIVATE VARIABLES
		 ********************/

		/******************
		 * FC Variables
		 ******************/

		bool 						osdusb 				= USB_MODE;
		double						looptime			= 0;

		/****************
		 * PID Variables
		 ****************/

		double						error[3] 			= {0, 0, 0};			//!< Error for the PID calculation
		double						previousError[3]	= {0, 0, 0};			//!< Previous error for the I-Gain of the PID

		int16_t						pidAngleMotion[3]	= {0, 0, 0};			//!< Angle motion PID
		int16_t						pidTrueAngle[3]		= {0, 0, 0};			//!< True angle PID

		double						pidGain_AM_P[3]		= {0, 0, 0};			//!< Angle motion P-Gain
		double						pidGain_AM_I[3]		= {0, 0, 0};			//!< Angle motion I-Gain
		double						pidGain_AM_D[3]		= {0, 0, 0};			//!< Angle motion D-Gain

		double						pidGain_TA_P[3]		= {0, 0, 0};			//!< True angle P-Gain
		double						pidGain_TA_I[3]		= {0, 0, 0};			//!< True angle I-Gain
		double						pidGain_TA_D[3]		= {0, 0, 0};			//!< True angle D-Gain

		double						pid_AM_P[3]		= {0, 0, 0};				//!< Angle motion P
		double						pid_AM_I[3]		= {0, 0, 0};				//!< Angle motion I
		double						pid_AM_D[3]		= {0, 0, 0};				//!< Angle motion D

		double						pid_TA_P[3]		= {0, 0, 0};				//!< True angle P
		double						pid_TA_I[3]		= {0, 0, 0};				//!< True angle I
		double						pid_TA_D[3]		= {0, 0, 0};				//!< True angle D

		double						pid_AM[3];
		double						pid_TA[3];

		/******************
		 * Motor Variables
		 ******************/

		uint16_t					motorSpeed[4]		= {0, 0, 0};			//!< Motor Speed [1024;2048]

		/*******************
		 * SDCard Variables
		 *******************/

		UINT						reSD;										//!< count errors of write to SD
		int 						dataCount = 0;								//!< count number of data in buffer
		double						elapsedTime = 0;							//!< elapsed time
		char 						logbuffer[10000];							//!< write buffer
		std::string 				tmpbuffer = " ";							//!< tmp write buffer in loop

		/****************
		 * GPS Variables
		 ****************/

		char						gpsbuffer[80];								//!< GPS buffer for data through UART

		/******************
		 * Radio Variables
		 ******************/

		//! acklowlegement message struct
		struct AckData{
			int16_t 				yaw;										//!< angle motion yaw
			int16_t 				pitch;										//!< true motion pitch
			int16_t 				roll;										//!< true motion roll

			uint16_t 				heading;									//!< magnetic heading
			uint32_t 				altitude; 									//!< barometric altitide

			uint32_t 				LV03x;										//!< Position Swiss LV03 x
			uint32_t 				LV03y;										//!< Position Swiss LV03 y

			uint16_t 				flags;										//!< Option Flags
			uint32_t				uptdate_time; 								//!< Loop time
		};

		//! receive message struct
		struct RadioData{
			int16_t					yaw;										//!< Setpoint yaw
			int16_t					pitch;										//!< Setpoint pitch
			int16_t					roll;										//!< Setpoint roll
			uint16_t				throttle;									//!< Throttle

			uint16_t				rp_P;										//!< roll pitch P-Gain
			uint16_t				rp_I;										//!< roll pitch I-Gain
			uint16_t				rp_D;										//!< roll pitch D-Gain

			uint16_t				y_P;										//!< yaw P-Gain
			uint16_t				y_I;										//!< yaw I-Gain
			uint16_t				y_D;										//!< yaw D-Gain

			uint16_t				flags;										//!< 2 bytes 10
			uint32_t				data;										//!< 4 bytes 14
		};

		AckData 					ackData;									//!< acknowlegement data
		RadioData 					recvData;									//!< receive data

		/******************
		 * DAC Variables
		 ******************/

		uint8_t  sine[256] = { //!< 8bit sine for test with DAC
		  0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
		  0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
		  0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
		  0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
		  0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
		  0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
		  0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
		  0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
		  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
		  0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
		  0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
		  0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
		  0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
		  0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
		  0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
		  0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
		  0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
		  0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
		  0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
		  0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
		  0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
		  0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
		  0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
		  0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
		  0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
		  0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
		  0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
		  0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
		  0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
		  0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
		  0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
		  0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
		};

	};

} /* namespace FLIGHTCONTROLLER */

#endif /* FLIGHTCONTROLLER_H_ */
