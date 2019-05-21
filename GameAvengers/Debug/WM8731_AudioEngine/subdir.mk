################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../WM8731_AudioEngine/WM8731_AudioEngine.c 

OBJS += \
./WM8731_AudioEngine/WM8731_AudioEngine.o 

C_DEPS += \
./WM8731_AudioEngine/WM8731_AudioEngine.d 


# Each subdirectory must supply rules for building sources it contributes
WM8731_AudioEngine/%.o: ../WM8731_AudioEngine/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


