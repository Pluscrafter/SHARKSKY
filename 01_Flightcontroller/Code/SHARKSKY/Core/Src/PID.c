/*
 * PID.c
 *
 *  Created on: 29.08.2020
 *      Author: Pluscrafter
 */

#include "PID.h"
extern float looptime;


void f_PID_P(float gain, float *error, float *corr){
	*corr = gain * *error;
}

void f_PID_I(float gain, float *error, float *corr){
	*corr += *error * gain * looptime;
}

void f_PID_D(float gain, float *error, float *corr){
	*corr = gain * ((*error - f_lastError) * looptime);
	f_lastError = *error;
}

void i_PID_P(float gain, int16_t *error, int16_t *corr){
	*corr = gain * *error;
}

void i_PID_I(float gain, int16_t *error, int16_t *corr){
	*corr += *error * gain * looptime;
}

void i_PID_D(float gain, int16_t *error, int16_t *corr){
	*corr = gain * ((*error - i_lastError) * looptime);
	i_lastError = *error;
}

