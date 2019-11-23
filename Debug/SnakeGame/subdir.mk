################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SnakeGame/Audio.c \
../SnakeGame/Snake.c \
../SnakeGame/Snake_Enviro.c \
../SnakeGame/Snake_Graphics.c 

OBJS += \
./SnakeGame/Audio.o \
./SnakeGame/Snake.o \
./SnakeGame/Snake_Enviro.o \
./SnakeGame/Snake_Graphics.o 

C_DEPS += \
./SnakeGame/Audio.d \
./SnakeGame/Snake.d \
./SnakeGame/Snake_Enviro.d \
./SnakeGame/Snake_Graphics.d 


# Each subdirectory must supply rules for building sources it contributes
SnakeGame/%.o: ../SnakeGame/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


