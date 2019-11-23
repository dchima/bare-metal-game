################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HeroGame/Sounds/avengers_Audio1.c \
../HeroGame/Sounds/avengers_Audio2.c \
../HeroGame/Sounds/avengers_Audio3.c 

OBJS += \
./HeroGame/Sounds/avengers_Audio1.o \
./HeroGame/Sounds/avengers_Audio2.o \
./HeroGame/Sounds/avengers_Audio3.o 

C_DEPS += \
./HeroGame/Sounds/avengers_Audio1.d \
./HeroGame/Sounds/avengers_Audio2.d \
./HeroGame/Sounds/avengers_Audio3.d 


# Each subdirectory must supply rules for building sources it contributes
HeroGame/Sounds/%.o: ../HeroGame/Sounds/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


