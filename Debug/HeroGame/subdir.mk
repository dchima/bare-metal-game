################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HeroGame/Audio.c \
../HeroGame/Hero.c \
../HeroGame/Hero_Enviro.c 

OBJS += \
./HeroGame/Audio.o \
./HeroGame/Hero.o \
./HeroGame/Hero_Enviro.o 

C_DEPS += \
./HeroGame/Audio.d \
./HeroGame/Hero.d \
./HeroGame/Hero_Enviro.d 


# Each subdirectory must supply rules for building sources it contributes
HeroGame/%.o: ../HeroGame/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


