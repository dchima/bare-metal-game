################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Snake_Engine/Snake.c \
../Snake_Engine/Snake_Enviro.c 

OBJS += \
./Snake_Engine/Snake.o \
./Snake_Engine/Snake_Enviro.o 

C_DEPS += \
./Snake_Engine/Snake.d \
./Snake_Engine/Snake_Enviro.d 


# Each subdirectory must supply rules for building sources it contributes
Snake_Engine/%.o: ../Snake_Engine/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


