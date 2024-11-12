cmake_minimum_required(VERSION 3.22)

#
# User is free to modify the file as much as necessary
#

#list(APPEND CMAKE_MODULE_PATH "C:/Users/user1/Downloads/CmakeTestProject/cmake")
list(APPEND CMAKE_MODULE_PATH "{{sr:cmake_path}}")
message("Build CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
include("cmake/gcc-arm-none-eabi.cmake")
message("Build CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

# Core project settings
project(Multi-Effects-Pedal)
enable_language(C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Core MCU flags, CPU type, instruction set and FPU setup
set(cpu_PARAMS -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16)


# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add system_stm32f7xx.c to the build
target_sources(${PROJECT_NAME} PRIVATE ${SYSTEM_SRC})



# Sources
set(sources_SRCS
    ${PROJECT_SOURCE_DIR}/Src/main.c
    ${PROJECT_SOURCE_DIR}/Src/audio_processing.c
    ${PROJECT_SOURCE_DIR}/Src/ui_interface.c
    ${PROJECT_SOURCE_DIR}/Src/system_stm32f7xx.c
    ${PROJECT_SOURCE_DIR}/Src/stm32f7xx_it.c
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_audio.c
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.c
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc_ex.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/Components/wm8994/wm8994.c



)



####
#${PROJECT_SOURCE_DIR}/Src/sysmem.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/stm32f7xx_hal_msp.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/stm32f7xx_it.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/ft5336.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/stm32746g_discovery_audio.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/stm32746g_discovery_lcd.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/stm32746g_discovery_sdram.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/stm32746g_discovery_ts.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/stm32746g_discovery.c
#${PROJECT_SOURCE_DIR}/Drivers/Src/wm8994.c

#${PROJECT_SOURCE_DIR}/Utilities/Fonts/font8.c
#${PROJECT_SOURCE_DIR}/Utilities/Fonts/font12.c
#${PROJECT_SOURCE_DIR}/Utilities/Fonts/font16.c
#${PROJECT_SOURCE_DIR}/Utilities/Fonts/font20.c
#${PROJECT_SOURCE_DIR}/Utilities/Fonts/font24.c



 

# Include directories for all compilers
set(include_DIRS
    ${PROJECT_SOURCE_DIR}/Inc
    ${PROJECT_SOURCE_DIR}/Drivers/Inc
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/STM32746G-Discovery
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/Components/Common
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/Components/wm8994
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/Components/ft5336
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/Components/rk043fn48h
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/Components/w25q128j
    ${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc
    ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${PROJECT_SOURCE_DIR}/Utilities/Fonts
)

# Include directories for each compiler
set(include_c_DIRS)
set(include_cxx_DIRS)
set(include_asm_DIRS)

# Symbols definition for all compilers
set(symbols_SYMB)

# Symbols definition for each compiler
set(symbols_c_SYMB)
set(symbols_cxx_SYMB)
set(symbols_asm_SYMB)

# Link directories and names of libraries
set(link_DIRS)
set(link_LIBS)

# Linker script
set(linker_script_SRC "${CMAKE_SOURCE_DIR}/STM32F746NGHx_FLASH.ld")

# Compiler options
set(compiler_OPTS)

# Linker options
set(linker_OPTS)

# Now call generated cmake
# This will add script generated
# information to the project
include("cmake/vscode_generated.cmake")

# Link directories setup
# Must be before executable is added
link_directories(${CMAKE_PROJECT_NAME} ${link_DIRS})

# Create an executable object type
add_executable(${PROJECT_NAME}.elf ${sources_SRCS})

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${sources_SRCS})

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${include_DIRS}
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${include_cxx_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    ${symbols_SYMB}
    $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
    $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
    $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

    # Configuration specific
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>: >
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${link_LIBS})

# Compiler options

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    ${cpu_PARAMS}
    -Wall -Wextra -Wpedantic -Wno-unused-parameter
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-O3>
)

# Linker options
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T${linker_script_SRC}
    ${cpu_PARAMS}
    -Wl,-Map=${PROJECT_NAME}.map
    --specs=nosys.specs
)

# Execute post-build to print size, generate hex and bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)
