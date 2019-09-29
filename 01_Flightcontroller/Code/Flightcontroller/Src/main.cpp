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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
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


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "ICM20689.h"
#include "MPU6050.h"
#include "RF24.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ARM_CM_DEMCR      		(*(uint32_t *)0xE000EDFC)
#define ARM_CM_DWT_CTRL   		(*(uint32_t *)0xE0001000)
#define ARM_CM_DWT_CYCCNT 		(*(uint32_t *)0xE0001004)

#define MPU6050_ENABLE 			0
#define MPU6000_ENABLE 			0
#define ICM20689_ENABLE 		1

#define ICM20689_OFFSET_FIND  	0

#define TEST_NRF_T				0
#define TEST_NRF_R				0

#define PID_TRUE_ANGLE			1
#define PID_ANGLE_MOTION		0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void loopRadio();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define USB_MODE true
#define OSD_MODE false

bool osdusb = USB_MODE;

uint8_t  sine[256] = {
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


//ICM
Sensor::ICM20689 imu;
float 		PI = 3.1415;

uint32_t 	start = 0, stop = 0;
double 		lptime = 0.101;

//Radio
RF24 radio(GPIOC, 6, GPIOB, 12, &hspi2);
const uint8_t addresses[][6] = {"1Node","2Node"};

uint8_t data[1] = { 0x41};

//! Acknowlegement struct
struct AckData{
	int16_t 	yaw;									//!<2 bytes 2
	int16_t 	pitch;									//!<2 bytes 4
	int16_t 	roll;									//!<2 bytes 6

	uint16_t 	heading;								//!<2 bytes 8
	uint32_t 	altitude; 								//!<4 bytes 12

	uint32_t 	LV03x;									//!<4 byte 16
	uint32_t 	LV03y;									//!<4 byte 20

	uint16_t 	flags;									//!<2 bytes 22
	uint32_t	uptdate_time; 							//!<4 bytes 26
};

//! receive message struct
struct RadioData{
	int16_t		yaw;									//!<2 bytes 2
	int16_t		pitch;									//!<2 bytes 4
	int16_t		roll;									//!<2 bytes 6
	uint16_t	throttle;								//!<2 bytes 8

	uint16_t	flags;									//!<2 bytes 10
	uint32_t	data;									//!<4 bytes 14
};

AckData 		ackData;								//!< define acknowlegement
RadioData 		recvData;								//!< Define receive data


//I2cdev MPU6050
#if MPU6050_ENABLE == 1
MPU6050 		mpu;
uint16_t 		packetSize;
uint16_t 		fifoCount;
uint8_t	 		fifoBuffer[64];
Quaternion 		q;
VectorFloat 	gravity;
float 			ypr[3];
#endif

//PID
void PID_TrueAngle();
void PID_AngleMotion();

int16_t pid_ta[3];											//!< define PID with true angle
int16_t pid_am[3];											//!< define PID with angle motion

float 	_pid_ta[3][3];										//!<  define individual PID for axis [axis][PID]
float 	_pid_am[3][3];										//!<  define individual PID for axis [axis][PID]

float	pid_gain_ta[3][3];									//!< define pid gain [axis][gain] for PID with true angles
float	pid_gain_am[3][3];									//!< define pid gain [axis][gain] for PID with angle motion

float 	error[3];											//!<  define error
float 	previous_error[3];									//!<  define previous error for D-Gain


void setMotorSpeed();

uint16_t motor_speed[4];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	pid_gain_ta[0][0] = 0;
	pid_gain_ta[0][1] = 0;
	pid_gain_ta[0][2] = 0;

	pid_gain_ta[1][0] = 15;
	pid_gain_ta[1][1] = 0;
	pid_gain_ta[1][2] = 0;

	pid_gain_ta[2][0] = 15;
	pid_gain_ta[2][1] = 0;
	pid_gain_ta[2][2] = 0;

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
  /* USER CODE BEGIN 2 */

  // INIT MOTORS
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

  // INIT GIMBAL
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

  //START LOGIC LEVEL CONVERTER
  HAL_GPIO_WritePin(LLVOE_GPIO_Port, LLVOE_Pin, GPIO_PIN_SET);

  if(HAL_GPIO_ReadPin(USBOSD_GPIO_Port, USBOSD_Pin) == GPIO_PIN_RESET){
	  osdusb = USB_MODE;
	  HAL_GPIO_WritePin(MOD0_LED_GPIO_Port, MOD0_LED_Pin, GPIO_PIN_SET);
  }else{
	  osdusb = OSD_MODE;
	  HAL_GPIO_WritePin(MOD1_LED_GPIO_Port, MOD1_LED_Pin, GPIO_PIN_SET);
  }

  //TEST DAC
  HAL_TIM_Base_Start(&htim6);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)sine, 256, DAC_ALIGN_8B_R);

  //init IMU
