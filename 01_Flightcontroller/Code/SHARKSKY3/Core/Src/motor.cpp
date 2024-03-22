/*
 * motor.cpp
 *
 *  Created on: Mar 6, 2024
 *      Author: pluscrafter
 */

#include "motor.h"

motor::motor(){

}

void motor::setMotor(){
	if (THROTTLE < 100){
		for (int i = 0; i < 4 ;i++){
			motor_speed[i] = 1024;
		}
	}else {
		motor_speed[0] = (1024 + THROTTLE) + R_CORR + P_CORR + Y_CORR;
		motor_speed[1] = (1024 + THROTTLE) + R_CORR - P_CORR - Y_CORR;
		motor_speed[2] = (1024 + THROTTLE) - R_CORR - P_CORR + Y_CORR;
		motor_speed[3] = (1024 + THROTTLE) - R_CORR + P_CORR - Y_CORR;

		for (int i = 0; i < 4 ;i++){
			if (motor_speed[i] > 2048){
				motor_speed[i] = 2048;
			}
			else if(motor_speed[i] < 1024){
				motor_speed[i] = 1024;
			}
		}
	}

	tim2_motor.PWM_setDutyCycle(1, motor_speed[0]);
	tim2_motor.PWM_setDutyCycle(2, motor_speed[1]);
	tim2_motor.PWM_setDutyCycle(3, motor_speed[2]);
	tim2_motor.PWM_setDutyCycle(4, motor_speed[3]);

}

