/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#define CMode
#undef CMode

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifndef CMode
#include "Flightcontroller.h"
#endif

#ifdef CMode
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

#include <math.h>
#include <string>
#include "stdio.h"

#include "RF24.h"
#include "ICM20689.h"

#endif

#include "IMU.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#ifdef CMode
SENSOR::ICM20689 			imu 				= SENSOR::ICM20689(hspi3, IMU_NSS_GPIO_Port, IMU_NSS_Pin);

bool 						osdusb 				= OSD_MODE;

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

double						pid_AM_P[3]			= {0, 0, 0};			//!< Angle motion P
double						pid_AM_I[3]			= {0, 0, 0};			//!< Angle motion I
double						pid_AM_D[3]			= {0, 0, 0};			//!< Angle motion D

double						pid_TA_P[3]			= {0, 0, 0};			//!< True angle P
double						pid_TA_I[3]			= {0, 0, 0};			//!< True angle I
double						pid_TA_D[3]			= {0, 0, 0};			//!< True angle D

double						pid_AM[3];
double						pid_TA[3];

uint16_t					motorSpeed[4]		= {0, 0, 0};			//!< Motor Speed [1024;2048]

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

RF24 						radio = RF24(GPIOC, 6, GPIOB, 12, &hspi2);	//!< define radio (nRF24L01)

uint8_t  sine[256] = { 													//!< 8bit sine for test with DAC
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

static volatile double		looptime;

bool initOK 					= false;

volatile float	ypr[3]			= {0, 0, 0};
float			t_ypr[3]		= {0, 0, 0};
float			f_ypr[3]		= {0, 0, 0};
volatile float 	accel[3]			= {0, 0, 0};
float 			fullVec			= 0;
float 			accelAngle[2]		= {0, 0};

float alpha2;

float acangle[2];	//acceleration angle
float fullvec;

float fastsqrt(float val);
float fastsin(float sin);
float fastasin(float x);

#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);



void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){
	//read IMU GYRO ACCEL alternating
	if (icm.init == true){
		int16_t r_gyro[3],r_accel[3];

		if(icm.ac == 0){
			r_gyro[0] = (icm.buf[0] << 8) | icm.buf[1];
			r_gyro[1] = (icm.buf[2] << 8) | icm.buf[3];
			r_gyro[2] = (icm.buf[4] << 8) | icm.buf[5];

			for(int i = 0; i<3; i++){
				icm.ypr[i] = r_gyro[i] / 65.5;
			}
			icm.ac = 1;
		}else{
			r_accel[0] = (icm.buf[0] << 8) | icm.buf[1];
			r_accel[1] = (icm.buf[2] << 8) | icm.buf[3];
			r_accel[2] = (icm.buf[4] << 8) | icm.buf[5];

			for(int i = 0; i<3; i++){
				icm.accel[i] =  r_accel[i] / 4096.0;
			}
			icm.ac = 0;
		}
		//NSS HIGH IMU SPI
		HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, GPIO_PIN_SET);
	}
}
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifndef CMode
FLIGHTCONTROLLER::Flightcontroller fc;
#endif

#ifdef CMode
void PID_AngleMotion();
void MotorCalibration();
void PID_TrueAngle();
void SetMotorSpeed();
#endif
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SDMMC1_SD_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_I2C3_Init();
  MX_TIM2_Init();
  MX_DAC_Init();
  MX_SPI3_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
#ifndef CMode
  fc.Init();
#endif

