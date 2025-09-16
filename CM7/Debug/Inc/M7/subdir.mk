################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Inc/M7/mpu.cpp 

OBJS += \
./Inc/M7/mpu.o 

CPP_DEPS += \
./Inc/M7/mpu.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/M7/%.o Inc/M7/%.su Inc/M7/%.cyclo: ../Inc/M7/%.cpp Inc/M7/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DSTM32H755ZITx -DSTM32 -DSTM32H7DUAL -DSTM32H7 -DNUCLEO_H755ZI_Q -c -I../Inc -I"F:/CodeSpaces/STM32Space/Spectrogram/CM7/Inc/Peripherals" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Inc-2f-M7

clean-Inc-2f-M7:
	-$(RM) ./Inc/M7/mpu.cyclo ./Inc/M7/mpu.d ./Inc/M7/mpu.o ./Inc/M7/mpu.su

.PHONY: clean-Inc-2f-M7

