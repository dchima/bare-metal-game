################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HeroGame/Images/avengers_IronShot.c \
../HeroGame/Images/avengers_Limage0.c \
../HeroGame/Images/avengers_Limage1.c \
../HeroGame/Images/avengers_Limage2.c \
../HeroGame/Images/avengers_Limage3.c \
../HeroGame/Images/avengers_Limage4.c \
../HeroGame/Images/avengers_Limage5.c \
../HeroGame/Images/avengers_Limage6.c \
../HeroGame/Images/avengers_enemy1.c \
../HeroGame/Images/avengers_pCap.c \
../HeroGame/Images/avengers_pIron.c \
../HeroGame/Images/avengers_pPanther.c \
../HeroGame/Images/avengers_pScarlet.c \
../HeroGame/Images/avengers_pThor.c \
../HeroGame/Images/avengers_pWidow.c \
../HeroGame/Images/avengers_sCap.c \
../HeroGame/Images/avengers_sCapText.c \
../HeroGame/Images/avengers_sIron.c \
../HeroGame/Images/avengers_sIronText.c \
../HeroGame/Images/avengers_sLogo.c \
../HeroGame/Images/avengers_sPanther.c \
../HeroGame/Images/avengers_sPantherText.c \
../HeroGame/Images/avengers_sScarlet.c \
../HeroGame/Images/avengers_sScarletText.c \
../HeroGame/Images/avengers_sThor.c \
../HeroGame/Images/avengers_sThorText.c \
../HeroGame/Images/avengers_sTitle.c \
../HeroGame/Images/avengers_sWidow.c \
../HeroGame/Images/avengers_sWidowText.c \
../HeroGame/Images/avengers_thanos.c 

OBJS += \
./HeroGame/Images/avengers_IronShot.o \
./HeroGame/Images/avengers_Limage0.o \
./HeroGame/Images/avengers_Limage1.o \
./HeroGame/Images/avengers_Limage2.o \
./HeroGame/Images/avengers_Limage3.o \
./HeroGame/Images/avengers_Limage4.o \
./HeroGame/Images/avengers_Limage5.o \
./HeroGame/Images/avengers_Limage6.o \
./HeroGame/Images/avengers_enemy1.o \
./HeroGame/Images/avengers_pCap.o \
./HeroGame/Images/avengers_pIron.o \
./HeroGame/Images/avengers_pPanther.o \
./HeroGame/Images/avengers_pScarlet.o \
./HeroGame/Images/avengers_pThor.o \
./HeroGame/Images/avengers_pWidow.o \
./HeroGame/Images/avengers_sCap.o \
./HeroGame/Images/avengers_sCapText.o \
./HeroGame/Images/avengers_sIron.o \
./HeroGame/Images/avengers_sIronText.o \
./HeroGame/Images/avengers_sLogo.o \
./HeroGame/Images/avengers_sPanther.o \
./HeroGame/Images/avengers_sPantherText.o \
./HeroGame/Images/avengers_sScarlet.o \
./HeroGame/Images/avengers_sScarletText.o \
./HeroGame/Images/avengers_sThor.o \
./HeroGame/Images/avengers_sThorText.o \
./HeroGame/Images/avengers_sTitle.o \
./HeroGame/Images/avengers_sWidow.o \
./HeroGame/Images/avengers_sWidowText.o \
./HeroGame/Images/avengers_thanos.o 

C_DEPS += \
./HeroGame/Images/avengers_IronShot.d \
./HeroGame/Images/avengers_Limage0.d \
./HeroGame/Images/avengers_Limage1.d \
./HeroGame/Images/avengers_Limage2.d \
./HeroGame/Images/avengers_Limage3.d \
./HeroGame/Images/avengers_Limage4.d \
./HeroGame/Images/avengers_Limage5.d \
./HeroGame/Images/avengers_Limage6.d \
./HeroGame/Images/avengers_enemy1.d \
./HeroGame/Images/avengers_pCap.d \
./HeroGame/Images/avengers_pIron.d \
./HeroGame/Images/avengers_pPanther.d \
./HeroGame/Images/avengers_pScarlet.d \
./HeroGame/Images/avengers_pThor.d \
./HeroGame/Images/avengers_pWidow.d \
./HeroGame/Images/avengers_sCap.d \
./HeroGame/Images/avengers_sCapText.d \
./HeroGame/Images/avengers_sIron.d \
./HeroGame/Images/avengers_sIronText.d \
./HeroGame/Images/avengers_sLogo.d \
./HeroGame/Images/avengers_sPanther.d \
./HeroGame/Images/avengers_sPantherText.d \
./HeroGame/Images/avengers_sScarlet.d \
./HeroGame/Images/avengers_sScarletText.d \
./HeroGame/Images/avengers_sThor.d \
./HeroGame/Images/avengers_sThorText.d \
./HeroGame/Images/avengers_sTitle.d \
./HeroGame/Images/avengers_sWidow.d \
./HeroGame/Images/avengers_sWidowText.d \
./HeroGame/Images/avengers_thanos.d 


# Each subdirectory must supply rules for building sources it contributes
HeroGame/Images/%.o: ../HeroGame/Images/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9.no_neon --fpu=vfpv3 --gnu -O0 -g --md --depend_format=unix_escaped -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


