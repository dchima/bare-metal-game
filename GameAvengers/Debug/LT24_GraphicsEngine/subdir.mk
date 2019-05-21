################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LT24_GraphicsEngine/LT24_GraphicsEngine.c 

OBJS += \
./LT24_GraphicsEngine/LT24_GraphicsEngine.o 

C_DEPS += \
./LT24_GraphicsEngine/LT24_GraphicsEngine.d 


# Each subdirectory must supply rules for building sources it contributes
LT24_GraphicsEngine/%.o: ../LT24_GraphicsEngine/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


