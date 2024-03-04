/*
 * TIM.cpp
 *
 *  Created on: Feb 28, 2024
 *      Author: pluscrafter
 */
#include "TIM.hpp"

namespace TIMER{
	void TIM::Init(){
		if(tim == TIM1){
			RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_TIM1EN;
			//IRQn = TIM1_CC_IRQn;
		}else if(tim == TIM2){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM2EN;
			IRQn = TIM2_IRQn;
		}else if(tim == TIM3){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM3EN;
			IRQn = TIM3_IRQn;
		}else if(tim == TIM4){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM4EN;
			IRQn = TIM4_IRQn;
		}else if(tim == TIM5){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM5EN;
			IRQn = TIM5_IRQn;
		}else if(tim == TIM6){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM6EN;
			//IRQn = TIM6_IRQn;
		}else if(tim == TIM7){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM7EN;
			IRQn = TIM7_IRQn;
		}


		tim->PSC = tim_prescale;
		tim->ARR = tim_resolution;


		RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_SYSCFGEN;
		NVIC_EnableIRQ(IRQn);
	}
	void TIM::EnableUpdateInterrupt(){
		tim->DIER = tim->DIER | TIM_DIER_UIE;
	}

	void TIM::UpdateInterrupt_Handler(){
		tim->SR &= ~TIM_SR_UIF;
	}

	void TIM::EnableChannel(GPIO_TypeDef *tim_gpio, uint8_t tim_pin, uint8_t tim_afr, uint8_t tim_channel){
		GPIO::EN(tim_gpio);
		GPIO::MODE(tim_gpio, tim_pin, MODE_ALT);
		GPIO::SPEED(tim_gpio, tim_pin, SPEED_VHIGH);
		GPIO::AFR(tim_gpio, tim_pin, tim_afr);

		if(tim_channel == 1){
			gpio_CH1 = tim_gpio;
			gpio_CH1_pin = tim_pin;
		}else if(tim_channel == 2){
			gpio_CH2 = tim_gpio;
			gpio_CH2_pin = tim_pin;
		}else if(tim_channel == 3){
			gpio_CH3 = tim_gpio;
			gpio_CH3_pin = tim_pin;
		}else if(tim_channel == 4){
			gpio_CH4 = tim_gpio;
			gpio_CH4_pin = tim_pin;
		}


	}

	void TIM::EnablePWM(uint8_t tim_channel){
		if(tim_channel == 1){
			tim->CCMR1 = tim->CCMR1 & ~(TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1PE);
			tim->CCMR1 = tim->CCMR1 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
			tim->CCER = tim->CCER | TIM_CCER_CC1E;
		} else if(tim_channel == 2){
			tim->CCMR1 = tim->CCMR1 & ~(TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_3 | TIM_CCMR1_OC2PE);
			tim->CCMR1 = tim->CCMR1 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
			tim->CCER = tim->CCER | TIM_CCER_CC2E;
		}else if(tim_channel == 3){
			tim->CCMR2 = tim->CCMR2 & ~(TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_3 | TIM_CCMR2_OC3PE);
			tim->CCMR2 = tim->CCMR2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
			tim->CCER = tim->CCER | TIM_CCER_CC3E;
		}else if(tim_channel == 4){
			tim->CCMR2 = tim->CCMR2 & ~(TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4M_3 | TIM_CCMR2_OC4PE);
			tim->CCMR2 = tim->CCMR2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
			tim->CCER = tim->CCER | TIM_CCER_CC4E;

		}
		tim->EGR = tim->EGR | TIM_EGR_UG;
	}

	void TIM::TIM_Start(){
		tim->CR1 = tim->CR1 | TIM_CR1_CEN;
	}

	void TIM::Interrupt_Handler(){
		if((tim->SR & TIM_SR_CC1IF_Msk) == TIM_SR_CC1IF_Msk){
			if(0b1 & GPIO::READ(gpio_CH1, gpio_CH1_pin)){
				CH_1_START = tim->CCR1;
				tim->CCER = tim->CCER | TIM_CCER_CC1P;
			}else{
				CH_1_VAL = tim->CCR1 - CH_1_START;
				if(CH_1_VAL < 0) CH_1_VAL += 4096;
				tim->CCER = tim->CCER & ~TIM_CCER_CC1P;
			}
		}

		if((tim->SR & TIM_SR_CC2IF_Msk) == TIM_SR_CC2IF_Msk){
			if(0b1 & GPIO::READ(gpio_CH2, gpio_CH2_pin)){
				CH_2_START = tim->CCR2;
				tim->CCER = tim->CCER | TIM_CCER_CC2P;
			}else{
				CH_2_VAL = tim->CCR2 - CH_2_START;
				if(CH_2_VAL < 0) CH_2_VAL += 4096;
				tim->CCER = tim->CCER & ~TIM_CCER_CC2P;
			}
		}

		if((tim->SR & TIM_SR_CC3IF_Msk) == TIM_SR_CC3IF_Msk){
			if(0b1 & GPIO::READ(gpio_CH3, gpio_CH3_pin)){
				CH_3_START = tim->CCR3;
				tim->CCER = tim->CCER | TIM_CCER_CC3P;
			}else{
				CH_3_VAL = tim->CCR3 - CH_3_START;
				if(CH_3_VAL < 0) CH_3_VAL += 4096;
				tim->CCER = tim->CCER & ~TIM_CCER_CC3P;
			}
		}

		if((tim->SR & TIM_SR_CC4IF_Msk) == TIM_SR_CC4IF_Msk){
			if(0b1 & GPIO::READ(gpio_CH4, gpio_CH4_pin)){
				CH_4_START = tim->CCR4;
				tim->CCER = tim->CCER | TIM_CCER_CC4P;
			}else{
				CH_4_VAL = tim->CCR4 - CH_4_START;
				if(CH_4_VAL < 0) CH_4_VAL += 4096;
				tim->CCER = tim->CCER & ~TIM_CCER_CC4P;
			}
		}
	}

