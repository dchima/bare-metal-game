################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HPS_Timers/HPS_Timers.c 

OBJS += \
./HPS_Timers/HPS_Timers.o 

C_DEPS += \
./HPS_Timers/HPS_Timers.d 


# Each subdirectory must supply rules for building sources it contributes
HPS_Timers/%.o: ../HPS_Timers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


