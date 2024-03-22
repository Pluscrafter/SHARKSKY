/*
 * motor.h
 *
 *  Created on: Mar 5, 2024
 *      Author: pluscrafter
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "Instances.hpp"

class motor{
public:
	motor();
	void setMotor();
	int16_t Y_CORR, P_CORR, R_CORR, THROTTLE;
private:
	int16_t motor_speed[4];
};



#endif /* INC_MOTOR_H_ */
