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
#include "math.h"
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
#include "ICM20689.h"
#include "MPU6050.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define USB_MODE true
#define OSD_MODE true

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

uint8_t sawtooth[256] = {
0, 1, 2, 3, 4, 5, 6, 7,
8, 9, 10, 11, 12, 13, 14, 15,
16, 17, 18, 19, 20, 21, 22, 23,
24, 25, 26, 27, 28, 29, 30, 31,
32, 33, 34, 35, 36, 37, 38, 39,
40, 41, 42, 43, 44, 45, 46, 47,
48, 49, 50, 51, 52, 53, 54, 55,56,
57, 58, 59, 60, 61, 62, 63, 64,
65, 66, 67, 68, 69, 70, 71, 72,
73, 74, 75, 76, 77, 78, 79, 80,
81, 82, 83, 84, 85, 86, 87, 88,
89, 90, 91, 92, 93, 94, 95, 96,
97, 98, 99, 100, 101, 102, 103,
104, 105, 106,107,
108, 109, 110, 111, 112, 113, 114, 115,
116, 117, 118, 119, 120, 121, 122, 123,
124, 125, 126, 127, 128, 129, 130, 131,
132, 133, 134, 135, 136, 137, 138, 139,
140, 141, 142, 143, 144, 145, 146, 147,
148, 149, 150, 151, 152, 153, 154, 155,156,
157, 158, 159, 160, 161, 162, 163, 164,
165, 166, 167, 168, 169, 170, 171, 172,
173, 174, 175, 176, 177, 178, 179, 180,
181, 182, 183, 184, 185, 186, 187, 188,
189, 190, 191, 192, 193, 194, 195, 196,
197, 198, 199, 200, 201, 202, 203, 204,
205, 206, 207, 208, 209,
210, 211, 212, 213, 214, 215,
216, 217, 218, 219, 220, 221, 222, 223,
224, 225, 226, 227, 228, 229, 230, 231,
232, 233, 234, 235, 236, 237, 238, 239,
240, 241, 242, 243, 244, 245, 246, 247,
248, 249, 250, 251, 252, 253, 254, 255};

Sensor::ICM20689 imu;
float 		PI = 3.1415;

uint32_t 	start = 0, stop = 0;
double 		lptime = 0.101;

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
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)sawtooth, 256, DAC_ALIGN_8B_R);

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

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  start = ARM_CM_DWT_CYCCNT;

#if ICM20689_ENABLE == 1
	  char txt[32];

	  imu.ReadGyro();
	  imu.ReadAccel();

	  float fullvec = sqrt(pow(imu.accel[0],2) + pow(imu.accel[1],2) + pow(imu.accel[2],2));
	  float acangle[2];

	  acangle[0] = asin(imu.accel[0]/fullvec) * -57.29577951;
	  acangle[1] = asin(imu.accel[1]/fullvec) * 57.29577951;
	  //imu.ReadTemp();

	  imu.t_ypr[0] += imu.ypr[0]*lptime;
	  imu.t_ypr[1] += imu.ypr[1]*lptime;
	  imu.t_ypr[2] += imu.ypr[2]*lptime;

	  imu.t_ypr[0] = imu.t_ypr[0] * 0.96 + acangle[1] * 0.04;
	  imu.t_ypr[1] = imu.t_ypr[1] * 0.96 + acangle[0] * 0.04;

	  HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "GYROX: %2.3f \t", imu.t_ypr[0]),100);
	  HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "GYROY: %2.3f \t", imu.t_ypr[1]),100);
	  HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "GYROZ: %2.3f \t \n\r", imu.t_ypr[2]),100);

	  //HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "ACCELX: %2.3f \t", imu.accel[0]),100);
	  //HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "ACCELY: %2.3f \t", imu.accel[1]),100);
	  //HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "ACCELZ: %2.3f \t \n\r", imu.accel[2]),100);

	  //HAL_UART_Transmit(&huart1, (uint8_t*)txt,sprintf(txt, "TEMP: %2.3f \t \n\r", imu.temp),100);


	  HAL_Delay(10);
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

	  stop = ARM_CM_DWT_CYCCNT;
	  lptime = (stop - start)/216000000.0;


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
