################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/Config_CMT0/Config_CMT0.c \
../src/smc_gen/Config_CMT0/Config_CMT0_user.c 

COMPILER_OBJS += \
src/smc_gen/Config_CMT0/Config_CMT0.obj \
src/smc_gen/Config_CMT0/Config_CMT0_user.obj 

C_DEPS += \
src/smc_gen/Config_CMT0/Config_CMT0.d \
src/smc_gen/Config_CMT0/Config_CMT0_user.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_CMT0/%.obj: ../src/smc_gen/Config_CMT0/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","D:\Project\IRO3\Wip\Source\IRO3\src\apps","D:\Project\IRO3\Wip\Source\IRO3\src\apps\flash","D:\Project\IRO3\Wip\Source\IRO3\src\apps\gpio","D:\Project\IRO3\Wip\Source\IRO3\src\apps\timer","D:\Project\IRO3\Wip\Source\IRO3\src\apps\uart","D:\Project\IRO3\Wip\Source\IRO3\src\apps\adc","D:\Project\IRO3\Wip\Source\IRO3\src","D:\Project\IRO3\Wip\Source\IRO3\src\apps\display","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT1","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx\src","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\general","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","D:\Project\IRO3\Wip\Source\IRO3\src\apps","D:\Project\IRO3\Wip\Source\IRO3\src\apps\flash","D:\Project\IRO3\Wip\Source\IRO3\src\apps\gpio","D:\Project\IRO3\Wip\Source\IRO3\src\apps\timer","D:\Project\IRO3\Wip\Source\IRO3\src\apps\uart","D:\Project\IRO3\Wip\Source\IRO3\src\apps\adc","D:\Project\IRO3\Wip\Source\IRO3\src","D:\Project\IRO3\Wip\Source\IRO3\src\apps\display","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT1","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx\src","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\general","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -output=obj -obj_path="src/smc_gen/Config_CMT0" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.
