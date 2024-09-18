################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Src/stm32746g_discovery.c \
../Drivers/BSP/Src/stm32746g_discovery_audio.c 

OBJS += \
./Drivers/BSP/Src/stm32746g_discovery.o \
./Drivers/BSP/Src/stm32746g_discovery_audio.o 

C_DEPS += \
./Drivers/BSP/Src/stm32746g_discovery.d \
./Drivers/BSP/Src/stm32746g_discovery_audio.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Src/%.o Drivers/BSP/Src/%.su Drivers/BSP/Src/%.cyclo: ../Drivers/BSP/Src/%.c Drivers/BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/BSP/Components/wm8994 -I../Drivers/BSP/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Src

clean-Drivers-2f-BSP-2f-Src:
	-$(RM) ./Drivers/BSP/Src/stm32746g_discovery.cyclo ./Drivers/BSP/Src/stm32746g_discovery.d ./Drivers/BSP/Src/stm32746g_discovery.o ./Drivers/BSP/Src/stm32746g_discovery.su ./Drivers/BSP/Src/stm32746g_discovery_audio.cyclo ./Drivers/BSP/Src/stm32746g_discovery_audio.d ./Drivers/BSP/Src/stm32746g_discovery_audio.o ./Drivers/BSP/Src/stm32746g_discovery_audio.su

.PHONY: clean-Drivers-2f-BSP-2f-Src

