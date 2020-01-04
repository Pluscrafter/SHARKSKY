################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/bsp_driver_sd.c \
../Src/dac.c \
../Src/dma.c \
../Src/fatfs.c \
../Src/fatfs_platform.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/sd_diskio.c \
../Src/sdmmc.c \
../Src/spi.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_it.c \
../Src/sys.c \
../Src/system_stm32f7xx.c \
../Src/tim.c \
../Src/usart.c 

CPP_SRCS += \
../Src/Flightcontroller.cpp \
../Src/ICM20689.cpp \
../Src/RF24.cpp \
../Src/main.cpp 

OBJS += \
./Src/Flightcontroller.o \
./Src/ICM20689.o \
./Src/RF24.o \
./Src/adc.o \
./Src/bsp_driver_sd.o \
./Src/dac.o \
./Src/dma.o \
./Src/fatfs.o \
./Src/fatfs_platform.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/sd_diskio.o \
./Src/sdmmc.o \
./Src/spi.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_it.o \
./Src/sys.o \
./Src/system_stm32f7xx.o \
./Src/tim.o \
./Src/usart.o 

C_DEPS += \
./Src/adc.d \
./Src/bsp_driver_sd.d \
./Src/dac.d \
./Src/dma.d \
./Src/fatfs.d \
./Src/fatfs_platform.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/sd_diskio.d \
./Src/sdmmc.d \
./Src/spi.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_it.d \
./Src/sys.d \
./Src/system_stm32f7xx.d \
./Src/tim.d \
./Src/usart.d 

CPP_DEPS += \
./Src/Flightcontroller.d \
./Src/ICM20689.d \
./Src/RF24.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Flightcontroller.o: ../Src/Flightcontroller.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/Flightcontroller.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ICM20689.o: ../Src/ICM20689.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/ICM20689.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/RF24.o: ../Src/RF24.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/RF24.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/adc.o: ../Src/adc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/adc.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/bsp_driver_sd.o: ../Src/bsp_driver_sd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/bsp_driver_sd.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/dac.o: ../Src/dac.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/dac.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/dma.o: ../Src/dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/dma.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/fatfs.o: ../Src/fatfs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/fatfs.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/fatfs_platform.o: ../Src/fatfs_platform.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/fatfs_platform.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/gpio.o: ../Src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/gpio.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/i2c.o: ../Src/i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/i2c.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sd_diskio.o: ../Src/sd_diskio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/sd_diskio.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sdmmc.o: ../Src/sdmmc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/sdmmc.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/spi.o: ../Src/spi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/spi.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f7xx_hal_msp.o: ../Src/stm32f7xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/stm32f7xx_hal_msp.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f7xx_it.o: ../Src/stm32f7xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/stm32f7xx_it.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sys.o: ../Src/sys.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/sys.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f7xx.o: ../Src/system_stm32f7xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f7xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim.o: ../Src/tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/tim.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usart.o: ../Src/usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F722xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -Ofast -ffunction-sections -fdata-sections -mslow-flash-data -Wall -fstack-usage -MMD -MP -MF"Src/usart.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

