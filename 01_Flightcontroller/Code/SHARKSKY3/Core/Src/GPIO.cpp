/*
 * GPIO.cpp
 *
 *  Created on: May 14, 2022
 *      Author: charpoank
 */

#include "GPIO.hpp"

	void GPIO::EN(GPIO_TypeDef* gpio){
		if(gpio == GPIOA){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOAEN;
		}else if(gpio == GPIOB){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOBEN;
		}else if(gpio == GPIOC){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOCEN;
		}else if(gpio == GPIOD){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIODEN;
		}else if(gpio == GPIOE){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOEEN;
		}else if(gpio == GPIOF){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOFEN;
		}else if(gpio == GPIOG){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOGEN;
		}else if(gpio == GPIOH){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOHEN;
		}else if(gpio == GPIOI){
			RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOIEN;
		}
	}

	void GPIO::MODE(GPIO_TypeDef* gpio, uint8_t pin, uint8_t mode){
		gpio->MODER = (gpio->MODER) | (mode << 2*pin);
	}

	void GPIO::SPEED(GPIO_TypeDef* gpio, uint8_t pin, uint8_t speed){
		gpio->OSPEEDR = (gpio->OSPEEDR) | (speed << 2*pin);
	}

	void GPIO::PUPD(GPIO_TypeDef* gpio, uint8_t pin, uint8_t pupd){
		gpio->PUPDR = (gpio->PUPDR) | (pupd << 2*pin);
	}

	void GPIO::AFR(GPIO_TypeDef* gpio, uint8_t pin, uint8_t afr){
		if(pin <= 7){
			gpio->AFR[0] = gpio->AFR[0] | (afr << 4*pin);
		}else{
			gpio->AFR[1] = gpio->AFR[1] | (afr << 4*(pin-8));
		}
	}

	void GPIO::WRITE(GPIO_TypeDef* gpio, uint8_t pin, bool pinState){
		gpio->BSRR |= (1 << (pin + (16* !pinState)));
	}

	bool GPIO::READ(GPIO_TypeDef* gpio, uint8_t pin){
		return gpio->IDR & (1<<pin);
	}

	void GPIO::INTERRUPT_INIT(GPIO_TypeDef* gpio, uint8_t pin){
		if(gpio == GPIOA){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b0000 << ((pin % 4) * 4));
		}else if(gpio == GPIOB){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b0001 << ((pin % 4) * 4));
		}else if(gpio == GPIOC){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b0010 << ((pin % 4) * 4));
		}else if(gpio == GPIOD){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b0011 << ((pin % 4) * 4));
		}else if(gpio == GPIOE){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b0100 << ((pin % 4) * 4));
		}else if(gpio == GPIOF){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b0101 << ((pin % 4) * 4));
		}else if(gpio == GPIOG){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b0110 << ((pin % 4) * 4));
		}else if(gpio == GPIOH){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b0111 << ((pin % 4) * 4));
		}else if(gpio == GPIOI){
			SYSCFG->EXTICR[pin/4] = SYSCFG->EXTICR[pin/4] | (0b1000 << ((pin % 4) * 4));
		}
	}
