/*
 * TIM.hpp
 *
 *  Created on: Feb 28, 2024
 *      Author: pluscrafter
 */

#ifndef INC_TIM_HPP_
#define INC_TIM_HPP_

#include <cstddef>
#include "stm32f722xx.h"
#include "stm32f7xx_it.h"
#include "core_cm7.h"
#include <cassert>
#include "GPIO.hpp"

namespace TIMER {
	class TIM{
	public:
		constexpr TIM(TIM_TypeDef* tim, uint32_t tim_prescale, uint16_t tim_resolution): tim(tim), tim_prescale(tim_prescale),
		tim_resolution(tim_resolution), CH_1_START(0), CH_2_START(0), CH_3_START(0), CH_4_START(0),
		CH_1_VAL(0), CH_2_VAL(0), CH_3_VAL(0), CH_4_VAL(0),
		gpio_CH1_pin(0), gpio_CH2_pin(0), gpio_CH3_pin(0), gpio_CH4_pin(0),
		gpio_CH1(GPIOA), gpio_CH2(GPIOA), gpio_CH3(GPIOA), gpio_CH4(GPIOA), IRQn(TIM3_IRQn){};

		void Init();
		void EnableChannel(GPIO_TypeDef *tim_gpio, uint8_t tim_pin, uint8_t tim_afr, uint8_t channel);
		void Interrupt_Handler() noexcept;
		void TIM_Start();

		void EnableUpdateInterrupt();
		void UpdateInterrupt_Handler();

		void EnablePWM(uint8_t tim_channel);
		void PWM_setDutyCycle(uint8_t tim_channel ,uint16_t dutyCycle);

		void EnablePWMInput(uint8_t tim_channel);
		uint32_t getInputCycle(uint8_t tim_channel);

		void EnableInputCapture(uint8_t tim_channel);


		int32_t CH_1_VAL ,CH_2_VAL ,CH_3_VAL ,CH_4_VAL;

	private:
		TIM_TypeDef* const tim;
		IRQn_Type IRQn;

		GPIO_TypeDef *gpio_CH1, *gpio_CH2, *gpio_CH3, *gpio_CH4;

		uint8_t gpio_CH1_pin, gpio_CH2_pin, gpio_CH3_pin, gpio_CH4_pin;

		uint32_t tim_prescale;
		int32_t CH_1_START, CH_2_START, CH_3_START, CH_4_START;


		const uint16_t tim_resolution;
	};


}



#endif /* INC_TIM_HPP_ */
