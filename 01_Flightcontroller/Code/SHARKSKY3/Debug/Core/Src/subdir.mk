################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/GPIO.cpp \
../Core/Src/ICM20689.cpp \
../Core/Src/InterruptHandlers.cpp \
../Core/Src/PID.cpp \
../Core/Src/RF24.cpp \
../Core/Src/SPI.cpp \
../Core/Src/Sensors.cpp \
../Core/Src/TIM.cpp \
../Core/Src/UART.cpp \
../Core/Src/fast_math.cpp \
../Core/Src/main.cpp 

C_SRCS += \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c 

C_DEPS += \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d 

OBJS += \
./Core/Src/GPIO.o \
./Core/Src/ICM20689.o \
./Core/Src/InterruptHandlers.o \
./Core/Src/PID.o \
./Core/Src/RF24.o \
./Core/Src/SPI.o \
./Core/Src/Sensors.o \
./Core/Src/TIM.o \
./Core/Src/UART.o \
./Core/Src/fast_math.o \
./Core/Src/main.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o 

CPP_DEPS += \
./Core/Src/GPIO.d \
./Core/Src/ICM20689.d \
./Core/Src/InterruptHandlers.d \
./Core/Src/PID.d \
./Core/Src/RF24.d \
./Core/Src/SPI.d \
./Core/Src/Sensors.d \
./Core/Src/TIM.d \
./Core/Src/UART.d \
./Core/Src/fast_math.d \
./Core/Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F722xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F722xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/GPIO.cyclo ./Core/Src/GPIO.d ./Core/Src/GPIO.o ./Core/Src/GPIO.su ./Core/Src/ICM20689.cyclo ./Core/Src/ICM20689.d ./Core/Src/ICM20689.o ./Core/Src/ICM20689.su ./Core/Src/InterruptHandlers.cyclo ./Core/Src/InterruptHandlers.d ./Core/Src/InterruptHandlers.o ./Core/Src/InterruptHandlers.su ./Core/Src/PID.cyclo ./Core/Src/PID.d ./Core/Src/PID.o ./Core/Src/PID.su ./Core/Src/RF24.cyclo ./Core/Src/RF24.d ./Core/Src/RF24.o ./Core/Src/RF24.su ./Core/Src/SPI.cyclo ./Core/Src/SPI.d ./Core/Src/SPI.o ./Core/Src/SPI.su ./Core/Src/Sensors.cyclo ./Core/Src/Sensors.d ./Core/Src/Sensors.o ./Core/Src/Sensors.su ./Core/Src/TIM.cyclo ./Core/Src/TIM.d ./Core/Src/TIM.o ./Core/Src/TIM.su ./Core/Src/UART.cyclo ./Core/Src/UART.d ./Core/Src/UART.o ./Core/Src/UART.su ./Core/Src/fast_math.cyclo ./Core/Src/fast_math.d ./Core/Src/fast_math.o ./Core/Src/fast_math.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su

.PHONY: clean-Core-2f-Src

