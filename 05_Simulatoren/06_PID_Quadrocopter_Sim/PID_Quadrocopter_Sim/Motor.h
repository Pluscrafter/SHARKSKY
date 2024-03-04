#pragma once

#include <stdint.h>
#include "constants.h"

const uint16_t minThrottle = 0;
const uint16_t	maxThrottle = 1024;
const float lift = 0;


struct Motor
{
	uint8_t id;
	uint16_t i_throttle;
	float f_v_lift[3];

	void (*throttleing)();
};
