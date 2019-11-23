################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../VFP_Enable/vfp_enable.s 

OBJS += \
./VFP_Enable/vfp_enable.o 

S_DEPS += \
./VFP_Enable/vfp_enable.d 


# Each subdirectory must supply rules for building sources it contributes
VFP_Enable/%.o: ../VFP_Enable/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Assembler 5'
	armasm --cpu=Cortex-A9.no_neon --fpu=vfpv3 -g --md --depend_format=unix_escaped -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


