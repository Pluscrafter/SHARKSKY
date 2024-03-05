/*
 * PID.h
 *
 *  Created on: Mar 5, 2024
 *      Author: pluscrafter
 */

#ifndef INC_PID_H_
#define INC_PID_H_

class PID{
	static float f_lastError;
	static float i_lastError;
public:
	static void PID_P(float gain, float error, float &corr);
	static void PID_I(float gain, float error, float &corr);
	static void PID_D(float gain, float error, float &corr);
};



#endif /* INC_PID_H_ */
