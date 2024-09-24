################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../myUtilities/Fonts/font12.c \
../myUtilities/Fonts/font16.c \
../myUtilities/Fonts/font20.c \
../myUtilities/Fonts/font24.c \
../myUtilities/Fonts/font8.c 

OBJS += \
./myUtilities/Fonts/font12.o \
./myUtilities/Fonts/font16.o \
./myUtilities/Fonts/font20.o \
./myUtilities/Fonts/font24.o \
./myUtilities/Fonts/font8.o 

C_DEPS += \
./myUtilities/Fonts/font12.d \
./myUtilities/Fonts/font16.d \
./myUtilities/Fonts/font20.d \
./myUtilities/Fonts/font24.d \
./myUtilities/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
myUtilities/Fonts/%.o myUtilities/Fonts/%.su myUtilities/Fonts/%.cyclo: ../myUtilities/Fonts/%.c myUtilities/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -IC:/Users/Chet/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/Users/Chet/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -IC:/Users/Chet/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/Chet/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.2/Drivers/CMSIS/Include -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/myDrivers/Inc" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/myUtilities" -I"C:/Users/Chet/Desktop/Guitar_Multi_Effects_Pedal/Guitar-Multi-Effects-Pedal/myUtilities/Fonts" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-myUtilities-2f-Fonts

clean-myUtilities-2f-Fonts:
	-$(RM) ./myUtilities/Fonts/font12.cyclo ./myUtilities/Fonts/font12.d ./myUtilities/Fonts/font12.o ./myUtilities/Fonts/font12.su ./myUtilities/Fonts/font16.cyclo ./myUtilities/Fonts/font16.d ./myUtilities/Fonts/font16.o ./myUtilities/Fonts/font16.su ./myUtilities/Fonts/font20.cyclo ./myUtilities/Fonts/font20.d ./myUtilities/Fonts/font20.o ./myUtilities/Fonts/font20.su ./myUtilities/Fonts/font24.cyclo ./myUtilities/Fonts/font24.d ./myUtilities/Fonts/font24.o ./myUtilities/Fonts/font24.su ./myUtilities/Fonts/font8.cyclo ./myUtilities/Fonts/font8.d ./myUtilities/Fonts/font8.o ./myUtilities/Fonts/font8.su

.PHONY: clean-myUtilities-2f-Fonts

