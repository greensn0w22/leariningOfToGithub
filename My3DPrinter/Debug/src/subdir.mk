################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/My3DPrinter.cpp \
../src/PID.cpp \
../src/UART2.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc175x_6x.cpp \
../src/fonctionsLPC.cpp \
../src/heat.cpp \
../src/interrupt.cpp \
../src/motorclass.cpp \
../src/printer3dclass.cpp \
../src/serialinterface.cpp 

C_SRCS += \
../src/crp.c \
../src/error.c \
../src/ff.c \
../src/ffsystem.c \
../src/ffunicode.c \
../src/mmc.c \
../src/spi.c \
../src/sysinit.c 

OBJS += \
./src/My3DPrinter.o \
./src/PID.o \
./src/UART2.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/error.o \
./src/ff.o \
./src/ffsystem.o \
./src/ffunicode.o \
./src/fonctionsLPC.o \
./src/heat.o \
./src/interrupt.o \
./src/mmc.o \
./src/motorclass.o \
./src/printer3dclass.o \
./src/serialinterface.o \
./src/spi.o \
./src/sysinit.o 

CPP_DEPS += \
./src/My3DPrinter.d \
./src/PID.d \
./src/UART2.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc175x_6x.d \
./src/fonctionsLPC.d \
./src/heat.d \
./src/interrupt.d \
./src/motorclass.d \
./src/printer3dclass.d \
./src/serialinterface.d 

C_DEPS += \
./src/crp.d \
./src/error.d \
./src/ff.d \
./src/ffsystem.d \
./src/ffunicode.d \
./src/mmc.d \
./src/spi.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC17XX__ -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\My3DPrinter\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_chip_175x_6x\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC17XX__ -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\My3DPrinter\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Julien\Dropbox\Cegep3Annee\3DPrinterProject\workPlaceCode\lpc_chip_175x_6x\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


