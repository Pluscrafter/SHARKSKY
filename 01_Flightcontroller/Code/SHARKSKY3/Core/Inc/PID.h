/*
 * PID.h
 *
 *  Created on: Mar 5, 2024
 *      Author: pluscrafter
 */

#ifndef INC_PID_H_
#define INC_PID_H_

class PID{
private:
	float dt, max, min, Kp, Ki, Kd;
	float _P, _I, _D, error, integral, prev_error, de, output;

public:
	PID(float dt, float max, float min, float Kp, float Ki, float Kd):
		dt(dt), max(max), min(min) ,Kp(Kp), Ki(Ki), Kd(Kd) {};

	float calc(float setpoint, float meas);
};



#endif /* INC_PID_H_ */
