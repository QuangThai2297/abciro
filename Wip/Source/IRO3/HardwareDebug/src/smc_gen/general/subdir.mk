################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/general/r_cg_hardware_setup.c \
../src/smc_gen/general/r_smc_cgc.c \
../src/smc_gen/general/r_smc_cgc_user.c \
../src/smc_gen/general/r_smc_interrupt.c 

COMPILER_OBJS += \
src/smc_gen/general/r_cg_hardware_setup.obj \
src/smc_gen/general/r_smc_cgc.obj \
src/smc_gen/general/r_smc_cgc_user.obj \
src/smc_gen/general/r_smc_interrupt.obj 

C_DEPS += \
src/smc_gen/general/r_cg_hardware_setup.d \
src/smc_gen/general/r_smc_cgc.d \
src/smc_gen/general/r_smc_cgc_user.d \
src/smc_gen/general/r_smc_interrupt.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/general/%.obj: ../src/smc_gen/general/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\flash","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\gpio","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\timer","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\uart","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\adc","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\general","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\flash","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\gpio","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\timer","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\uart","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\adc","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\general","K:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -output=obj -obj_path="src/smc_gen/general" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

