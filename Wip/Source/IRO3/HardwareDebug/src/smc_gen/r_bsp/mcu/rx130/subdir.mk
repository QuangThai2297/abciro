################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_bsp/mcu/rx130/cpu.c \
../src/smc_gen/r_bsp/mcu/rx130/locking.c \
../src/smc_gen/r_bsp/mcu/rx130/mcu_clocks.c \
../src/smc_gen/r_bsp/mcu/rx130/mcu_init.c \
../src/smc_gen/r_bsp/mcu/rx130/mcu_interrupts.c \
../src/smc_gen/r_bsp/mcu/rx130/mcu_locks.c 

COMPILER_OBJS += \
src/smc_gen/r_bsp/mcu/rx130/cpu.obj \
src/smc_gen/r_bsp/mcu/rx130/locking.obj \
src/smc_gen/r_bsp/mcu/rx130/mcu_clocks.obj \
src/smc_gen/r_bsp/mcu/rx130/mcu_init.obj \
src/smc_gen/r_bsp/mcu/rx130/mcu_interrupts.obj \
src/smc_gen/r_bsp/mcu/rx130/mcu_locks.obj 

C_DEPS += \
src/smc_gen/r_bsp/mcu/rx130/cpu.d \
src/smc_gen/r_bsp/mcu/rx130/locking.d \
src/smc_gen/r_bsp/mcu/rx130/mcu_clocks.d \
src/smc_gen/r_bsp/mcu/rx130/mcu_init.d \
src/smc_gen/r_bsp/mcu/rx130/mcu_interrupts.d \
src/smc_gen/r_bsp/mcu/rx130/mcu_locks.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_bsp/mcu/rx130/%.obj: ../src/smc_gen/r_bsp/mcu/rx130/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\general","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg","D:\Project\IRO3\Wip\Source\IRO3\src\apps","D:\Project\IRO3\Wip\Source\IRO3\src\apps\flash","D:\Project\IRO3\Wip\Source\IRO3\src\apps\gpio","D:\Project\IRO3\Wip\Source\IRO3\src\apps\timer","D:\Project\IRO3\Wip\Source\IRO3\src\apps\uart","D:\Project\IRO3\Wip\Source\IRO3\src\apps\adc","D:\Project\IRO3\Wip\Source\IRO3\src" -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_7_0\include","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_bsp","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_config","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\general","D:\Project\IRO3\Wip\Source\IRO3\src\smc_gen\r_pincfg","D:\Project\IRO3\Wip\Source\IRO3\src\apps","D:\Project\IRO3\Wip\Source\IRO3\src\apps\flash","D:\Project\IRO3\Wip\Source\IRO3\src\apps\gpio","D:\Project\IRO3\Wip\Source\IRO3\src\apps\timer","D:\Project\IRO3\Wip\Source\IRO3\src\apps\uart","D:\Project\IRO3\Wip\Source\IRO3\src\apps\adc","D:\Project\IRO3\Wip\Source\IRO3\src" -lang=c99 -nomessage -output=obj -obj_path="src/smc_gen/r_bsp/mcu/rx130" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

