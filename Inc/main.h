/* main.h */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


/* Macros---------------------------------------------------------------------*/


#define DELAY          0
#define OCTAVE         1
#define CHORUS         2
#define PHASER         3
#define WAH            4
#define REVERB         5
#define DISTORSION     6
#define FLANGER        7
#define TREMOLO        8
#define VIBRATO        9
#define EQ             10
#define RINGMOD        11
#define LEFT           0
#define MENU_1         14
#define MENU_2         15
#define RIGHT          1
#define HOME           2
#define LEFT_MENU      16
#define RIGHT_MENU     17


#define RGB565_BYTE_PER_PIXEL        2
#define ARBG8888_BYTE_PER_PIXEL      4

// Camera have a max resolution of VGA : 640x480
#define CAMERA_RES_MAX_X             640
#define CAMERA_RES_MAX_Y             480

#define SDRAM_WRITE_READ_ADDR_OFFSET ((uint32_t)0x0800)
#define SRAM_WRITE_READ_ADDR_OFFSET  SDRAM_WRITE_READ_ADDR_OFFSET

// All the SDRAM BUFFERS WE ARE USING

#define AUDIO_BLOCK_SIZE             ((uint32_t)512)
#define AUDIO_BLOCK_HALFSIZE         ((uint32_t)256)

#define LCD_FRAME_BUFFER             LCD_FB_START_ADDRESS

#define BMP_IMAGE_BUFFER             LCD_FRAME_BUFFER + (RK043FN48H_WIDTH * RK043FN48H_HEIGHT * ARBG8888_BYTE_PER_PIXEL) * 2     /* The buffer has 2 layers (2 * screen size) */

#define AUDIO_REC_START_ADDR         BMP_IMAGE_BUFFER + 0x07D000                                                                 /* The buffer has 512000 bytes */

#define AUDIO_PLAY_BUFFER            AUDIO_REC_START_ADDR + AUDIO_BLOCK_SIZE * 4                                                 /* The buffer has 512 samples */

#define AUDIO_PROCESSED_BUFFER      AUDIO_PLAY_BUFFER + AUDIO_BLOCK_SIZE * 4                                                    /* The buffer has 512 samples */

#define AUDIO_DELAY_BUFFER           AUDIO_PROCESSED_BUFFER + AUDIO_BLOCK_SIZE * 4                                                 /* The delay has 50k samples */

#define KNOB_35x35x25             AUDIO_DELAY_BUFFER + (50000 * 4)                                                            /* The knobs were moved from flash to SDRAM */

#define KNOB_42x41x25             KNOB_35x35x25 + (122528)                                                                 /* The knobs were moved from flash to SDRAM */

#define KNOB_52x52x25             KNOB_42x41x25 + (172224)                                                                 /* The knobs were moved from flash to SDRAM */

#define KNOB_EQ_SLIDER            KNOB_52x52x25 + (270432)                                                                 /* The knobs were moved from flash to SDRAM */

#define AUDIO_WAV_RECORD_BUFFER      KNOB_EQ_SLIDER + (258976)                                                                /* We have approximately 40 seconds of recording time remaining */

// Definition for TIMx's NVIC
#define TIMx_IRQn              TIM3_IRQn
#define TIMx_IRQHandler        TIM3_IRQHandler


#define COUNT_OF_EXAMPLE(x)              (sizeof(x) / sizeof(BSP_DemoTypedef))


// Enable Features ---------------------------------------------------
#define EXTERNAL_WHEEL_ENABLE 0

#define AUDIO_ENABLE          1

#define SCREEN_ENABLE         1
// CODEC VOLUME SETTINGS
#define CODEC_OUT_VOLUME 80 // at 80 no noise in headphone
#define CODEC_IN_VOLUME	 92

/* Typedef---------------------------------------------------------------------*/
typedef enum {
    AUDIO_ERROR_NONE = 0,
    AUDIO_ERROR_NOTREADY,
    AUDIO_ERROR_IO,
    AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;




/* Global Variables---------------------------------------------------------------------*/
extern uint32_t ErrorCounter;



/* Exported functions ------------------------------------------------------- */
uint8_t AUDIO_Process(void);
uint8_t CheckForUserInput(void);
void BSP_LCD_DMA2D_IRQHandler(void);


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
#include "stm32746g_audio.h"
#include "fatfs_storage.h"
#include "diskio.h"
#include "ff.h"


/* Custom Drivers */
#include "ui_interface.h"

/* Function prototypes */
void SystemClock_Config(void);
void Error_Handler(void);
void CPU_CACHE_Enable();
void HAL_Init_Config(void);

#endif