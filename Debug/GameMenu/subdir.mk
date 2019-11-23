################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GameMenu/GameMenu.c 

OBJS += \
./GameMenu/GameMenu.o 

C_DEPS += \
./GameMenu/GameMenu.d 


# Each subdirectory must supply rules for building sources it contributes
GameMenu/%.o: ../GameMenu/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


