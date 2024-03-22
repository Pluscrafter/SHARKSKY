/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RF24_MOSI_Pin GPIO_PIN_1
#define RF24_MOSI_GPIO_Port GPIOC
#define RF24_MISO_Pin GPIO_PIN_2
#define RF24_MISO_GPIO_Port GPIOC
#define MOTOR_1_Pin GPIO_PIN_0
#define MOTOR_1_GPIO_Port GPIOA
#define MOTOR_2_Pin GPIO_PIN_1
#define MOTOR_2_GPIO_Port GPIOA
#define GPS_TX_Pin GPIO_PIN_2
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_3
#define GPS_RX_GPIO_Port GPIOA
#define IMU_NSS_Pin GPIO_PIN_4
#define IMU_NSS_GPIO_Port GPIOA
#define INIT_OK_Pin GPIO_PIN_7
#define INIT_OK_GPIO_Port GPIOA
#define MOD0_LED_Pin GPIO_PIN_4
#define MOD0_LED_GPIO_Port GPIOC
#define GIMBAL_X_Pin GPIO_PIN_0
#define GIMBAL_X_GPIO_Port GPIOB
#define MOD1_LED_Pin GPIO_PIN_1
#define MOD1_LED_GPIO_Port GPIOB
#define IMU_MOSI_Pin GPIO_PIN_2
#define IMU_MOSI_GPIO_Port GPIOB
#define MOTOR_3_Pin GPIO_PIN_10
#define MOTOR_3_GPIO_Port GPIOB
#define MOTOR_4_Pin GPIO_PIN_11
#define MOTOR_4_GPIO_Port GPIOB
#define RF24_NSS_Pin GPIO_PIN_12
#define RF24_NSS_GPIO_Port GPIOB
#define RF24_SCK_Pin GPIO_PIN_13
#define RF24_SCK_GPIO_Port GPIOB
#define CE_Pin GPIO_PIN_6
#define CE_GPIO_Port GPIOC
#define IRQ_Pin GPIO_PIN_7
#define IRQ_GPIO_Port GPIOC
#define IRQ_EXTI_IRQn EXTI9_5_IRQn
#define ARDU_SDA_Pin GPIO_PIN_9
#define ARDU_SDA_GPIO_Port GPIOC
#define ARDU_SCL_Pin GPIO_PIN_8
#define ARDU_SCL_GPIO_Port GPIOA
#define FTDI_TX_Pin GPIO_PIN_9
#define FTDI_TX_GPIO_Port GPIOA
#define FTDI_RX_Pin GPIO_PIN_10
#define FTDI_RX_GPIO_Port GPIOA
#define LLVOE_Pin GPIO_PIN_11
#define LLVOE_GPIO_Port GPIOA
#define USBOSD_Pin GPIO_PIN_12
#define USBOSD_GPIO_Port GPIOA
#define IMU_SCK_Pin GPIO_PIN_10
#define IMU_SCK_GPIO_Port GPIOC
#define IMU_MISO_Pin GPIO_PIN_11
#define IMU_MISO_GPIO_Port GPIOC
#define GIMBAL_Y_Pin GPIO_PIN_4
#define GIMBAL_Y_GPIO_Port GPIOB
#define GIMBAL_Z_Pin GPIO_PIN_5
#define GIMBAL_Z_GPIO_Port GPIOB
#define IMU2_SCL_Pin GPIO_PIN_6
#define IMU2_SCL_GPIO_Port GPIOB
#define IMU2_SDA_Pin GPIO_PIN_7
#define IMU2_SDA_GPIO_Port GPIOB
#define INTA_Pin GPIO_PIN_8
#define INTA_GPIO_Port GPIOB
#define INTA_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */