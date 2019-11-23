################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../IO_Peripherals/IO_Peripherals.c 

OBJS += \
./IO_Peripherals/IO_Peripherals.o 

C_DEPS += \
./IO_Peripherals/IO_Peripherals.d 


# Each subdirectory must supply rules for building sources it contributes
IO_Peripherals/%.o: ../IO_Peripherals/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


