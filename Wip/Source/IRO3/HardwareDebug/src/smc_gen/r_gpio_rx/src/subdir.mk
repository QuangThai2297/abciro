################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_gpio_rx/src/r_gpio_rx.c 

COMPILER_OBJS += \
src/smc_gen/r_gpio_rx/src/r_gpio_rx.obj 

C_DEPS += \
src/smc_gen/r_gpio_rx/src/r_gpio_rx.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_gpio_rx/src/%.obj: ../src/smc_gen/r_gpio_rx/src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\gpio","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\timer","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\uart","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\adc","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\util","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\display","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module\r_ctsu_rx","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module\r_touch_button","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module\r_touch_rx","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module\ctsu","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\touch_app","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT1","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx\src","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_mpc_rx","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_mpc_rx\src","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\general","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\gpio","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\timer","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\uart","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\adc","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\util","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\display","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module\r_ctsu_rx","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module\r_touch_button","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module\r_touch_rx","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\touch_module\ctsu","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\apps\touch_app","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT1","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx\src","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_mpc_rx","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_mpc_rx\src","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\general","D:\trongkn\outsource\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -output=obj -obj_path="src/smc_gen/r_gpio_rx/src" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

