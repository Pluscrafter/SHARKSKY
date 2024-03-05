/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f722xx.h"
#include "core_cm7.h"
#include "Sensors.hpp"
#include "Instances.hpp"
#include "GPIO.hpp"
#include "ICM20689.h"

#define USER_CTRL			0x6A
#define PWR_MGMT_1			0x6B
#define WHO_AM_I			0x75

void delay (uint32_t time){
	for(volatile uint32_t i = 0; i < time; i++);
}

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

struct AckData{
	int16_t 				yaw;									//!< 2 bytes 2
	int16_t 				pitch;									//!< 2 bytes 4
	int16_t 				roll;									//!< 2 bytes 6

	uint16_t 				heading;								//!< 2 bytes 8
	uint32_t 				altitude; 								//!< 4 bytes 12

	uint32_t 				LV03x;									//!< 4 byte 16
	uint32_t 				LV03y;									//!< 4 byte 20

	uint16_t 				flags;									//!< 2 bytes 22
	uint32_t				uptdate_time; 							//!< 4 bytes 26
};

//! receive message struct
struct RadioData{
	int16_t					yaw;									//!< 2 bytes 2
	int16_t					pitch;									//!< 2 bytes 4
	int16_t					roll;									//!< 2 bytes 6
	uint16_t				throttle;								//!< 2 bytes 8

	uint16_t				rp_P;									//!<2 bytes 10
	uint16_t				rp_I;									//!<2 bytes 12
	uint16_t				rp_D;									//!<2 bytes 14

	uint16_t				y_P;									//!<2 bytes 16
	uint16_t				y_I;									//!<2 bytes 18
	uint16_t				y_D;									//!<2 bytes 20

	uint16_t				flags;									//!< 2 bytes 10
	uint32_t				data;									//!< 4 bytes 14
};

AckData 					ackData;								//!< define acknowlegement data
RadioData 					recvData;								//!< define receive data
//RF24 radio(GPIOC, 6);		//!< define radio (nRF24L01)

float looptime = 0.001;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();

	//start DWT counter also when not in Debug mode to count clock ticks
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	//https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter
	DWT->LAR = 0xC5ACCE55; 							//https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter 12.10.19 02:03
	DWT->CYCCNT = 0;								//https://www.carminenoviello.com/2015/09/04/precisely-measure-microseconds-stm32/ 12.10.19 01:30
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	GPIO::EN(GPIOC);
	GPIO::EN(GPIOB);
	GPIO::MODE(GPIOC, 4, MODE_OUTPUT);
	GPIO::MODE(GPIOB, 1, MODE_OUTPUT);

	imu.IMU_init_ok = false;

	spi2.Init();
	spi3.Init();
	//uart1.Init();
	tim2_motor.Init();
	tim3_controller.Init();
	tim4_controller.Init();
	tim5_trig.Init();
	tim7_trig.Init();
	tim5_trig.EnableUpdateInterrupt();
	tim7_trig.EnableUpdateInterrupt();

	tim2_motor.EnableChannel(GPIOA, 0, GPIO_AF1_TIM2, 1);
	tim2_motor.EnableChannel(GPIOA, 1, GPIO_AF1_TIM2, 2);
	tim2_motor.EnableChannel(GPIOB, 10, GPIO_AF1_TIM2, 3);
	tim2_motor.EnableChannel(GPIOB, 11, GPIO_AF1_TIM2, 4);

	tim2_motor.EnablePWM(1);
	tim2_motor.EnablePWM(2);
	tim2_motor.EnablePWM(3);
	tim2_motor.EnablePWM(4);
	tim2_motor.TIM_Start();

	tim2_motor.PWM_setDutyCycle(1, 50);
	tim2_motor.PWM_setDutyCycle(2, 4000);
	tim2_motor.PWM_setDutyCycle(3, 3072);
	tim2_motor.PWM_setDutyCycle(4, 2000);

	tim3_controller.EnableChannel(GPIOB, 4, GPIO_AF2_TIM3, 1); //CH 1 ROLL
	tim3_controller.EnableChannel(GPIOB, 5, GPIO_AF2_TIM3, 2); //CH 2 PITCH
	tim3_controller.EnableChannel(GPIOB, 0, GPIO_AF2_TIM3, 3); //CH 3 THROTTLE
	tim4_controller.EnableChannel(GPIOB, 8, GPIO_AF2_TIM4, 3); //CH 3 THROTTLE

	tim3_controller.EnableInputCapture(1);
	tim3_controller.EnableInputCapture(2);
	tim3_controller.EnableInputCapture(3);
	tim4_controller.EnableInputCapture(3);

	tim3_controller.TIM_Start();
	tim4_controller.TIM_Start();

	tim5_trig.TIM_Start();
	tim7_trig.TIM_Start();



	imu.IMU_init_ok = imu.IMU_Init();
	if(imu.IMU_init_ok){
		GPIO::WRITE(GPIOC, 4, HIGH);
	}

	//imu.IMU_startINT();

	while (1){
		double start = DWT->CYCCNT;
		//imu.IMU_calcAngle();
		imu.IMU_readAngle();

		double stop = DWT->CYCCNT;
		looptime = (stop - start)/216000000.0;
	}

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
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

  /** Initializes the CPU, AHB and APB buses clocks
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
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  /*__disable_irq();
  while (1)
  {
  }*/
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
