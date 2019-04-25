/*
 * I_I2C.h
 *
 *  Created on: 22.04.2019
 *      Author: Pluscrafter
 */

#ifndef I_I2C_H_
#define I_I2C_H_

#include "stm32f7xx_hal.h"
#include "dma.h"
#include "i2c.h"

namespace Interface {

class I_I2C {
public:
	I_I2C();
	virtual ~I_I2C();
};

} /* namespace Interface */

#endif /* I_I2C_H_ */
