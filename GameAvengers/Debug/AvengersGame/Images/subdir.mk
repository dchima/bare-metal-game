################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AvengersGame/Images/A_Launch1.c \
../AvengersGame/Images/A_Launch2.c \
../AvengersGame/Images/A_Launch3.c \
../AvengersGame/Images/A_Launch4.c \
../AvengersGame/Images/A_Launch5.c \
../AvengersGame/Images/A_Launch6.c 

OBJS += \
./AvengersGame/Images/A_Launch1.o \
./AvengersGame/Images/A_Launch2.o \
./AvengersGame/Images/A_Launch3.o \
./AvengersGame/Images/A_Launch4.o \
./AvengersGame/Images/A_Launch5.o \
./AvengersGame/Images/A_Launch6.o 

C_DEPS += \
./AvengersGame/Images/A_Launch1.d \
./AvengersGame/Images/A_Launch2.d \
./AvengersGame/Images/A_Launch3.d \
./AvengersGame/Images/A_Launch4.d \
./AvengersGame/Images/A_Launch5.d \
./AvengersGame/Images/A_Launch6.d 


# Each subdirectory must supply rules for building sources it contributes
AvengersGame/Images/%.o: ../AvengersGame/Images/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


