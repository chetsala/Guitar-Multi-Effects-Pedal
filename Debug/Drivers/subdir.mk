################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/stm32f7xx_hal_uart.c 

OBJS += \
./Drivers/stm32f7xx_hal_uart.o 

C_DEPS += \
./Drivers/stm32f7xx_hal_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o Drivers/%.su Drivers/%.cyclo: ../Drivers/%.c Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../myDrivers/Inc -I../Core/Inc -I../my_Utilities/Fonts -I../Drivers/BSP/Components/wm8994 -I../Drivers/BSP/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers

clean-Drivers:
	-$(RM) ./Drivers/stm32f7xx_hal_uart.cyclo ./Drivers/stm32f7xx_hal_uart.d ./Drivers/stm32f7xx_hal_uart.o ./Drivers/stm32f7xx_hal_uart.su

.PHONY: clean-Drivers

