################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HPS_usleep/HPS_usleep.c 

OBJS += \
./HPS_usleep/HPS_usleep.o 

C_DEPS += \
./HPS_usleep/HPS_usleep.d 


# Each subdirectory must supply rules for building sources it contributes
HPS_usleep/%.o: ../HPS_usleep/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


