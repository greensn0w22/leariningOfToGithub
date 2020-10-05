################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TestZone.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc175x_6x.cpp 

C_SRCS += \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/TestZone.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/sysinit.o 

CPP_DEPS += \
./src/TestZone.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc175x_6x.d 

C_DEPS += \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC17XX__ -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\TestZone\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_chip_175x_6x\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\UART_2\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\UART_2\src" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC17XX__ -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\TestZone\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_chip_175x_6x\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\UART_2\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\UART_2\src" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


