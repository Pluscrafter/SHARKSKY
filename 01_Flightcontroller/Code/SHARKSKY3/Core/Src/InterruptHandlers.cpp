
#include "Instances.hpp"
#include "Sensors.hpp"


COM::SPI spi3(SPI3, GPIOB, 2, 7 ,GPIOC, 10, 6, GPIOC, 11, 6, GPIOA, 4);

COM::SPI spi2(SPI2, GPIOC, 1, 5 ,GPIOC, 2, 5, GPIOB, 13, 5, GPIOB, 12);

COM::UART uart1(USART1, GPIOA, 9, 7, GPIOA, 10, 7);

TIMER::TIM tim2_motor(TIM2, 12, 4096);
TIMER::TIM tim3_controller(TIM3,71, 4096);
TIMER::TIM tim4_controller(TIM4,71, 4096);
TIMER::TIM tim5_trig(TIM5,1, 26999);
TIMER::TIM tim7_trig(TIM7,1, 53999);

extern "C" void SPI3_IRQHandler(){
	spi3.Interrupt_Handler();
}

extern "C" void SPI2_IRQHandler(){
	spi2.Interrupt_Handler();
}

extern "C" void USART1_IRQHandler(){
	uart1.Interrupt_Handler();
}

extern "C" void TIM3_IRQHandler(){
	tim3_controller.Interrupt_Handler();
}

extern "C" void TIM4_IRQHandler(){
	tim4_controller.Interrupt_Handler();
}

extern "C" void TIM5_IRQHandler(){
	if(GPIO::READ(GPIOC, 4)){
		GPIO::WRITE(GPIOC, 4, LOW);
	}else{
		GPIO::WRITE(GPIOC, 4, HIGH);
	}
	imu.IMU_calcAngle();
	tim5_trig.UpdateInterrupt_Handler();
}


extern "C" void TIM7_IRQHandler(){
	tim7_trig.UpdateInterrupt_Handler();
}

extern "C" void EXTI9_5_IRQHandler(){

	//imu.IMU_readAngle();
	EXTI->PR = EXTI->PR | (1 << 9);
}
