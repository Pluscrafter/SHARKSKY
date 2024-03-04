
#include "Instances.hpp"



COM::SPI spi3(SPI3, GPIOB, 2, 7 ,GPIOC, 10, 6, GPIOC, 11, 6, GPIOA, 4);
COM::SPI spi2(SPI2, GPIOC, 1, 5 ,GPIOC, 10, 5, GPIOB, 13, 5, GPIOB, 12);
COM::UART uart1(USART1,GPIOA, 9, 7, GPIOA, 10,7);

extern "C" void SPI3_IRQHandler(){
	spi3.Interrupt_Handler();
}

extern "C" void SPI2_IRQHandler(){
	spi2.Interrupt_Handler();
}

extern "C" void USART1_IRQHandler(){
	uart1.Interrupt_Handler();
}
