################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HPS_IRQ/HPS_IRQ.c 

OBJS += \
./HPS_IRQ/HPS_IRQ.o 

C_DEPS += \
./HPS_IRQ/HPS_IRQ.d 


# Each subdirectory must supply rules for building sources it contributes
HPS_IRQ/%.o: ../HPS_IRQ/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


