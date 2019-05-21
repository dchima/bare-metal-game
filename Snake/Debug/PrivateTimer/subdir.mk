################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PrivateTimer/PrivateTimer.c 

OBJS += \
./PrivateTimer/PrivateTimer.o 

C_DEPS += \
./PrivateTimer/PrivateTimer.d 


# Each subdirectory must supply rules for building sources it contributes
PrivateTimer/%.o: ../PrivateTimer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


