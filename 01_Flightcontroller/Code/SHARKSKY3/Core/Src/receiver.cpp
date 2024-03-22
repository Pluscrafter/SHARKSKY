/*
 * receiver.cpp
 *
 *  Created on: Mar 5, 2024
 *      Author: pluscrafter
 */

#include "receiver.h"

receiver::receiver(){

}

void receiver::getVal(){
	r_throttle = (uint16_t)(tim3_controller.CH_3_VAL) - 1499;
	r_yaw = (uint16_t)(tim4_controller.CH_3_VAL) - 1499;
	r_pitch = (uint16_t)(tim3_controller.CH_2_VAL) - 1499;
	r_roll = (uint16_t)(tim3_controller.CH_1_VAL) - 1499;

	throttle = (uint16_t)((r_throttle/1500) * 1024);

	yaw = ((r_yaw/1500) * 90) - 45;
	pitch = ((r_pitch/1500) * 90) - 45;
	roll = ((r_roll/1500) * 90) - 45;
}
