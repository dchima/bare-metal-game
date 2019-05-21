################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DE1SoC_LT24/DE1SoC_LT24.c 

OBJS += \
./DE1SoC_LT24/DE1SoC_LT24.o 

C_DEPS += \
./DE1SoC_LT24/DE1SoC_LT24.d 


# Each subdirectory must supply rules for building sources it contributes
DE1SoC_LT24/%.o: ../DE1SoC_LT24/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


