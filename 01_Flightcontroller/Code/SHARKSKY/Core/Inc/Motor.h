/*
 * Motor.h
 *
 *  Created on: 29.08.2020
 *      Author: Pluscrafter
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdint.h>
#include "tim.h"

void setMotorSpeed(uint8_t motor, uint16_t speed);
void setMotorsSpeed(uint16_t speed_1, uint16_t speed_2, uint16_t speed_3, uint16_t speed_4);
void _setMotorsSpeed(uint16_t speed);
void calibrateMotors();

#endif /* INC_MOTOR_H_ */
