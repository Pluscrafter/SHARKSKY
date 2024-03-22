/*
 * receiver.h
 *
 *  Created on: Mar 5, 2024
 *      Author: pluscrafter
 */

#ifndef INC_RECEIVER_H_
#define INC_RECEIVER_H_

#include "Instances.hpp"

	class receiver{
	public:
		float yaw, pitch, roll;
		uint16_t throttle;

		receiver();
		void getVal();
	private:
		volatile float r_throttle, r_yaw, r_pitch, r_roll;

	};

#endif /* INC_RECEIVER_H_ */
