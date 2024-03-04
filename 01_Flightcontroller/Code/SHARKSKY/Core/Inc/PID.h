/*
 * PID.h
 *
 *  Created on: 29.08.2020
 *      Author: Pluscrafter
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include <stdint.h>

float f_lastError = 0;
float i_lastError = 0;

void f_PID_P(float gain, float *error, float *corr);
void f_PID_I(float gain, float *error, float *corr);
void f_PID_D(float gain, float *error, float *corr);

void i_PID_P(float gain, int16_t *error, int16_t *corr);
void i_PID_I(float gain, int16_t *error, int16_t *corr);
void i_PID_D(float gain, int16_t *error, int16_t *corr);

#endif /* INC_PID_H_ */
