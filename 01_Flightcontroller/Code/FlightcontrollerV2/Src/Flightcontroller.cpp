/*
 * Flightcontroller.cpp
 *
 *  Created on: Dec 22, 2019
 *      Author: Pluscrafter
 */

#include "Flightcontroller.h"

namespace FLIGHTCONTROLLER {

	volatile double Flightcontroller::looptime = 0;

	Flightcontroller::Flightcontroller() {
		// TODO Auto-generated constructor stub

	}

	Flightcontroller::~Flightcontroller() {
		// TODO Auto-generated destructor stub
	}

	void Flightcontroller::Init(){

		InitCounter();

		InitMotors();
		InitGimbal();
		InitDAC();

		InitUSBOSD();

		HAL_GPIO_WritePin(LLVOE_GPIO_Port, LLVOE_Pin, GPIO_PIN_SET);

		InitRadio();

		if(!imu.initICM()){
			for(;;){
				HAL_GPIO_TogglePin(INIT_OK_GPIO_Port, INIT_OK_Pin);
				HAL_Delay(100);
			}
		}

		//HAL_TIM_Base_Start_IT(&htim7); //start baro trigger

		// initalisation is OK
		HAL_Delay(1000);
		HAL_GPIO_WritePin(INIT_OK_GPIO_Port, INIT_OK_Pin, GPIO_PIN_SET);

	}

	void Flightcontroller::Loop(){
		double start = DWT->CYCCNT; //read value from counter register
		elapsedTime += looptime;
		imu.lptime = looptime;
		imu.calculateICM();

		SetPIDGain();

		//set previous error from error
		for (uint8_t i = 0;  i < 3; i++){
		  previousError[i] = error[i];
		}

		CalculateError();

		LoopRadio();
		PID_AngleMotion();
		PID_TrueAngle();

		SetMotorSpeed();

		double stop = DWT->CYCCNT;
		looptime = (stop - start)/216000000.0;

	}

	void Flightcontroller::InitMotors(){
		// INIT MOTORS
		// Start PWM on timer 2
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	}

	void Flightcontroller::InitGimbal(){
		// INIT GIMBAL
		// Start PWM on timer 3
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	}

