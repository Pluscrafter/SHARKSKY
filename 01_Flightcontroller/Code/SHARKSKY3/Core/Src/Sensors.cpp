#include "Sensors.hpp"


SENSORS::IMU imu = SENSORS::IMU();
receiver recv = receiver();
PID YAW_PID	 	= PID(0.001, 600, -600, 100, 0.001, 10);
PID PITCH_PID	= PID(0.001, 600, -600, 100, 0.001, 10);
PID ROLL_PID	= PID(0.001, 600, -600, 100, 0.001, 10);
motor mot		= motor();
