################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -I"C:/Users/Kurat/Documents/git_repos/LwIP/Inc" -I"C:/Users/Kurat/Documents/git_repos/LwIP/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Kurat/Documents/git_repos/LwIP/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Kurat/Documents/git_repos/LwIP/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Kurat/Documents/git_repos/LwIP/Drivers/CMSIS/Include" -I"C:/Users/Kurat/Documents/git_repos/LwIP/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/Kurat/Documents/git_repos/LwIP/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I"C:/Users/Kurat/Documents/git_repos/LwIP/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


