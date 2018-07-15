################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/apps/timer/timer.c 

COMPILER_OBJS += \
src/apps/timer/timer.obj 

C_DEPS += \
src/apps/timer/timer.d 

# Each subdirectory must supply rules for building sources it contributes
src/apps/timer/%.obj: ../src/apps/timer/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\flash","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\gpio","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\timer","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\uart","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\adc","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\general","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\flash","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\gpio","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\timer","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\uart","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\adc","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\general","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -output=obj -obj_path="src/apps/timer" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