#ifdef CMode
	//start DWT counter also when not in Debug mode to count clock ticks
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	//https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter
	DWT->LAR = 0xC5ACCE55; 							//https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter 12.10.19 02:03
	DWT->CYCCNT = 0;								//https://www.carminenoviello.com/2015/09/04/precisely-measure-microseconds-stm32/ 12.10.19 01:30
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	// INIT MOTORS
	// Start PWM on timer 2
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

	// INIT GIMBAL
	// Start PWM on timer 3
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

	//Start and Test DAC
	HAL_TIM_Base_Start(&htim6);														//start Timer 6 for dac tickspeed
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)sine, 256, DAC_ALIGN_8B_R);	//start DAC DMA read from sine array in circular mode
	HAL_Delay(1000);
	HAL_TIM_Base_Stop(&htim6);

	//Set UART to USB or OSD Mode
	if(HAL_GPIO_ReadPin(USBOSD_GPIO_Port, USBOSD_Pin) == GPIO_PIN_RESET){
		osdusb = USB_MODE;
		MotorCalibration();
		HAL_GPIO_WritePin(MOD0_LED_GPIO_Port, MOD0_LED_Pin, GPIO_PIN_SET);
	}else{
		osdusb = OSD_MODE;
		HAL_GPIO_WritePin(MOD1_LED_GPIO_Port, MOD1_LED_Pin, GPIO_PIN_SET);
	}

	//init Radio
	const uint64_t pipe = 0xE8E8F0F0E2;	//pipe address
	radio.begin();
	radio.setPayloadSize(32);
	radio.setChannel(125);
	radio.setDataRate(RF24_250KBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setAutoAck(true);
	radio.enableDynamicPayloads();
	radio.enableAckPayload();
	radio.openReadingPipe(1, pipe);

	//radio.printDetails();

	radio.startListening();

	if(!imu.initICM()){
		for(;;){
			HAL_GPIO_TogglePin(INIT_OK_GPIO_Port, INIT_OK_Pin);
			HAL_Delay(100);
		}
	}

	initOK = true;
	HAL_TIM_Base_Start_IT(&htim4); //start IMU trigger

	// initalisation is OK
	HAL_Delay(1000);
	HAL_GPIO_WritePin(INIT_OK_GPIO_Port, INIT_OK_Pin, GPIO_PIN_SET);

#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#ifndef CMode
	  for (uint8_t i = 0; i < 3; i++){
		  fc.imu.accel[i] = icm.accel[i];
		  fc.imu.ypr[i] = icm.ypr[i];
	  }

	  icm.init = fc.imu.initOK;
	  fc.Loop();
#endif

#ifdef CMode
		double start = DWT->CYCCNT; //read value from counter register
		imu.lptime = looptime;

		  fullvec = fastsqrt(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]); //calculate full vector with Pythagoras' theorem
		  if(fullvec == 0) {
			  fullvec = 1;
		  }

		  acangle[0] = fastasin(accel[0]/fullvec) * -57.29577951;	//calculate acceleration sin-1(angle/fullvec)
		  acangle[1] = fastasin(accel[1]/fullvec) * 57.29577951;

		  //calculate angle with angular motion
		  t_ypr[0] += ypr[0]*looptime;
		  t_ypr[1] += ypr[1]*looptime;
		  t_ypr[2] += ypr[2]*looptime;


		  alpha2 = (DLPF_FREQ_YAW * looptime) / (1 + DLPF_FREQ_YAW * looptime);
		  f_ypr[2] = f_ypr[2] - (alpha2 * (f_ypr[2] - ypr[2]));

		  //roll and pitch tuning on yaw movement https://www.youtube.com/watch?v=4BoIE8YQwM8 17.10.2019
		  t_ypr[0] -= t_ypr[1] * fastsin(ypr[2] * 0.017453293 * looptime);
		  t_ypr[1] += t_ypr[0] * fastsin(ypr[2] * 0.017453293 * looptime);

		  //complementary filter
		  t_ypr[0] = t_ypr[0] * 0.9996 + acangle[1] * 0.0004;	// angle is mixed up
		  t_ypr[1] = t_ypr[1] * 0.9996 + acangle[0] * 0.0004;

		  //is nan error
		  if (isnan(t_ypr[0])){
			  t_ypr[0] = 0;
		  }

		  if (isnan(t_ypr[1])){
			  t_ypr[1] = 0;
		  }

		if(recvData.throttle < 100){
			pidGain_AM_P[0] = recvData.y_P/100.0;
			pidGain_AM_I[0] = recvData.y_I/100.0;
			pidGain_AM_D[0] = recvData.y_D/100.0;

			pidGain_TA_P[1] = recvData.rp_P/100.0;
			pidGain_TA_I[1] = recvData.rp_I/100.0;
			pidGain_TA_D[1] = recvData.rp_D/100.0;

			pidGain_TA_P[2] = recvData.rp_P/100.0;
			pidGain_TA_I[2] = recvData.rp_I/100.0;
			pidGain_TA_D[2] = recvData.rp_D/100.0;

		}

		//set previous error from error
		for (uint8_t i = 0;  i < 3; i++){
		  previousError[i] = error[i];
		}

		error[0] = imu.f_ypr[2] - recvData.yaw;
		error[1] = recvData.pitch - imu.t_ypr[1] ;
		error[2] = recvData.roll - imu.t_ypr[0];

		if(radio.available()){
			radio.read(&recvData,sizeof(RadioData));
			radio.writeAckPayload(1,&ackData,sizeof(AckData));
		}

		PID_AngleMotion();
		PID_TrueAngle();

		SetMotorSpeed();

		double stop = DWT->CYCCNT;
		looptime = (stop - start)/216000000.0;
#endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C3
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#ifdef CMode
void MotorCalibration(){
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,2048);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,2048);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,2048);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,2048);

	HAL_Delay(5000);

	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1024);

	HAL_Delay(5000);

	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,2048);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,2048);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,2048);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,2048);

	HAL_Delay(5000);

	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1024);

	HAL_Delay(10000);

	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1124);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1124);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1124);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1124);

	HAL_Delay(2000);

	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1024);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1024);
}

void SetMotorSpeed(){
	// throttle value under 100 no movement of motors
	if (recvData.throttle < 100){
		for (int i = 0; i < 4 ;i++){
			motorSpeed[i] = 1024;
		}
	}
	else{
	//PID applied to the calculation motor speed
		motorSpeed[0] = (1024 + recvData.throttle) + pid_TA[1] + pid_TA[2] + pid_AM[0];
		motorSpeed[1] = (1024 + recvData.throttle) + pid_TA[1] - pid_TA[2] - pid_AM[0];
		motorSpeed[2] = (1024 + recvData.throttle) - pid_TA[1] - pid_TA[2] + pid_AM[0];
		motorSpeed[3] = (1024 + recvData.throttle) - pid_TA[1] + pid_TA[2] - pid_AM[0];

	}

	// maximum motor speed
	for (int i = 0; i < 4 ;i++){
		if (motorSpeed[i] > 2048){
			motorSpeed[i] = 2048;
		}
		else if(motorSpeed[i] < 1024){
			motorSpeed[i] = 1024;
		}
	}

	//write to timer
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,motorSpeed[0]);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,motorSpeed[1]);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,motorSpeed[2]);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,motorSpeed[3]);
}


void PID_TrueAngle(){
	for(uint8_t i = 0; i < 3; i++){
		//calc pid
		pid_TA_P[i] = error[i] * pidGain_TA_P[i];
		pid_TA_I[i] += error[i] * pidGain_TA_I[i] * looptime;
		pid_TA_D[i] = pidGain_TA_D[i] * ((error[i] - previousError[i]) * looptime);

		//sum of all corrections
		pid_TA[i] = pid_TA_P[i] + pid_TA_I[i] + pid_TA_D[i];

		//set max correction
		if(pid_TA[i] > MAX_CORRECTION){
			pid_TA[i] = MAX_CORRECTION;
		}else if(pid_TA[i] < -MAX_CORRECTION){
			pid_TA[i] = -MAX_CORRECTION;
		}
	}
}

void PID_AngleMotion(){
	for(uint8_t i = 0; i < 3; i++){
		//calc pid
		pid_AM_P[i] = error[i] * pidGain_AM_P[i];
		pid_AM_I[i] += error[i] * pidGain_AM_I[i] * looptime;
		pid_AM_D[i] = pidGain_AM_D[i] * ((error[i] - previousError[i]) * looptime);

		//sum of all corrections
		pid_AM[i] = pid_AM_P[i] + pid_AM_I[i] + pid_AM_D[i];

		//set max correction
		if(pid_AM[i] > MAX_CORRECTION){
			pid_AM[i] = MAX_CORRECTION;
		}else if(pid_AM[i] < -MAX_CORRECTION){
			pid_AM[i] = -MAX_CORRECTION;
		}
	}
}

float fastsqrt(float val) { //https://bits.stephan-brumme.com/squareRoot.html
	unsigned int i = *(unsigned int*) &val;
	// adjust bias
	i  += 127 << 23;
	// approximation of square root
	i >>= 1;
	return *(float*) &i;
}

float fastsin(float val){ //sinus polynom
	return val - ((val*val*val)/6) + ((val*val*val*val*val)/120) - ((val*val*val*val*val*val*val)/5040);
}

float fastasin(float x){//https://en.wikipedia.org/wiki/Inverse_trigonometric_functions#Infinite_series
	return x+(0.5)*((x*x*x)/3)+(0.375)*((x*x*x*x*x)/5)+(0.3125)*((x*x*x*x*x*x*x)/7);
}

#endif
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
