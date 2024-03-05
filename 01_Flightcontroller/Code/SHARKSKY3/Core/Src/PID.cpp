/*
 * PID.cpp
 *
 *  Created on: Mar 5, 2024
 *      Author: pluscrafter
 */




#include "PID.h"
extern float looptime;

float PID::f_lastError = 0;
float PID::i_lastError = 0;

void PID::PID_P(float gain, float error, float &corr){
	corr = gain * error;
}

void PID::PID_I(float gain, float error, float &corr){
	corr += error * gain * looptime;
}

void PID::PID_D(float gain, float error, float &corr){
	corr = gain * ((error - f_lastError) * looptime);
	f_lastError = error;
}
