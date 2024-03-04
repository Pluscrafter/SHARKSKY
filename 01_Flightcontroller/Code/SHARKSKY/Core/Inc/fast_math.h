/*
 * fast_math.h
 *
 *  Created on: 29.08.2020
 *      Author: Pluscrafter
 */

#ifndef INC_FAST_MATH_H_
#define INC_FAST_MATH_H_

#include <stdint.h>

float ff_fastSin(float val);
float ff_fastASin(float val);
float ff_fastSqrt(float val);

float fi_fastSin(int val);
float fi_fastASin(int val);
float fi_fastSqrt(int val);

int16_t if_fastSin(float val);
int16_t if_fastASin(float val);
int16_t if_fastSqrt(float val);

int16_t ii_fastSin(int val);
int16_t ii_fastASin(int val);
int16_t ii_fastSqrt(int val);


#endif /* INC_FAST_MATH_H_ */
