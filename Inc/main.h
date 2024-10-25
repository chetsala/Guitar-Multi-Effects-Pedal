/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "wm8994.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_audio.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define AUDIO_FREQ AUDIO_FREQUENCY_48K
// AUDIO_FREQUENCY_96K
//AUDIO_FREQUENCY_48K

#define VOLUME 100
#define BUFFER_SIZE_SAMPLES 240
// 8192
#define DMA_BYTES_PER_FRAME 8
#define DMA_BYTES_PER_MSIZE 2
#define DMA_BUFFER_SIZE_BYTES BUFFER_SIZE_SAMPLES *DMA_BYTES_PER_FRAME
#define DMA_BUFFER_SIZE_MSIZE DMA_BUFFER_SIZE_BYTES / DMA_BYTES_PER_MSIZE


#define CAMERA_RES_MAX_X          640
#define CAMERA_RES_MAX_Y          480
#define RGB565_BYTE_PER_PIXEL     2
#define ARBG8888_BYTE_PER_PIXEL   4
#define LCD_FRAME_BUFFER          SDRAM_DEVICE_ADDR
#define CAMERA_FRAME_BUFFER       ((uint32_t)(LCD_FRAME_BUFFER + (RK043FN48H_WIDTH * RK043FN48H_HEIGHT * ARBG8888_BYTE_PER_PIXEL)))
#define SDRAM_WRITE_READ_ADDR        ((uint32_t)(CAMERA_FRAME_BUFFER + (CAMERA_RES_MAX_X * CAMERA_RES_MAX_Y * RGB565_BYTE_PER_PIXEL)))

#define AUDIO_REC_START_ADDR         SDRAM_WRITE_READ_ADDR

#define BUFFER_SIZE 8192
#define BIT_RES     16
#define CHAN_NUMB   2
#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE)
#define AUDIO_NB_BLOCKS    ((uint32_t)4)

#define SAMPLE_RATE 48000 // Sample rate in Hz
#define FREQUENCY 440     // Frequency of the sine wave in Hz (A4 note)
#define AMPLITUDE 16000   // Amplitude of the sine wave (half of 16-bit max value)
#define PI 3.14159265359  // Pi value for sine calculations




/* Exported macro ------------------------------------------------------------*/

extern LTDC_HandleTypeDef hltdc;

extern DMA2D_HandleTypeDef hdma2d;


extern SAI_HandleTypeDef haudio_out_sai;
extern SAI_HandleTypeDef haudio_in_sai;
extern volatile int16_t sampleInBuffer[BUFFER_SIZE_SAMPLES];
extern volatile int16_t sampleOutBuffer[BUFFER_SIZE_SAMPLES];
extern volatile uint8_t saiDMATxBuffer[DMA_BUFFER_SIZE_BYTES];
extern volatile uint8_t saiDMARxBuffer[DMA_BUFFER_SIZE_BYTES];




/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);



/* Private defines -----------------------------------------------------------*/




#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
