################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/touch_module/ctsu/button_config.c \
../src/touch_module/ctsu/ctsu_config0.c \
../src/touch_module/ctsu/ctsu_configs.c \
../src/touch_module/ctsu/touch_config0.c \
../src/touch_module/ctsu/touch_configs.c 

COMPILER_OBJS += \
src/touch_module/ctsu/button_config.obj \
src/touch_module/ctsu/ctsu_config0.obj \
src/touch_module/ctsu/ctsu_configs.obj \
src/touch_module/ctsu/touch_config0.obj \
src/touch_module/ctsu/touch_configs.obj 

C_DEPS += \
src/touch_module/ctsu/button_config.d \
src/touch_module/ctsu/ctsu_config0.d \
src/touch_module/ctsu/ctsu_configs.d \
src/touch_module/ctsu/touch_config0.d \
src/touch_module/ctsu/touch_configs.d 

# Each subdirectory must supply rules for building sources it contributes
src/touch_module/ctsu/%.obj: ../src/touch_module/ctsu/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","D:\Project\IRO3\Wip\Source\IRO3\src\apps","D:\Project\IRO3\Wip\Source\IRO3\src\apps\flash","D:\Project\IRO3\Wip\Source\IRO3\src\apps\gpio","D:\Project\IRO3\Wip\Source\IRO3\src\apps\timer","D:\Project\IRO3\Wip\Source\IRO3\src\apps\uart","D:\Project\IRO3\Wip\Source\IRO3\src\apps\adc","D:\Project\IRO3\Wip\Source\IRO3\src\apps\util","D:\Project\IRO3\Wip\Source\IRO3\src","D:\Project\IRO3\Wip\Source\IRO3\src\apps\display","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module\r_ctsu_rx","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module\r_touch_button","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module\r_touch_rx","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module\ctsu","D:\Project\IRO3\Wip\Source\IRO3\src\apps\touch_app","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT1","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx\src","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_mpc_rx","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_mpc_rx\src","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\flash_type_1","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\flash_type_2","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\flash_type_3","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\flash_type_4","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\targets","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\general","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","D:\Project\IRO3\Wip\Source\IRO3\src\apps","D:\Project\IRO3\Wip\Source\IRO3\src\apps\flash","D:\Project\IRO3\Wip\Source\IRO3\src\apps\gpio","D:\Project\IRO3\Wip\Source\IRO3\src\apps\timer","D:\Project\IRO3\Wip\Source\IRO3\src\apps\uart","D:\Project\IRO3\Wip\Source\IRO3\src\apps\adc","D:\Project\IRO3\Wip\Source\IRO3\src\apps\util","D:\Project\IRO3\Wip\Source\IRO3\src","D:\Project\IRO3\Wip\Source\IRO3\src\apps\display","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module\r_ctsu_rx","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module\r_touch_button","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module\r_touch_rx","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module","D:\Project\IRO3\Wip\Source\IRO3\src\touch_module\ctsu","D:\Project\IRO3\Wip\Source\IRO3\src\apps\touch_app","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT0","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_PORT","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_CMT1","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_gpio_rx\src","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_mpc_rx","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_mpc_rx\src","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\Config_S12AD0","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\flash_type_1","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\flash_type_2","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\flash_type_3","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\flash_type_4","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_flash_rx\src\targets","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\general","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg" -lang=c99 -nomessage -output=obj -obj_path="src/touch_module/ctsu" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

