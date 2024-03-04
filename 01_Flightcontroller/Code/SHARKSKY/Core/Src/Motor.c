/*
 * Motor.c
 *
 *  Created on: 29.08.2020
 *      Author: Pluscrafter
 */

#include "Motor.h"

void setMotorSpeed(uint8_t motor, uint16_t speed){
	switch(motor){
		case 1:
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,speed);
			break;
		case 2:
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed);
			break;
		case 3:
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed);
			break;
		case 4:
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speed);
			break;

		default:
			_setMotorsSpeed(0);
	}

}

void setMotorsSpeed(uint16_t speed_1, uint16_t speed_2, uint16_t speed_3, uint16_t speed_4){
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,speed_1);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_2);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_3);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speed_4);
}

void _setMotorsSpeed(uint16_t speed){
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,speed);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speed);
}

void calibrateMotors(){
	_setMotorsSpeed(2048);
	HAL_Delay(5000);
	_setMotorsSpeed(1024);
	HAL_Delay(5000);
	_setMotorsSpeed(2048);
	HAL_Delay(5000);
	_setMotorsSpeed(1024);
	HAL_Delay(10000);
	_setMotorsSpeed(1124);
	HAL_Delay(2000);
	_setMotorsSpeed(1024);
}
