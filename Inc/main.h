/* main.h */

#ifndef MAIN_H
#define MAIN_H

/* Standard C libraries */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* STM32 HAL and CMSIS */
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_tim.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_i2c.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_i2s.h"
#include "stm32f7xx_hal_def.h"

/* Board Support Package (BSP) */
#include "stm32746g_discovery.h"


/* Function prototypes */
void SystemClock_Config(void);
void Error_Handler(void);
void CPU_CACHE_Enable();
void HAL_Init_Config(void);

#endif