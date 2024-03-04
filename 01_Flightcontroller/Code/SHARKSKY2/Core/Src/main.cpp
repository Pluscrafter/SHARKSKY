#include "stm32f722xx.h"
#include "core_cm7.h"
#include "Instances.hpp"

#define USER_CTRL			0x6A
#define PWR_MGMT_1			0x6B
#define WHO_AM_I			0x75

void delay (uint32_t time){
	for(volatile uint32_t i = 0; i < time; i++);
}

void writeRegister(uint8_t reg, uint8_t value){
	uint8_t tmp[2];

	tmp[0] = reg;
	tmp[1] = value;

	spi3.CS_EN();
	spi3.Transmit(tmp, 2);
	spi3.CS_DIS();
}

uint8_t readRegister(uint8_t reg){
	uint8_t tmp[1];

	tmp[0] = reg|0x80;
	uint8_t ret[1];

	spi3.CS_EN();
	spi3.Transmit(tmp, 1);
	spi3.Receive(ret, 1);
	spi3.CS_DIS();

	return ret[0];
}



int main(void){
	SystemInit();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;
	GPIOC->MODER |= GPIO_MODER_MODER4_0;
	GPIOB->MODER |= GPIO_MODER_MODER1_0;
	spi3.Init();
	spi2.Init();
	uart1.Init();

	writeRegister(PWR_MGMT_1, 0x40);		// RESET IMU
	writeRegister(USER_CTRL, 0x10);				// DISABLE I2C
	writeRegister(PWR_MGMT_1, 0x00);			// START IMU

	uint8_t tmp = readRegister(WHO_AM_I);
	if(tmp == 0x98){
		GPIOC->BSRR = GPIO_BSRR_BS4;
	}

	char ttmp[11] = "HELLO Worl";
	uart1.Transmit(ttmp, 11);

	while (1){
		if(tmp == 0x98){
			GPIOC->BSRR = GPIO_BSRR_BS4;
		}else{
			GPIOC->BSRR = GPIO_BSRR_BR4;
		}

		uart1.Transmit(ttmp, 11);
	}

}