	void Flightcontroller::InitDAC(){
		//Start and Test DAC
		HAL_TIM_Base_Start(&htim6);														//start Timer 6 for dac tickspeed
		HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)sine, 256, DAC_ALIGN_8B_R);	//start DAC DMA read from sine array in circular mode
		HAL_Delay(1000);
		HAL_TIM_Base_Stop(&htim6);
	}

	void Flightcontroller::InitCounter(){
		//start DWT counter also when not in Debug mode to count clock ticks
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	//https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter
		DWT->LAR = 0xC5ACCE55; 							//https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter 12.10.19 02:03
		DWT->CYCCNT = 0;								//https://www.carminenoviello.com/2015/09/04/precisely-measure-microseconds-stm32/ 12.10.19 01:30
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}

	void Flightcontroller::InitRadio(){
		const uint64_t pipe = 0xE8E8F0F0E2;	//pipe address
		radio.begin();
		radio.setPayloadSize(32);
		radio.setChannel(125);
		radio.setDataRate(RF24_250KBPS);
		radio.setPALevel(RF24_PA_MAX);
		radio.setAutoAck(true);
		radio.enableDynamicPayloads();
		radio.enableAckPayload();
		radio.openReadingPipe(1, pipe);

		//radio.printDetails();

		radio.startListening();
	}

	void Flightcontroller::InitSDCard(){
		//https://www.youtube.com/watch?v=0NbBem8U80Y [11.10.19 14:22] // https://drive.google.com/file/d/1ZunUVcv1RYljzmQe1B3sUUbpJ6705hpM/view
		if(f_mount(&SDFatFS, SDPath, 1) == FR_OK){
			char mfil[] = "TESTEXT";
			if(f_open(&SDFile, mfil, FA_WRITE|FA_CREATE_ALWAYS) == FR_OK){
				char buf[] = "Hello World";
				f_write(&SDFile, buf, sizeof(buf), &reSD);
			}
			f_close(&SDFile);
		}

		//Init and create new LOG file
		char mfil[] = "LOG";
		if(f_open(&SDFile, mfil, FA_WRITE|FA_CREATE_ALWAYS) == FR_OK){

		}
		f_close(&SDFile);
	}

	void Flightcontroller::LoopRadio(){
		if(radio.available()){
			radio.read(&recvData,sizeof(RadioData));
			radio.writeAckPayload(1,&ackData,sizeof(AckData));
		}
	}

	void Flightcontroller::SetMotorSpeed(){
		// throttle value under 100 no movement of motors
		if (recvData.throttle < 100){
			for (int i = 0; i < 4 ;i++){
				motorSpeed[i] = 1024;
			}
		}
		else{
		//PID applied to the calculation motor speed
			motorSpeed[0] = (1024 + recvData.throttle) + pid_TA[1] + pid_TA[2] + pid_AM[0];
			motorSpeed[1] = (1024 + recvData.throttle) + pid_TA[1] - pid_TA[2] - pid_AM[0];
			motorSpeed[2] = (1024 + recvData.throttle) - pid_TA[1] - pid_TA[2] + pid_AM[0];
			motorSpeed[3] = (1024 + recvData.throttle) - pid_TA[1] + pid_TA[2] - pid_AM[0];

		}

		// maximum motor speed
		for (int i = 0; i < 4 ;i++){
			if (motorSpeed[i] > 2048){
				motorSpeed[i] = 2048;
			}
			else if(motorSpeed[i] < 1024){
				motorSpeed[i] = 1024;
			}
		}

		//write to timer
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,motorSpeed[0]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,motorSpeed[1]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,motorSpeed[2]);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,motorSpeed[3]);
	}

	void Flightcontroller::InitUSBOSD(){
		//Set UART to USB or OSD Mode
		if(HAL_GPIO_ReadPin(USBOSD_GPIO_Port, USBOSD_Pin) == GPIO_PIN_RESET){
			osdusb = USB_MODE;
			MotorCalibration();
			HAL_GPIO_WritePin(MOD0_LED_GPIO_Port, MOD0_LED_Pin, GPIO_PIN_SET);
		}else{
			osdusb = OSD_MODE;
			HAL_GPIO_WritePin(MOD1_LED_GPIO_Port, MOD1_LED_Pin, GPIO_PIN_SET);
		}
	}

	void Flightcontroller::CalculateError(){
		error[0] = imu.f_ypr[2] - recvData.yaw;
		error[1] = recvData.pitch - imu.t_ypr[1] ;
		error[2] = recvData.roll - imu.t_ypr[0];
	}

	void Flightcontroller::PID_TrueAngle(){
		for(uint8_t i = 0; i < 3; i++){
			//calc pid
			pid_TA_P[i] = error[i] * pidGain_TA_P[i];
			pid_TA_I[i] += error[i] * pidGain_TA_I[i] * looptime;
			pid_TA_D[i] = pidGain_TA_D[i] * ((error[i] - previousError[i]) * looptime);

			//sum of all corrections
			pid_TA[i] = pid_TA_P[i] + pid_TA_I[i] + pid_TA_D[i];

			//set max correction
			if(pid_TA[i] > MAX_CORRECTION){
				pid_TA[i] = MAX_CORRECTION;
			}else if(pid_TA[i] < -MAX_CORRECTION){
				pid_TA[i] = -MAX_CORRECTION;
			}
		}
	}

	void Flightcontroller::PID_AngleMotion(){
		for(uint8_t i = 0; i < 3; i++){
			//calc pid
			pid_AM_P[i] = error[i] * pidGain_AM_P[i];
			pid_AM_I[i] += error[i] * pidGain_AM_I[i] * looptime;
			pid_AM_D[i] = pidGain_AM_D[i] * ((error[i] - previousError[i]) * looptime);

			//sum of all corrections
			pid_AM[i] = pid_AM_P[i] + pid_AM_I[i] + pid_AM_D[i];

			//set max correction
			if(pid_AM[i] > MAX_CORRECTION){
				pid_AM[i] = MAX_CORRECTION;
			}else if(pid_AM[i] < -MAX_CORRECTION){
				pid_AM[i] = -MAX_CORRECTION;
			}
		}
	}

	void Flightcontroller::SetPIDGain(){
		if(recvData.throttle < 100){
			pidGain_AM_P[0] = recvData.y_P/100.0;
			pidGain_AM_I[0] = recvData.y_I/100.0;
			pidGain_AM_D[0] = recvData.y_D/100.0;

			pidGain_TA_P[1] = recvData.rp_P/100.0;
			pidGain_TA_I[1] = recvData.rp_I/100.0;
			pidGain_TA_D[1] = recvData.rp_D/100.0;

			pidGain_TA_P[2] = recvData.rp_P/100.0;
			pidGain_TA_I[2] = recvData.rp_I/100.0;
			pidGain_TA_D[2] = recvData.rp_D/100.0;

		}
	}

	void Flightcontroller::MotorCalibration(){
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,2048);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,2048);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,2048);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,2048);

		HAL_Delay(5000);

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1024);

		HAL_Delay(5000);

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,2048);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,2048);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,2048);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,2048);

		HAL_Delay(5000);

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1024);

		HAL_Delay(10000);

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1124);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1124);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1124);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1124);

		HAL_Delay(2000);

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1024);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1024);

	}

} /* namespace FLIGHTCONTROLLER */
