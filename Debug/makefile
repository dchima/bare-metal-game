################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include WM8731_AudioEngine/subdir.mk
-include VFP_Enable/subdir.mk
-include SnakeGame/Sounds/subdir.mk
-include SnakeGame/subdir.mk
-include LT24_GraphicsEngine/subdir.mk
-include IO_Peripherals/subdir.mk
-include HeroGame/Sounds/subdir.mk
-include HeroGame/Images/subdir.mk
-include HeroGame/subdir.mk
-include HPS_usleep/subdir.mk
-include HPS_Timers/subdir.mk
-include HPS_IRQ/subdir.mk
-include HPS_I2C/subdir.mk
-include GameMenu/images/subdir.mk
-include GameMenu/subdir.mk
-include FatFS/hwlib/subdir.mk
-include FatFS/subdir.mk
-include DE1SoC_LT24/subdir.mk
-include BasicFont/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: EmbedGame.axf

# Tool invocations
EmbedGame.axf: $(OBJS) C:/Users/Azeem/Documents/DS-5\ Workspace/EmbedGame/DDRRomRamVFP.scat $(USER_OBJS) $(LIBS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker 5'
	armlink --cpu=Cortex-A9.no_neon --fpu=vfpv3 --entry=__enable_vfp --scatter="C:/Users/Azeem/Documents/DS-5 Workspace/EmbedGame/DDRRomRamVFP.scat" --info=sizes -o "EmbedGame.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS) $(C++_DEPS) $(ASM_DEPS) $(C_DEPS) $(CC_DEPS) $(ASM_UPPER_DEPS) $(CPP_DEPS) $(S_DEPS) $(EXECUTABLES) $(CXX_DEPS) $(C_UPPER_DEPS) $(S_UPPER_DEPS) EmbedGame.axf 
	-@echo ' '

post-build:
	-@echo 'Extract Binary File from Compiler Output'
	-fromelf --bin --output=baremetal.bin EmbedGame.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