	void TIM::PWM_setDutyCycle(uint8_t tim_channel ,uint16_t dutyCycle){
		if(tim_channel == 1){
			tim->CCR1 = dutyCycle;
		}else if(tim_channel == 2){
			tim->CCR2 = dutyCycle;
		}else if(tim_channel == 3){
			tim->CCR3 = dutyCycle;
		}else if(tim_channel == 4){
			tim->CCR4 = dutyCycle;
		}
	}

	void TIM::EnableInputCapture(uint8_t tim_channel){
		//static_assert((tim == TIM2) | (tim == TIM3) | (tim == TIM4) | (tim == TIM5) );

		if(tim_channel == 1){
			tim->CCMR1 =  tim->CCMR1 | TIM_CCMR1_CC1S_0;
			tim->CCMR1 =  tim->CCMR1 & ~TIM_CCMR1_CC1S_1;

			tim->CCER = tim->CCER | TIM_CCER_CC1P;
			tim->CCER = tim->CCER & ~TIM_CCER_CC1NP;

			tim->DIER = tim->DIER | TIM_DIER_CC1IE;

			tim->CCER = tim->CCER | TIM_CCER_CC1E;

		} else if(tim_channel == 2){
			tim->CCMR1 =  tim->CCMR1 | TIM_CCMR1_CC2S_0;
			tim->CCMR1 =  tim->CCMR1 & ~TIM_CCMR1_CC2S_1;

			tim->CCER = tim->CCER | TIM_CCER_CC2P;
			tim->CCER = tim->CCER & ~TIM_CCER_CC2NP;

			tim->DIER = tim->DIER | TIM_DIER_CC2IE;

			tim->CCER = tim->CCER | TIM_CCER_CC2E;

		} else if(tim_channel == 3){
			tim->CCMR2 =  tim->CCMR2 | TIM_CCMR2_CC3S_0;
			tim->CCMR2 =  tim->CCMR2 & ~TIM_CCMR2_CC3S_1;

			tim->CCER = tim->CCER | TIM_CCER_CC3P;
			tim->CCER = tim->CCER & ~TIM_CCER_CC3NP;

			tim->DIER = tim->DIER | TIM_DIER_CC3IE;

			tim->CCER = tim->CCER | TIM_CCER_CC3E;
		} else if(tim_channel == 4){
			tim->CCMR2 =  tim->CCMR2 | TIM_CCMR2_CC4S_0;
			tim->CCMR2 =  tim->CCMR2 & ~TIM_CCMR2_CC4S_1;

			tim->CCER = tim->CCER | TIM_CCER_CC4P;
			tim->CCER = tim->CCER & ~TIM_CCER_CC4NP;

			tim->DIER = tim->DIER | TIM_DIER_CC4IE;

			tim->CCER = tim->CCER | TIM_CCER_CC4E;
		}
	}

	uint32_t TIM::getInputCycle(uint8_t tim_channel){
		if(tim_channel == 1){
			return tim->CCR1;
		} else if(tim_channel == 2){
			return tim->CCR2;
		} else if(tim_channel == 3){
			return tim->CCR3;
		} else if(tim_channel == 4){
			return tim->CCR4;
		}
	}

	void TIM::EnablePWMInput(uint8_t tim_channel){
		if(tim_channel == 1){
			tim->CCMR1 =  tim->CCMR1 | TIM_CCMR1_CC1S_0;
			tim->CCMR1 =  tim->CCMR1 & ~TIM_CCMR1_CC1S_1;

			tim->CCER = tim->CCER & ~(TIM_CCER_CC1P|TIM_CCER_CC1NP);

			tim->CCMR1 = tim->CCMR1 | TIM_CCMR1_CC2S_1;
			tim->CCMR1 = tim->CCMR1 & ~TIM_CCMR1_CC2S_0;

			tim->CCER = tim->CCER | TIM_CCER_CC2P;
			tim->CCER = tim->CCER & ~TIM_CCER_CC2NP;

			tim->SMCR = tim->SMCR | TIM_SMCR_TS_2 | TIM_SMCR_TS_0;
			tim->SMCR = tim->SMCR & ~TIM_SMCR_TS_1;

			tim->SMCR = tim->SMCR | TIM_SMCR_SMS_2;
			tim->SMCR = tim->SMCR & ~(TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1);

			tim->CCER = tim->CCER | TIM_CCER_CC1E | TIM_CCER_CC2E;

		}
	}




}


