/*
 * Instances.hpp
 *
 *  Created on: May 13, 2022
 *      Author: charpoank
 */

#ifndef INC_INSTANCES_HPP_
#define INC_INSTANCES_HPP_


#include "SPI.hpp"
#include "UART.hpp"
#include "TIM.hpp"

extern COM::SPI spi3;
extern COM::SPI spi2;
extern COM::UART uart1;
extern TIMER::TIM tim2_motor;
extern TIMER::TIM tim3_controller;
extern TIMER::TIM tim4_controller;
extern TIMER::TIM tim5_trig;
extern TIMER::TIM tim7_trig;




#endif /* INC_INSTANCES_HPP_ */