#if ICM20689_ENABLE == 1
  imu.Initalize();

  if(imu.init_ok == false){
	  for(;;){
		  HAL_GPIO_TogglePin(INIT_OK_GPIO_Port, INIT_OK_Pin);
		  HAL_Delay(100);
	  }
  }
#endif

  HAL_GPIO_WritePin(INIT_OK_GPIO_Port, INIT_OK_Pin, GPIO_PIN_SET);

#if ICM20689_OFFSET_FIND == 1 and ICM20689_ENABLE == 1
  imu.FindOffset();
#endif
  if (ARM_CM_DWT_CTRL != 0) {                  // See if DWT is available
	  ARM_CM_DEMCR      |= 1 << 24;            // Set bit 24
	  ARM_CM_DWT_CYCCNT  = 0;
	  ARM_CM_DWT_CTRL   |= 1 << 0;             // Set bit 0
  }

  //I2cdev MPU6050
#if MPU6050_ENABLE == 1
    mpu.initialize();
  	mpu.dmpInitialize();
  	mpu.setXGyroOffset(82);
  	mpu.setYGyroOffset(-23);
  	mpu.setZGyroOffset(-25);

  	mpu.setXAccelOffset(686);
  	mpu.setYAccelOffset(-3251);
  	mpu.setZAccelOffset(1029);
  	mpu.setDMPEnabled(true);
  	packetSize = mpu.dmpGetFIFOPacketSize();
  	fifoCount = mpu.getFIFOCount();
#endif
  	//init RF24
  	const uint64_t pipe = 0xE8E8F0F0E2;
  	radio.begin();
  	radio.setPayloadSize(32);
	radio.setChannel(125);
	radio.setDataRate(RF24_250KBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setAutoAck(true);
	radio.enableDynamicPayloads();
	radio.enableAckPayload();
  	radio.openReadingPipe(1, pipe);

#if TEST_NRF_T == 1
  	radio.openWritingPipe(pipe);
#endif
  	radio.printDetails();
  	HAL_Delay(1000);

#if TEST_NRF_T == 0
  	radio.startListening();
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  start = ARM_CM_DWT_CYCCNT;
#if TEST_NRF_R == 1
	if (radio.available()){
		radio.read(data, 1);
		HAL_UART_Transmit(&huart1,data,1,100);
		HAL_UART_Transmit(&huart1,(uint8_t *)"\n",1,100);
	}else{
		//HAL_UART_Transmit(&huart1,(uint8_t *)"NODATA\n",8,100);
	}
#endif

#if TEST_NRF_T == 1
	radio.write(data,1);
#endif

#if ICM20689_ENABLE == 1
	  char txt[32];

	  imu.ReadGyro();
	  imu.ReadAccel();

	  float fullvec = sqrt(pow(imu.accel[0],2) + pow(imu.accel[1],2) + pow(imu.accel[2],2));
	  float acangle[2];

	  acangle[0] = asin(imu.accel[0]/fullvec) * -57.29577951;
	  acangle[1] = asin(imu.accel[1]/fullvec) * 57.29577951;
	  //imu.ReadTemp();

	  lptime = 0.001325;

	  imu.t_ypr[0] += imu.ypr[0]*lptime;
	  imu.t_ypr[1] += imu.ypr[1]*lptime;
	  imu.t_ypr[2] += imu.ypr[2]*lptime;

	  imu.t_ypr[0] = imu.t_ypr[0] * 0.96 + acangle[1] * 0.04;
	  imu.t_ypr[1] = imu.t_ypr[1] * 0.96 + acangle[0] * 0.04;

	  HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "throttle: %u \t", recvData.throttle),100);
	  HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "pitch   : %u \t", recvData.pitch),100);
	  HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "roll    : %u \t", recvData.roll),100);

	  //HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "GYROX	 : %2.3f \t", imu.t_ypr[0]),100);
	  //HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "GYROY	 : %2.3f \t", imu.t_ypr[1]),100);
	  //HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "GYROZ	 : %2.3f \t", imu.t_ypr[2]),100);
	  //HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "%2.3f \n\r", lptime),100);

