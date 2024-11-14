/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32 VS Code Extension
 * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "stdlib.h"
#include "main.h"
#include <stdio.h>
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stm32f7xx_it.h"
#include <math.h>

/* SD Card and File Handling */
char SD_Path[4];              /* SD card logical drive path */
static uint32_t uwBmplen = 0; /* Length of BMP images for display */
static uint8_t *uwInternelBuffer = (uint8_t *)(BMP_IMAGE_BUFFER);



/* Touchscreen and Menu Navigation */
TS_StateTypeDef rawTouchState;                                     /* Raw touch state from touch controller */
GUITouchState touchState;                                          /* Processed touch state for GUI */
volatile int32_t pedal_individual = 0;                             /* Indicates individual pedal mode */
volatile int32_t selection_menu = MENU_1;                          /* Current menu selection */
LinkElementMenu *Left_Menu_Arrow = NULL, *Right_Menu_Arrow = NULL; /* Menu arrows */
volatile int32_t selection_pedal = 0;                              /* Selected pedal index */

/* Timer Handle for GUI updates */
TIM_HandleTypeDef htimx;

/* Audio Buffer and Recording Variables */
int32_t *Buffer_in = (int32_t *)AUDIO_REC_START_ADDR;                     /* Input audio buffer */
int32_t *Buffer_out = (int32_t *)AUDIO_PLAY_BUFFER;                       /* Output audio buffer */
int32_t *processed_buffer = (int32_t *)AUDIO_PROCESSED_BUFFER;            /* Working audio buffer */
static int32_t sample_count = 0, effect_count = 0;                        /* Counters for audio processing */
static uint8_t *AUDIO_RECORD_BUFFER = (uint8_t *)AUDIO_WAV_RECORD_BUFFER; /* Record buffer */

/* Audio Recording State Variables */
volatile bool recording_state = false;                     /* Recording state */
static uint32_t recording_timer = 0, recording_offset = 0; /* Timer and buffer offset for recording */
volatile bool block_machine = false;                       /* Flag to prevent state changes during processing */

/* SD Card Write Handling */
static uint32_t sd_write_position = 0; /* SD write position */
#define SECONDS 20                     /* Max recording duration in seconds */
#define SECTOR_SD_WRITE 256            /* Write block size for SD card */
unsigned int aux_cont_bytes = 0;       /* Auxiliary byte counter */
DIR directory;                         /* Directory structure for file system */
FILINFO file_info_for_search;          /* File information structure */
FRESULT f_result;                      /* Result of file operations */
uint16_t record_count = 0;             /* Number of records saved */

void Error_Handler(void);
void SystemClock_Config(void);
void CPU_CACHE_Enable(void);

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
    NONE_STATE = 0,
    INIT_SCREEN_AUDIO = 1,
    BUFFER_OFFSET_HALF = 2,
    BUFFER_OFFSET_FULL = 3,
    SCREEN_REFRESH = 4,
    START_RECORDING = 5,
    RECORDING_DONE = 6
} machine_states_t;

/* Private variables ---------------------------------------------------------*/
static volatile machine_states_t machine_state = INIT_SCREEN_AUDIO;

PedalElement *pedal_effects[13];

