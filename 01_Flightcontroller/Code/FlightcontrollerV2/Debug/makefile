################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
FlightcontrollerV2.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
FlightcontrollerV2.list \

OBJCOPY_BIN += \
FlightcontrollerV2.bin \


# All Target
all: FlightcontrollerV2.elf secondary-outputs

# Tool invocations
FlightcontrollerV2.elf: $(OBJS) $(USER_OBJS) D:\Pluscrafter\Documents\SHARKSKY\01_Flightcontroller\Code\FlightcontrollerV2\STM32F722RETX_FLASH.ld
	arm-none-eabi-gcc -o "FlightcontrollerV2.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"D:\Pluscrafter\Documents\SHARKSKY\01_Flightcontroller\Code\FlightcontrollerV2\STM32F722RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="FlightcontrollerV2.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

FlightcontrollerV2.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "FlightcontrollerV2.list"
	@echo 'Finished building: $@'
	@echo ' '

FlightcontrollerV2.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "FlightcontrollerV2.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
