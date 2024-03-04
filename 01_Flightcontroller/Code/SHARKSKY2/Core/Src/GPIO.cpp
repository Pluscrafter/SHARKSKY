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
