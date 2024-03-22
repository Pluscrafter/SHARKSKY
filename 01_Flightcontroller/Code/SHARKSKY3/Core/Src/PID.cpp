/*
 * PID.cpp
 *
 *  Created on: Mar 5, 2024
 *      Author: pluscrafter
 */




#include "PID.h"


float PID::calc(float setpoint, float meas){
	float error = setpoint - meas;

	_P = Kp * error;

	integral += error * dt;

	_I = Ki * integral;

	de = (error - prev_error) / dt;
	_D = Kd * de;

	output = _P + _I + _D;

	if(output > max){
		output = max;
	} else if(output < min){
		output = min;
	}

	prev_error = error;

	return output;
}
