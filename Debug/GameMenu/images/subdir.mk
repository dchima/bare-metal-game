################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GameMenu/images/menu_lAvengers.c \
../GameMenu/images/menu_lSnake.c \
../GameMenu/images/menu_tAvengers.c \
../GameMenu/images/menu_tSnake.c \
../GameMenu/images/menu_title.c 

OBJS += \
./GameMenu/images/menu_lAvengers.o \
./GameMenu/images/menu_lSnake.o \
./GameMenu/images/menu_tAvengers.o \
./GameMenu/images/menu_tSnake.o \
./GameMenu/images/menu_title.o 

C_DEPS += \
./GameMenu/images/menu_lAvengers.d \
./GameMenu/images/menu_lSnake.d \
./GameMenu/images/menu_tAvengers.d \
./GameMenu/images/menu_tSnake.d \
./GameMenu/images/menu_title.d 


# Each subdirectory must supply rules for building sources it contributes
GameMenu/images/%.o: ../GameMenu/images/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


