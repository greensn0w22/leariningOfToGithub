################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/LPCCommand.c \
../src/LPCfunctions.c \
../src/UART_C.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/interrupt.c \
../src/sysinit.c 

OBJS += \
./src/LPCCommand.o \
./src/LPCfunctions.o \
./src/UART_C.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/interrupt.o \
./src/sysinit.o 

C_DEPS += \
./src/LPCCommand.d \
./src/LPCfunctions.d \
./src/UART_C.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/interrupt.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__LPC17XX__ -D__REDLIB__ -I"D:\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\UART_C2\inc" -I"D:\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_board_nxp_lpcxpresso_1769\inc" -I"D:\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_chip_175x_6x\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


