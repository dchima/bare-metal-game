################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HPS_I2C/HPS_I2C.c 

OBJS += \
./HPS_I2C/HPS_I2C.o 

C_DEPS += \
./HPS_I2C/HPS_I2C.d 


# Each subdirectory must supply rules for building sources it contributes
HPS_I2C/%.o: ../HPS_I2C/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


