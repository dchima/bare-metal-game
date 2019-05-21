################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AvengersGame/Sounds/AvengersLaunch1.c \
../AvengersGame/Sounds/AvengersLaunch2.c 

OBJS += \
./AvengersGame/Sounds/AvengersLaunch1.o \
./AvengersGame/Sounds/AvengersLaunch2.o 

C_DEPS += \
./AvengersGame/Sounds/AvengersLaunch1.d \
./AvengersGame/Sounds/AvengersLaunch2.d 


# Each subdirectory must supply rules for building sources it contributes
AvengersGame/Sounds/%.o: ../AvengersGame/Sounds/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


