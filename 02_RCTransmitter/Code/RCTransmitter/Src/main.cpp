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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "eth.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_hd44780.h"
#include <string.h>
#include<stdio.h>
#include "RF24.h"
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
uint32_t 				adc3[6];								//!< define adc values

LCD_HD44780 			lcd;									//!< define lcd

//Radio
RF24 					radio(GPIOD, GPIO_PIN_15 ,GPIOD , GPIO_PIN_14, &hspi1);
//const uint8_t 			addresses[][6] = {"1Node","2Node"};

//uint8_t data[1] = { 0x41};

//! Acknowlegement struct
struct AckData{
	int16_t 			yaw;									//!<2 bytes 2
	int16_t 			pitch;									//!<2 bytes 4
	int16_t 			roll;									//!<2 bytes 6

	uint16_t 			heading;								//!<2 bytes 8
	uint32_t 			altitude; 								//!<4 bytes 12

	uint32_t 			LV03x;									//!<4 byte 16
	uint32_t 			LV03y;									//!<4 byte 20

	uint16_t 			flags;									//!<2 bytes 22
	uint32_t			uptdate_time; 							//!<4 bytes 26
};

//! receive message struct
struct RadioData{
	int16_t				yaw;									//!<2 bytes 2
	int16_t				pitch;									//!<2 bytes 4
	int16_t				roll;									//!<2 bytes 6
	uint16_t			throttle;								//!<2 bytes 8

	uint16_t			rp_P;									//!<2 bytes 10
	uint16_t			rp_I;									//!<2 bytes 12
	uint16_t			rp_D;									//!<2 bytes 14

	uint16_t			y_P;									//!<2 bytes 16
	uint16_t			y_I;									//!<2 bytes 18
	uint16_t			y_D;									//!<2 bytes 20

	uint16_t			flags;									//!<2 bytes 22
	uint32_t			data;									//!<4 bytes 26
};

AckData 		ackData;										//!< define acknowlegement data
RadioData 		transData;										//!< define transmit data
RadioData		_transData;

