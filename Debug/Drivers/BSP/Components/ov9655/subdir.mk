################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/ov9655/ov9655.c 

OBJS += \
./Drivers/BSP/Components/ov9655/ov9655.o 

C_DEPS += \
./Drivers/BSP/Components/ov9655/ov9655.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ov9655/%.o Drivers/BSP/Components/ov9655/%.su Drivers/BSP/Components/ov9655/%.cyclo: ../Drivers/BSP/Components/ov9655/%.c Drivers/BSP/Components/ov9655/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -IC:/Users/Chet/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/Users/Chet/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -IC:/Users/Chet/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/Chet/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/CMSIS/Include -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/BSP/Components/rk043fn48h" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/BSP/Components/wm8994" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/BSP/Components/ft5336" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/BSP/STM32746G-Discovery" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/BSP/Components/ov9655" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/myUtilities/Fonts" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/BSP/Components/n25q128a" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/BSP/Components/Common" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/HAL_Drivers/Src" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/HAL_Drivers/Inc/Legacy" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/Drivers/HAL_Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-ov9655

clean-Drivers-2f-BSP-2f-Components-2f-ov9655:
	-$(RM) ./Drivers/BSP/Components/ov9655/ov9655.cyclo ./Drivers/BSP/Components/ov9655/ov9655.d ./Drivers/BSP/Components/ov9655/ov9655.o ./Drivers/BSP/Components/ov9655/ov9655.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-ov9655