int main(void)
{
    /* Enable CPU Cache */
    CPU_CACHE_Enable();

    /* Initialize HAL and System Clock */
    HAL_Init_Config();

    /* Timer configuration for screen refresh */
    Tim3_DisplayRefresh_Config();

    /* Display and touchscreen initialization */
    BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    BSP_LCD_Init();
    LCD_Config();
    Storage_Init();

    init_LL_ConvertLine_DMA2D(CM_RGB888);

    /* Initialize pedal structures */
    FATFS_LinkDriver(&SD_Driver, SD_Path); // Link SD card driver
    initPedals();                          // Initialize pedal structures
    initEffects();                         // Initialize effects

    readKnobFiles(); // Load knob settings from files

    initWavHeader(AUDIO_RECORD_BUFFER); // Initialize WAV header in the audio record buffer

    sd_write_position = (DEFAULT_AUDIO_IN_FREQ * 3 * RECORDING_DURATION_SECONDS) / SD_WRITE_BLOCK_SIZE;
    InitializeDemoScreen();
    while (1)
    {
        switch (machine_state)
        {
        case NONE_STATE:
        {
            break;
        }

        case INIT_SCREEN_AUDIO:
        {
            /* Start  Playback */

            BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_INPUT_LINE_1, OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_IN_FREQ,
                                  DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR, CODEC_OUT_VOLUME, CODEC_IN_VOLUME);
            BSP_AUDIO_IN_Record((uint16_t *)Buffer_in, AUDIO_BLOCK_SIZE);
            BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
            BSP_AUDIO_OUT_Play((uint16_t *)Buffer_out, AUDIO_BLOCK_SIZE);
            NVIC_DisableIRQ((IRQn_Type)DMA2_Stream4_IRQn);

            machine_state = NONE_STATE;

            HAL_NVIC_SetPriority(TIM3_IRQn, AUDIO_IN_IRQ_PREPRIO - 1, 0);

            if (HAL_TIM_Base_Start_IT(&htimx) != HAL_OK)
            {
                /* Starting Error */
                Error_Handler();
            }
            break;
        }

        case BUFFER_OFFSET_HALF:
        {
            memcpy(processed_buffer, Buffer_in, AUDIO_BLOCK_HALFSIZE * sizeof(int32_t));

            for (sample_count = 1; sample_count < AUDIO_BLOCK_HALFSIZE; sample_count += 2)
            {
                if ((processed_buffer[sample_count] & 0x800000) == 0x800000)
                {
                    processed_buffer[sample_count] = ((~processed_buffer[sample_count] + 1) & 0xFFFFFF) * -1;
                }

                for (effect_count = 11; effect_count >= 0; effect_count--)
                {

                    if ((selection_pedal[effect_count])->push->push_state == GUI_ON)
                    {
                        processed_buffer[sample_count] = selection_pedal[effect_count]->effect(processed_buffer[sample_count]);
                    }
                }

                if (processed_buffer[sample_count] < 0)
                {
                    if (recording_state == true)
                    {
                        AUDIO_RECORD_BUFFER[recording_state] = processed_buffer[sample_count] & 0xFF;
                        recording_state++;
                        AUDIO_RECORD_BUFFER[recording_state] = (processed_buffer[sample_count] & (0xFF << 8)) >> 8;
                        recording_state++;
                        AUDIO_RECORD_BUFFER[recording_state] = (processed_buffer[sample_count] & (0xFF << 16)) >> 16;
                        recording_state++;
                    }

                    processed_buffer[sample_count] = (~((-1 * processed_buffer[sample_count]) - 1)) & 0xFFFFFF;
                }
                else
                {
                    if (recording_state == true)
                    {
                        AUDIO_RECORD_BUFFER[recording_state] = processed_buffer[sample_count] & 0xFF;
                        recording_state++;
                        AUDIO_RECORD_BUFFER[recording_state] = (processed_buffer[sample_count] & (0xFF << 8)) >> 8;
                        recording_state++;
                        AUDIO_RECORD_BUFFER[recording_state] = (processed_buffer[sample_count] & (0xFF << 16)) >> 16;
                        recording_state++;
                    }
                }
            }
            memcpy(Buffer_out, processed_buffer, AUDIO_BLOCK_HALFSIZE * sizeof(int32_t));
            if (machine_state == SCREEN_REFRESH)
            {
                break;
            }
            else
            {
                machine_state = NONE_STATE;
                block_machine = false;
            }
            break;
        }

        /* AUDIO */
        case BUFFER_OFFSET_FULL:
        {
            memcpy(processed_buffer, &Buffer_in[AUDIO_BLOCK_HALFSIZE], AUDIO_BLOCK_HALFSIZE * sizeof(int32_t));

            for (sample_count = 1; sample_count < AUDIO_BLOCK_HALFSIZE; sample_count += 2)
            {
                if ((processed_buffer[sample_count] & 0x800000) == 0x800000)
                {
                    processed_buffer[sample_count] = ((~processed_buffer[sample_count] + 1) & 0xFFFFFF) * -1;
                }

                for (effect_count = 11; effect_count >= 0; effect_count--)
                {
#if SCREEN_ENABLE
                    if ((selection_pedal[effect_count])->push->push_state == GUI_ON)
                    {
                        processed_buffer[sample_count] = selection_pedal[effect_count]->efecto(processed_buffer[sample_count]);
                    }
#endif
                }

                if (processed_buffer[sample_count] < 0)
                {
                    if (recording_state == true)
                    {
                        AUDIO_RECORD_BUFFER[recording_state] = processed_buffer[sample_count] & 0xFF;
                        recording_state++;
                        AUDIO_RECORD_BUFFER[recording_state] = (processed_buffer[sample_count] & (0xFF << 8)) >> 8;
                        recording_state++;
                        AUDIO_RECORD_BUFFER[recording_state] = (processed_buffer[sample_count] & (0xFF << 16)) >> 16;
                        recording_state++;
                    }

                    processed_buffer[sample_count] = (~((-1 * processed_buffer[sample_count]) - 1)) & 0xFFFFFF;
                }
                else
                {
                    if (recording_state == true)
                    {
                        AUDIO_RECORD_BUFFER[recording_state] = processed_buffer[sample_count] & 0xFF;
                        recording_state++;
                        AUDIO_RECORD_BUFFER[recording_state] = (processed_buffer[sample_count] & (0xFF << 8)) >> 8;
                        recording_state++;
                        AUDIO_RECORD_BUFFER[recording_state] = (processed_buffer[sample_count] & (0xFF << 16)) >> 16;
                        recording_state++;
                    }
                }
            }

            memcpy(&Buffer_out[AUDIO_BLOCK_HALFSIZE], processed_buffer, AUDIO_BLOCK_HALFSIZE * sizeof(int32_t));

            if (machine_state == SCREEN_REFRESH)
            {
                break;
            }
            else
            {
                machine_state = NONE_STATE;
                block_machine = false;
            }

            break;
        }

#if SCREEN_ENABLE
        case SCREEN_REFRESH:
        {

#if EXTERNAL_WHEEL_ENABLE
            if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
            {
                /* Start Conversation Error */
                Error_Handler();
            }
#endif
            NVIC_DisableIRQ((IRQn_Type)DMA2_Stream7_IRQn);
            NVIC_DisableIRQ(TIMx_IRQn);

            BSP_TS_GetState(&rawTouchState);
            guiUpdateTouch(&rawTouchState, &touchState);

            if (pedal_individual == 1)
            {
                if ((pedal_effects[selection_pedal]->dial->dials[0]->id) != 8)
                {
                    guiUpdate(pedal_effects[selection_pedal]->dial, &touchState);
                }

                handleIndividualPushButton(pedal_effects[selection_pedal], &touchState);
                linkRequestHandlers_pedal_individual(pedal_effects[selection_pedal], &touchState); // stopped here
            }
            else if (pedal_individual == 0)
            {
                PushRequestHandler_menu(pedal_effects, &touchState);
                linkRequestHandler_menu(pedal_effects, &touchState);
                linkRequestHandler_Flechas_Menu(Left_Menu_Arrow, &touchState);
                linkRequestHandler_Flechas_Menu(Right_Menu_Arrow, &touchState);
            }

            machine_state = NONE_STATE;

            if (recording_state == true)
            {
                if (timer_grabacion < SEGUNDOS * 20)
                { /* 5=1s (200ms) , *10(tim=100ms), *20(tim=100ms)	*/
                    timer_grabacion++;
                }
                else
                {
                    machine_state = RECORDING_DONE;
                    break;
                }
            }
            else
            {
                if (selection_pedal[12]->push->push_state == GUI_ON)
                {
                    machine_state = START_RECORDING;
                    break;
                }
            }

            block_machine = false;
            NVIC_EnableIRQ(TIMx_IRQn);
            NVIC_EnableIRQ((IRQn_Type)DMA2_Stream7_IRQn);
            break;
        }
#endif /* if SCREEN_ENABLE */

        case START_RECORDING:
        {
            NVIC_DisableIRQ((IRQn_Type)DMA2_Stream7_IRQn);
            NVIC_DisableIRQ(TIMx_IRQn);
            BSP_LED_On(LED1);
            recording_state = true;
            timer_grabacion = 0;
            recording_state = 44;
            block_machine = false;
            MenuForceRedraw();
            machine_state = NONE_STATE;
            NVIC_EnableIRQ(TIMx_IRQn);
            NVIC_EnableIRQ((IRQn_Type)DMA2_Stream7_IRQn);
            break;
        }

        case RECORDING_DONE:
        {
            NVIC_DisableIRQ((IRQn_Type)DMA2_Stream7_IRQn);
            NVIC_DisableIRQ(TIMx_IRQn);

            BSP_AUDIO_OUT_SetVolume(0);

            AUDIO_RECORD_BUFFER[4] = (((escritura_sd * SECTOR_SD_WRITE) - 44) + 36) & (0xFF);      // 0xF0;
            AUDIO_RECORD_BUFFER[5] = (((escritura_sd * SECTOR_SD_WRITE) - 44) & (0xFF << 8)) >> 8; // 0x04;
            AUDIO_RECORD_BUFFER[6] = (((escritura_sd * SECTOR_SD_WRITE) - 44) & (0xFF << 16)) >> 16;
            AUDIO_RECORD_BUFFER[7] = ((int32_t)(((int32_t)(escritura_sd * SECTOR_SD_WRITE) - 44) & ((int32_t)0xFF000000)) >> 24); // 0x00;
            AUDIO_RECORD_BUFFER[40] = (AUDIO_RECORD_BUFFER[4] - 36) & (0xFF);
            AUDIO_RECORD_BUFFER[41] = AUDIO_RECORD_BUFFER[5];
            AUDIO_RECORD_BUFFER[42] = AUDIO_RECORD_BUFFER[6];
            AUDIO_RECORD_BUFFER[43] = AUDIO_RECORD_BUFFER[7];

            char nombre_archivo[20];

            sprintf(nombre_archivo, "R%d.wav", cantidad_records);
            f_open(&wav_ptr, nombre_archivo, FA_CREATE_ALWAYS | FA_WRITE);

            for (uint32_t cont_i_bytes = 0; cont_i_bytes < escritura_sd; cont_i_bytes++)
            {
                f_write(&wav_ptr, &(AUDIO_RECORD_BUFFER[cont_i_bytes * 256]), 256, &aux_cont_bytes);
            }

            f_close(&wav_ptr);
            cantidad_records++;

            machine_state = NONE_STATE;

            recording_state = false;
            timer_grabacion = 0;
            recording_state = 0;
            BSP_LED_Off(LED1);

            block_machine = false;

            BSP_AUDIO_OUT_SetVolume(CODEC_OUT_VOLUME);

            selection_pedal[12]->push->push_state = GUI_OFF;
            MenuForceRedraw();

            NVIC_EnableIRQ(TIMx_IRQn);
            NVIC_EnableIRQ((IRQn_Type)DMA2_Stream7_IRQn);
            break;
        }

        default:
        {
            Error_Handler();
            break;
        }
        }
    }
}

void CPU_CACHE_Enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

void SystemClock_Config(void)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Set voltage scaling to optimize power consumption */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 8;

    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (ret != HAL_OK)
    {
        Error_Handler();
    }

    /* Activate the OverDrive */
    ret = HAL_PWREx_ActivateOverDrive();
    if (ret != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure clock dividers */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
    if (ret != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_Init_Config(void)
{
    /* Initialize the HAL library */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();
}

void Error_Handler(void)
{
    while (1)
    {
        /* You can use LED blinks, logging, or other methods to signal errors */
        BSP_LED_Toggle(LED1);
        HAL_Delay(100);
    }
}