int page = 0;
int t = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void loopRadio();												//!< function: read from radio buffer
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_ADC3_Init();
  MX_SDMMC1_SD_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_I2C1_Init();
  MX_ETH_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  //start timer2 for read from adc
  HAL_TIM_Base_Start(&htim2);
  //start adc dma with circular reading
  HAL_ADC_Start_DMA(&hadc3, adc3, 6);

  //init LCD
  lcd.lcdInit(&hi2c1, (uint8_t)0x27, (uint8_t)4, (uint8_t)20);
  lcd.lcdBacklightOn();

  char txt[20];

  //init radio
  const uint64_t pipe = 0xE8E8F0F0E2; //radio adress
  radio.begin();
  radio.setPayloadSize(32);
  radio.setChannel(125);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setAutoAck(true);
  radio.enableDynamicPayloads();
  radio.enableAckPayload();
  radio.openWritingPipe(pipe);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //write adc value to transdata struct
	  transData.throttle = (int16_t)adc3[3];
	  transData.yaw = (int16_t)(((int16_t)adc3[2]- 470) * (30.0/470.0));
	  transData.pitch = (int16_t)(((int16_t)adc3[0]- 470) * (30.0/470.0));
	  transData.roll = (int16_t)(((int16_t)adc3[1]- 470) * (30.0/470.0));

	  //Write to LCD
	  if(page == 0){
		  lcd.lcdSetCursorPosition(0, 0);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"Throttle :%i  %lu  ",transData.throttle, adc3[3]));
		  lcd.lcdSetCursorPosition(0, 1);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"Yaw      :%i  %lu  ",transData.yaw, adc3[2]));
		  lcd.lcdSetCursorPosition(0, 2);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"Pitch    :%i  %lu  ",transData.pitch, adc3[0]));
		  lcd.lcdSetCursorPosition(0, 3);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"Roll     :%i  %lu  ",transData.roll, adc3[1]));
	  }
	  if(page == 1){
		  lcd.lcdSetCursorPosition(0, 0);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"Throttle :%i  %lu  ",transData.throttle, adc3[3]));
		  lcd.lcdSetCursorPosition(0, 1);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"Yaw      :%2.2f",ackData.yaw/100.0));
		  lcd.lcdSetCursorPosition(0, 2);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"Pitch    :%2.2f",ackData.pitch/100.0));
		  lcd.lcdSetCursorPosition(0, 3);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"Roll     :%2.2f",ackData.roll/100.0));
	  }
	  if(page == 2){
		  lcd.lcdSetCursorPosition(0, 0);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"ROLL PITCH PID"));
		  lcd.lcdSetCursorPosition(0, 1);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"rp_P     :%2.2f",transData.rp_P/100.0));
		  lcd.lcdSetCursorPosition(0, 2);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"rp_I     :%2.2f",transData.rp_I/100.0));
		  lcd.lcdSetCursorPosition(0, 3);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"rp_D     :%2.2f",transData.rp_D/100.0));
	  }
	  if(page == 3){
		  lcd.lcdSetCursorPosition(0, 0);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"YAW PID"));
		  lcd.lcdSetCursorPosition(0, 1);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"y_P      :%2.2f",transData.y_P/100.0));
		  lcd.lcdSetCursorPosition(0, 2);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"y_D      :%2.2f",transData.y_I/100.0));
		  lcd.lcdSetCursorPosition(0, 3);
		  lcd.lcdPrintStr((uint8_t*)txt, sprintf(txt,"y_I      :%2.2f",transData.y_D/100.0));
	  }

	  //read radio

	  if(_transData.throttle != transData.throttle || _transData.pitch != transData.pitch  || _transData.roll != transData.roll || _transData.yaw != transData.yaw){
		  loopRadio();
		  loopRadio();
		  loopRadio();
	  }

	  _transData = transData;
	  //rotate lcd
	  if(HAL_GPIO_ReadPin(BTN_U_GPIO_Port, BTN_U_Pin) == 0){
		  HAL_Delay(30);
		  if(HAL_GPIO_ReadPin(BTN_U_GPIO_Port, BTN_U_Pin) == 0){
			  page++;
			  lcd.lcdDisplayClear();
			  if (page > 3){
				  page = 0;
			  }
		  }
	  }

	  t = HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin);

	  if(HAL_GPIO_ReadPin(BTN_D_GPIO_Port, BTN_D_Pin) == 0){
		  HAL_Delay(30);
		  if(HAL_GPIO_ReadPin(BTN_D_GPIO_Port, BTN_D_Pin) == 0){
			  page--;
			  lcd.lcdDisplayClear();
			  if (page < 0){
				  page = 3;
			  }
		  }
	  }

	  t = HAL_GPIO_ReadPin(BTN_L_GPIO_Port, BTN_L_Pin);

	 //pid
	  if(page == 2){
		  if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 0){
			  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
				  HAL_Delay(30);
				  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
					  transData.rp_P += 1*100;
				  }
			  }

			  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
				  HAL_Delay(30);
				  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
					  transData.rp_P -= 1*100;
				  }
			  }
		  }

		  if(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 0){
			  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
				  HAL_Delay(30);
				  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
					  transData.rp_I += 1*1;
				  }
			  }



			  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
				  HAL_Delay(30);
				  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
					  transData.rp_I -= 1*1;
				  }
			  }
		  }

		  if(HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == 0){
			  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
				  HAL_Delay(30);
				  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
					  transData.rp_D += 1*100;
				  }
			  }



			  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
				  HAL_Delay(30);
				  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
					  transData.rp_D -= 1*100;
				  }
			  }
		  }
	  }


	  if(page == 3){
	  		  if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 0){
	  			  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
	  				  HAL_Delay(30);
	  				  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
	  					  transData.y_P += 1*100;
	  				  }
	  			  }

	  			  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
	  				  HAL_Delay(30);
	  				  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
	  					  transData.y_P -= 1*100;
	  				  }
	  			  }
	  		  }

	  		  if(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 0){
	  			  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
	  				  HAL_Delay(30);
	  				  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
	  					  transData.y_I += 1*1;
	  				  }
	  			  }



	  			  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
	  				  HAL_Delay(30);
	  				  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
	  					  transData.y_I -= 1*1;
	  				  }
	  			  }
	  		  }

	  		  if(HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == 0){
	  			  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
	  				  HAL_Delay(30);
	  				  if(HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) == 0){
	  					  transData.y_D += 1*100;
	  				  }
	  			  }



	  			  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
	  				  HAL_Delay(30);
	  				  if(HAL_GPIO_ReadPin(BTN_M_GPIO_Port, BTN_M_Pin) == 0){
	  					  transData.y_D -= 1*100;
	  				  }
	  			  }
	  		  }

	  	  }

	  //lcd.lcdDisplayClear();
	  //HAL_Delay(1);
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

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void loopRadio(){
	radio.write(&transData,sizeof(RadioData));

	if(radio.isAckPayloadAvailable()){
		radio.read(&ackData,radio.getDynamicPayloadSize());
	}
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
