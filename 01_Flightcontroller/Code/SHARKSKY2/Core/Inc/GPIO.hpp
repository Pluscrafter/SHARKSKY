/*
 * GPIO.h
 *
 *  Created on: May 14, 2022
 *      Author: charpoank
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

#define MODE_INPUT 0
#define MODE_OUTPUT 1
#define MODE_ALT 2
#define MODE_ANALOG 3

#define SPEED_LOW 0
#define SPEED_MEDIUM 1
#define SPEED_HIGH 2
#define SPEED_VHIGH 3

#define NO_PUPD 0
#define PULLUP 1
#define PULLDOWN 2

#include "stm32f722xx.h"

class GPIO {
public:

	static void EN(GPIO_TypeDef* gpio);
	static void MODE(GPIO_TypeDef* gpio, uint8_t pin, uint8_t mode);
	static void SPEED(GPIO_TypeDef* gpio, uint8_t pin, uint8_t speed);
	static void PUPD(GPIO_TypeDef* gpio, uint8_t pin, uint8_t pupd);
	static void AFR(GPIO_TypeDef* gpio, uint8_t pin, uint8_t afr);
};

#endif /* SRC_GPIO_H_ */
