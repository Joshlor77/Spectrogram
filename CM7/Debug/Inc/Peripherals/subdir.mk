################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Inc/Peripherals/adc.cpp \
../Inc/Peripherals/dma.cpp \
../Inc/Peripherals/dmamux.cpp \
../Inc/Peripherals/gpio.cpp \
../Inc/Peripherals/hsem.cpp \
../Inc/Peripherals/rcc.cpp \
../Inc/Peripherals/spi.cpp \
../Inc/Peripherals/tim2345.cpp 

OBJS += \
./Inc/Peripherals/adc.o \
./Inc/Peripherals/dma.o \
./Inc/Peripherals/dmamux.o \
./Inc/Peripherals/gpio.o \
./Inc/Peripherals/hsem.o \
./Inc/Peripherals/rcc.o \
./Inc/Peripherals/spi.o \
./Inc/Peripherals/tim2345.o 

CPP_DEPS += \
./Inc/Peripherals/adc.d \
./Inc/Peripherals/dma.d \
./Inc/Peripherals/dmamux.d \
./Inc/Peripherals/gpio.d \
./Inc/Peripherals/hsem.d \
./Inc/Peripherals/rcc.d \
./Inc/Peripherals/spi.d \
./Inc/Peripherals/tim2345.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/Peripherals/%.o Inc/Peripherals/%.su Inc/Peripherals/%.cyclo: ../Inc/Peripherals/%.cpp Inc/Peripherals/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DSTM32H755ZITx -DSTM32 -DSTM32H7DUAL -DSTM32H7 -DNUCLEO_H755ZI_Q -c -I../Inc -I"F:/CodeSpaces/STM32Space/Spectrogram/CM7/Inc/Peripherals" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Inc-2f-Peripherals

clean-Inc-2f-Peripherals:
	-$(RM) ./Inc/Peripherals/adc.cyclo ./Inc/Peripherals/adc.d ./Inc/Peripherals/adc.o ./Inc/Peripherals/adc.su ./Inc/Peripherals/dma.cyclo ./Inc/Peripherals/dma.d ./Inc/Peripherals/dma.o ./Inc/Peripherals/dma.su ./Inc/Peripherals/dmamux.cyclo ./Inc/Peripherals/dmamux.d ./Inc/Peripherals/dmamux.o ./Inc/Peripherals/dmamux.su ./Inc/Peripherals/gpio.cyclo ./Inc/Peripherals/gpio.d ./Inc/Peripherals/gpio.o ./Inc/Peripherals/gpio.su ./Inc/Peripherals/hsem.cyclo ./Inc/Peripherals/hsem.d ./Inc/Peripherals/hsem.o ./Inc/Peripherals/hsem.su ./Inc/Peripherals/rcc.cyclo ./Inc/Peripherals/rcc.d ./Inc/Peripherals/rcc.o ./Inc/Peripherals/rcc.su ./Inc/Peripherals/spi.cyclo ./Inc/Peripherals/spi.d ./Inc/Peripherals/spi.o ./Inc/Peripherals/spi.su ./Inc/Peripherals/tim2345.cyclo ./Inc/Peripherals/tim2345.d ./Inc/Peripherals/tim2345.o ./Inc/Peripherals/tim2345.su

.PHONY: clean-Inc-2f-Peripherals

