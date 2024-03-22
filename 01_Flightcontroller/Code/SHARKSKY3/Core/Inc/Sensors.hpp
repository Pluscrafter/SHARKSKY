/*
 * Sensors.hpp
 *
 *  Created on: Mar 1, 2024
 *      Author: pluscrafter
 */

#ifndef INC_SENSORS_HPP_
#define INC_SENSORS_HPP_
#include "ICM20689.h"
#include "receiver.h"
#include "PID.h"
#include "motor.h"

extern SENSORS::IMU imu;
extern receiver recv;
extern PID YAW_PID;
extern PID PITCH_PID;
extern PID ROLL_PID;
extern motor mot;
#endif /* INC_SENSORS_HPP_ */