//	  HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "TEMP: %2.3f \t \n\r", imu.temp),100);

	  for (uint8_t i = 0;  i < 3; i++){
		  previous_error[i] = error[i];
	  }

	  for (uint8_t i = 0;  i < 3; i++){
		  error[i] = imu.t_ypr[i];
	  }

	  //HAL_Delay(10);
#endif

#if MPU6050_ENABLE == 1
	  while (fifoCount < packetSize) {
		  fifoCount = mpu.getFIFOCount();
	  }

	  if (fifoCount >= 1024) {
		  mpu.resetFIFO();
	  }
	  else{
		if (fifoCount % packetSize != 0) {
			mpu.resetFIFO();
			fifoCount = mpu.getFIFOCount();
		}
		else{
			while (fifoCount >= packetSize) {
				mpu.getFIFOBytes(fifoBuffer,packetSize);
				fifoCount -= packetSize;
			}
			mpu.dmpGetQuaternion(&q,fifoBuffer);
			mpu.dmpGetGravity(&gravity,&q);
			mpu.dmpGetYawPitchRoll(ypr,&q,&gravity);

			char txt[32];

			HAL_UART_Transmit(&huart1,(uint8_t*)txt,sprintf(txt, "GYROX: %2.3f \t", ypr[1]*180/PI),100);
			HAL_UART_Transmit(&huart1,(uint8_t*)txt,sprintf(txt, "GYROY: %2.3f \t", ypr[2]*180/PI),100);
			HAL_UART_Transmit(&huart1,(uint8_t*)txt,sprintf(txt, "GYROZ: %2.3f \n\r", ypr[0]*180/PI),100);
			}
	  }
	HAL_Delay(50);
#endif

#if PID_ANGLE_MOTION == 1
	PID_AngleMotion();
#endif

#if PID_TRUE_ANGLE == 1
	//error[0] = imu.t_ypr[2] - recvData.yaw;
	error[1] = recvData.pitch - imu.t_ypr[1];
	error[2] = recvData.roll - imu.t_ypr[0];

	PID_TrueAngle();
#endif
	loopRadio();
	setMotorSpeed();

	stop = ARM_CM_DWT_CYCCNT;
	lptime = (stop - start)/216000000.0;
	//HAL_Delay(500); //TtODO: Uncomment when in flightmode

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
  RCC_OscInitStruct.PLL.PLLN = 400;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
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
void PID_TrueAngle(){
	//caculate_PID
	for(uint8_t i = 0; i < 3; i++){
		//P-Regler
		_pid_ta[i][0] 			= 	error[i] * pid_gain_ta[i][0];
		//I-Regler
		_pid_ta[i][1] 			+= 	error[i] * pid_gain_ta[i][1];
		//D-Regler
		_pid_ta[i][2] 			= 	pid_gain_ta[i][2] * (error[i]-previous_error[i]);

		pid_ta[i] 		 		=	_pid_ta[i][0] + _pid_ta[i][1] + _pid_ta[i][2];
	}

	for (uint8_t i = 0; i<3;i++){
		if(pid_ta[i] > 400){
			pid_ta[i] = 400;
		}

		if(pid_ta[i] < -400){
			pid_ta[i] = -400;
		}
	}
}

