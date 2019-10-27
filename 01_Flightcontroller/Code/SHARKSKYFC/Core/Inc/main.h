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
#define CELL_4_Pin GPIO_PIN_0
#define CELL_4_GPIO_Port GPIOC
#define CELL_3_Pin GPIO_PIN_1
#define CELL_3_GPIO_Port GPIOC
#define CELL_2_Pin GPIO_PIN_2
#define CELL_2_GPIO_Port GPIOC
#define CELL_1_Pin GPIO_PIN_3
#define CELL_1_GPIO_Port GPIOC
#define RF24_NSS_Pin GPIO_PIN_4
#define RF24_NSS_GPIO_Port GPIOA
#define RF24_SCK_Pin GPIO_PIN_5
#define RF24_SCK_GPIO_Port GPIOA
#define RF24_MISO_Pin GPIO_PIN_6
#define RF24_MISO_GPIO_Port GPIOA
#define RF24_MOSI_Pin GPIO_PIN_7
#define RF24_MOSI_GPIO_Port GPIOA
#define RF24_CE_Pin GPIO_PIN_4
#define RF24_CE_GPIO_Port GPIOC
#define RF24_INT_Pin GPIO_PIN_0
#define RF24_INT_GPIO_Port GPIOB
#define RNG_ADC_Pin GPIO_PIN_1
#define RNG_ADC_GPIO_Port GPIOB
#define GPS_TX_Pin GPIO_PIN_10
#define GPS_TX_GPIO_Port GPIOB
#define GPS_RX_Pin GPIO_PIN_11
#define GPS_RX_GPIO_Port GPIOB
#define SD_NSS_Pin GPIO_PIN_12
#define SD_NSS_GPIO_Port GPIOB
#define SD_SCK_Pin GPIO_PIN_13
#define SD_SCK_GPIO_Port GPIOB
#define SD_MISO_Pin GPIO_PIN_14
#define SD_MISO_GPIO_Port GPIOB
#define SD_MOSI_Pin GPIO_PIN_15
#define SD_MOSI_GPIO_Port GPIOB
#define RPI_SDA_Pin GPIO_PIN_9
#define RPI_SDA_GPIO_Port GPIOC
#define RPI_SCL_Pin GPIO_PIN_8
#define RPI_SCL_GPIO_Port GPIOA
#define UBSOSD_TX_Pin GPIO_PIN_9
#define UBSOSD_TX_GPIO_Port GPIOA
#define USBOSD_RX_Pin GPIO_PIN_10
#define USBOSD_RX_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_11
#define BUZZER_GPIO_Port GPIOA
#define USBOSD_Pin GPIO_PIN_12
#define USBOSD_GPIO_Port GPIOA
#define IMU_NSS_Pin GPIO_PIN_15
#define IMU_NSS_GPIO_Port GPIOA
#define IMU_SCK_Pin GPIO_PIN_10
#define IMU_SCK_GPIO_Port GPIOC
#define IMU_MISO_Pin GPIO_PIN_11
#define IMU_MISO_GPIO_Port GPIOC
#define IMU_MOSI_Pin GPIO_PIN_12
#define IMU_MOSI_GPIO_Port GPIOC
#define IMU_INT_Pin GPIO_PIN_2
#define IMU_INT_GPIO_Port GPIOD
#define LED_3_Pin GPIO_PIN_3
#define LED_3_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_4
#define LED_2_GPIO_Port GPIOB
#define LED_1_Pin GPIO_PIN_5
#define LED_1_GPIO_Port GPIOB
#define ARDU_SCL_Pin GPIO_PIN_6
#define ARDU_SCL_GPIO_Port GPIOB
#define ARDU_SDA_Pin GPIO_PIN_7
#define ARDU_SDA_GPIO_Port GPIOB
#define LLVOE_Pin GPIO_PIN_8
#define LLVOE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
