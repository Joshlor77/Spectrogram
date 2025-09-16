################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Inc/binary_semaphore.cpp \
../Inc/st7789v3.cpp 

OBJS += \
./Inc/binary_semaphore.o \
./Inc/st7789v3.o 

CPP_DEPS += \
./Inc/binary_semaphore.d \
./Inc/st7789v3.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/%.o Inc/%.su Inc/%.cyclo: ../Inc/%.cpp Inc/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DSTM32H755ZITx -DSTM32 -DSTM32H7DUAL -DSTM32H7 -DNUCLEO_H755ZI_Q -c -I../Inc -I"F:/CodeSpaces/STM32Space/Spectrogram/CM7/Inc/Peripherals" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Inc

clean-Inc:
	-$(RM) ./Inc/binary_semaphore.cyclo ./Inc/binary_semaphore.d ./Inc/binary_semaphore.o ./Inc/binary_semaphore.su ./Inc/st7789v3.cyclo ./Inc/st7789v3.d ./Inc/st7789v3.o ./Inc/st7789v3.su

.PHONY: clean-Inc