void PID_AngleMotion(){
	//caculate_PID
	for(uint8_t i = 0; i < 3; i++){
		//P-Regler
		_pid_am[i][0] 			= 	error[i] * pid_gain_am[i][0];
		//I-Regler
		_pid_am[i][1] 			+= 	error[i] * pid_gain_am[i][1];
		//D-Regler
		_pid_am[i][2] 			= 	pid_gain_am[i][2] * (error[i]-previous_error[i]);

		pid_am[i] 		 		=	_pid_am[i][0] + _pid_am[i][1] + _pid_am[i][2];
	}

	for (uint8_t i = 0; i<3;i++){
		if(pid_am[i] > 400){
			pid_am[i] = 400;
		}

		if(pid_am[i] < -400){
			pid_am[i] = -400;
		}
	}
}

void setMotorSpeed(){
	if (recvData.throttle < 100){
		for (int i = 0; i < 4 ;i++){
			motor_speed[i] = 1024 + 10;
		}
	}
	else{
#if PID_TRUE_ANGLE == 1
	motor_speed[0] = (1024 + recvData.throttle) + pid_ta[1] + pid_ta[2] - pid_ta[0];
	motor_speed[1] = (1024 + recvData.throttle) + pid_ta[1] - pid_ta[2] + pid_ta[0];
	motor_speed[2] = (1024 + recvData.throttle) - pid_ta[1] - pid_ta[2] - pid_ta[0];
	motor_speed[3] = (1024 + recvData.throttle) - pid_ta[1] + pid_ta[2] + pid_ta[0];
#endif

#if PID_ANGLE_MOTION == 1
	motor_speed[0] = (1024 + recvData.throttle) + pid_am[1] + pid_am[2] - pid_am[0];
	motor_speed[1] = (1024 + recvData.throttle) + pid_am[1] - pid_am[2] + pid_am[0];
	motor_speed[2] = (1024 + recvData.throttle) - pid_am[1] - pid_am[2] - pid_am[0];
	motor_speed[3] = (1024 + recvData.throttle) - pid_am[1] + pid_am[2] + pid_am[0];
#endif

#if PID_TRUE_ANGLE == 1 &&  PID_ANGLE_MOTION == 1
	motor_speed[0] = (float)(0.5 * ((1024 + recvData.throttle) + pid_ta[1] + pid_ta[2] - pid_ta[0]) + 0.5 * ((1024 + recvData.throttle) + pid_am[1] + pid_am[2] - pid_am[0]));
	motor_speed[1] = (float)(0.5 * ((1024 + recvData.throttle) + pid_ta[1] - pid_ta[2] + pid_ta[0]) + 0.5 * ((1024 + recvData.throttle) + pid_am[1] - pid_am[2] + pid_am[0]));
	motor_speed[2] = (float)(0.5 * ((1024 + recvData.throttle) - pid_ta[1] - pid_ta[2] - pid_ta[0]) + 0.5 * ((1024 + recvData.throttle) - pid_am[1] - pid_am[2] - pid_am[0]));
	motor_speed[3] = (float)(0.5 * ((1024 + recvData.throttle) - pid_ta[1] + pid_ta[2] + pid_ta[0]) + 0.5 * ((1024 + recvData.throttle) - pid_am[1] + pid_am[2] + pid_am[0]));
#endif
	}

	for (int i = 0; i < 4 ;i++){
		if (motor_speed[i] > 2048){
			motor_speed[i] = 2048;
		}
		else if(motor_speed[i] < 1024){
			motor_speed[i] = 1024;
		}
	}

	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,motor_speed[0]);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,motor_speed[1]);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,motor_speed[2]);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,motor_speed[3]);
}

void loopRadio(){
	if(radio.available()){
		radio.read(&recvData,sizeof(RadioData));
		radio.writeAckPayload(1,&ackData,sizeof(AckData));
	}
	else{

	}
}
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
