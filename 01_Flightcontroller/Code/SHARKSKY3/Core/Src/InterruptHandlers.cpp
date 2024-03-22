
#include "Instances.hpp"
#include "Sensors.hpp"
#include "PID.h"


COM::SPI spi3(SPI3, GPIOB, 2, 7 ,GPIOC, 10, 6, GPIOC, 11, 6, GPIOA, 4);

COM::SPI spi2(SPI2, GPIOC, 1, 5 ,GPIOC, 2, 5, GPIOB, 13, 5, GPIOB, 12);

COM::UART uart1(USART1, GPIOA, 9, 7, GPIOA, 10, 7);

TIMER::TIM tim2_motor(TIM2, 12, 4096);
TIMER::TIM tim3_controller(TIM3,71, 4096);
TIMER::TIM tim4_controller(TIM4,71, 4096);
TIMER::TIM tim7_trig(TIM7,0, 13499);
TIMER::TIM tim5_trig(TIM5,1, 53999);

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

extern "C" void TIM5_IRQHandler() noexcept{
	tim5_trig.UpdateInterrupt_Handler();
	if(GPIO::READ(GPIOB,1)){
		GPIO::WRITE(GPIOB, 1, LOW);
	}else{
		GPIO::WRITE(GPIOB, 1, HIGH);
	}
	recv.getVal();

	mot.Y_CORR = YAW_PID.calc(recv.yaw, imu.IMU_f_ypr[2]);
	mot.P_CORR = PITCH_PID.calc(recv.pitch, imu.IMU_f_ypr[1]);
	mot.R_CORR = ROLL_PID.calc(recv.roll, imu.IMU_f_ypr[0]);
	mot.THROTTLE = recv.throttle;

	mot.setMotor();
}

extern "C" void TIM7_IRQHandler() noexcept{
	tim7_trig.UpdateInterrupt_Handler();
	if(GPIO::READ(GPIOC,4)){
		GPIO::WRITE(GPIOC, 4, LOW);
	}else{
		GPIO::WRITE(GPIOC, 4, HIGH);
	}

	imu.IMU_calcAngle();
}

extern "C" void EXTI9_5_IRQHandler(){

	//imu.IMU_readAngle();
	//EXTI->PR = EXTI->PR | (1 << 9);
}
