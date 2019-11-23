################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FatFS/diskio_cyclonev.c \
../FatFS/ff.c \
../FatFS/ffsystem.c \
../FatFS/ffunicode.c 

OBJS += \
./FatFS/diskio_cyclonev.o \
./FatFS/ff.o \
./FatFS/ffsystem.o \
./FatFS/ffunicode.o 

C_DEPS += \
./FatFS/diskio_cyclonev.d \
./FatFS/ff.d \
./FatFS/ffsystem.d \
./FatFS/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
FatFS/%.o: ../FatFS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


