################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/UART2.cpp \
../src/fonctionsLPC.cpp \
../src/serialinterface.cpp 

OBJS += \
./src/UART2.o \
./src/fonctionsLPC.o \
./src/serialinterface.o 

CPP_DEPS += \
./src/UART2.d \
./src/fonctionsLPC.d \
./src/serialinterface.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=gnu++11 -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__LPC17XX__ -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\UART_2\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_chip_175x_6x\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


