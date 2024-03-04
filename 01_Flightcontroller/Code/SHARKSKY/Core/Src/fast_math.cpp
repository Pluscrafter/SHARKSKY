/*
 * fast_math.c
 *
 *  Created on: 29.08.2020
 *      Author: Pluscrafter
 */

#include "fast_math.h"

float ff_fastSin(float val){
	return val - ((val*val*val)/6) + ((val*val*val*val*val)/120) - ((val*val*val*val*val*val*val)/5040);
}

float ff_fastASin(float val){
	return val+(0.5)*((val*val*val)/3)+(0.375)*((val*val*val*val*val)/5)+(0.3125)*((val*val*val*val*val*val*val)/7);
}

float ff_fastSqrt(float val){
	unsigned int i = *(unsigned int*) &val;
	// adjust bias
	i  += 127 << 23;
	// approximation of square root
	i >>= 1;
	return *(float*) &i;
}

float fi_fastSin(int val){
	return val - ((val*val*val)/6) + ((val*val*val*val*val)/120) - ((val*val*val*val*val*val*val)/5040);
}

float fi_fastASin(int val){
	return val+(0.5)*((val*val*val)/3)+(0.375)*((val*val*val*val*val)/5)+(0.3125)*((val*val*val*val*val*val*val)/7);
}

float fi_fastSqrt(int val){
	unsigned int i = *(unsigned int*) &val;
	// adjust bias
	i  += 127 << 23;
	// approximation of square root
	i >>= 1;
	return *(float*) &i;
}

int16_t if_fastSin(float val){
	return val - ((val*val*val)/6) + ((val*val*val*val*val)/120) - ((val*val*val*val*val*val*val)/5040);
}

int16_t if_fastASin(float val){
	return val+(0.5)*((val*val*val)/3)+(0.375)*((val*val*val*val*val)/5)+(0.3125)*((val*val*val*val*val*val*val)/7);
}

int16_t if_fastSqrt(float val){
	unsigned int i = *(unsigned int*) &val;
	// adjust bias
	i  += 127 << 23;
	// approximation of square root
	i >>= 1;
	return *(float*) &i;
}

int16_t ii_fastSin(int val){
	return val - ((val*val*val)/6) + ((val*val*val*val*val)/120) - ((val*val*val*val*val*val*val)/5040);
}

int16_t ii_fastASin(int val){
	return val+(0.5)*((val*val*val)/3)+(0.375)*((val*val*val*val*val)/5)+(0.3125)*((val*val*val*val*val*val*val)/7);
}

int16_t ii_fastSqrt(int val){
	unsigned int i = *(unsigned int*) &val;
	// adjust bias
	i  += 127 << 23;
	// approximation of square root
	i >>= 1;
	return *(float*) &i;
}
