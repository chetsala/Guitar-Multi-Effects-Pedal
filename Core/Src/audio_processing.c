/*
 * audio_processing.c
 *
 *  Created on: Sep 25, 2024
 *      Author: Chet
 */



#include "audio_processing.h"
#include "wm8994.h"
#include "stm32746g_discovery.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32746g_discovery_audio.h"
#include "display_init.h"
#include <stdio.h>


#define MY_BUFFER_SIZE_SAMPLES 1024   // 1024
#define MY_DMA_BYTES_PER_FRAME 8
#define MY_DMA_BYTES_PER_MSIZE 2
#define MY_DMA_BUFFER_SIZE_BYTES MY_BUFFER_SIZE_SAMPLES * MY_DMA_BYTES_PER_FRAME
#define MY_DMA_BUFFER_SIZE_MSIZES MY_DMA_BUFFER_SIZE_BYTES / MY_DMA_BYTES_PER_MSIZE

#define RGB565_BYTE_PER_PIXEL     2
#define ARBG8888_BYTE_PER_PIXEL   4

#define CAMERA_RES_MAX_X          640
#define CAMERA_RES_MAX_Y          480



#define AUDIO_BLOCK_SIZE   ((uint32_t)256)
#define AUDIO_BUFFER_SIZE ((uint32_t)45056)
#define AUDIO_BUFFER_IN    AUDIO_REC_START_ADDR     /* In SDRAM */
#define AUDIO_BUFFER_OUT   (AUDIO_REC_START_ADDR + (AUDIO_BLOCK_SIZE * 2)) /* In SDRAM */
#define LCD_FRAME_BUFFER          SDRAM_DEVICE_ADDR
#define CAMERA_FRAME_BUFFER       ((uint32_t)(LCD_FRAME_BUFFER + (RK043FN48H_WIDTH * RK043FN48H_HEIGHT * ARBG8888_BYTE_PER_PIXEL)))

#define SDRAM_WRITE_READ_ADDR        ((uint32_t)(CAMERA_FRAME_BUFFER + (CAMERA_RES_MAX_X * CAMERA_RES_MAX_Y * RGB565_BYTE_PER_PIXEL)))

#define SDRAM_WRITE_READ_ADDR_OFFSET ((uint32_t)0x0800)
#define SRAM_WRITE_READ_ADDR_OFFSET  SDRAM_WRITE_READ_ADDR_OFFSET

#define AUDIO_REC_START_ADDR         SDRAM_WRITE_READ_ADDR



typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;



static void fill_buffer_with_square_wave(int16_t * buf, uint32_t num_samples);







static int16_t playbackBuffer[MY_BUFFER_SIZE_SAMPLES];
static uint8_t saiDMATransmitBuffer[MY_DMA_BUFFER_SIZE_BYTES];
static uint32_t frequency = AUDIO_FREQUENCY_48K;
static uint8_t volume = 100;






static void fill_buffer_with_square_wave(int16_t * buf, uint32_t num_samples)
{
	// Fill up a 100Hz square wave
	//48khz sample rate --> 480 samples in 100 hz --> toggle every 240 samples
	int toggle_period = 480;
	int count = 0;
	int wave_state = 1;
	int magnitude = 33000;

	for(int i = 0; i < num_samples; i++){

		buf[i] = magnitude * wave_state;

		count++;
		if(count >= toggle_period){
			count = 0;
			wave_state = wave_state * (-1); //toggle
		}
	}
}


void Multieffect(void)
	{
		/* Codec initialization, SAI */
	  if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, volume, I2S_AUDIOFREQ_48K) == AUDIO_OK)
	  {
		  Display_Draw_Text("Codec Initialized", 0, 0);
	  }
	  else
	  {
		  Display_Draw_Text("Codec Initialization Failed", 0, 0);
	  }
	  fill_buffer_with_square_wave(playbackBuffer, MY_BUFFER_SIZE_SAMPLES);
	  BSP_AUDIO_OUT_SetAudioFrameSlot(SAI_SLOTACTIVE_0);
	  BSP_AUDIO_OUT_Play(&playbackBuffer, AUDIO_BLOCK_SIZE * 2);


//	 		 while (1)
//	 	  {
//	 	    while(audio_rec_buffer_state != BUFFER_OFFSET_HALF)
//	 	    {
//	 				HAL_Delay(1);
//	 	    }
//	 	    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
//
//	 	    memcpy((uint16_t *)(AUDIO_BUFFER_OUT),
//	 	           (uint16_t *)(AUDIO_BUFFER_IN),
//	 	           AUDIO_BLOCK_SIZE);
//
//	 	    while(audio_rec_buffer_state != BUFFER_OFFSET_FULL)
//	 	    {
//	 				HAL_Delay(1);
//	 	    }
//	 	    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
//	 	    memcpy((uint16_t *)(AUDIO_BUFFER_OUT + (AUDIO_BLOCK_SIZE)),
//	 	           (uint16_t *)(AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE)),
//	 	           AUDIO_BLOCK_SIZE);

	/*   Initialization of SDRAM buffers
	  memset((uint16_t*)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE*2);
	  memset((uint16_t*)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE*2);
	  audio_rec_buffer_state = BUFFER_OFFSET_NONE;

	   Start recording
	  BSP_AUDIO_IN_Record((uint16_t*)AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE);

	   Reconstruction
	  BSP_AUDIO_OUT_SetAudioFrameSlot(SAI_SLOTACTIVE_0);
	  BSP_AUDIO_OUT_Play((uint16_t*)AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE * 2);

		 while (1)
	  {
	     Wait for half of the block to be recorded
	    while(audio_rec_buffer_state != BUFFER_OFFSET_HALF)
	    {
				HAL_Delay(1);
	    }
	    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
	     Copy the recorded block
	    memcpy((uint16_t *)(AUDIO_BUFFER_OUT),
	           (uint16_t *)(AUDIO_BUFFER_IN),
	           AUDIO_BLOCK_SIZE);

	     Wait until the end of the block recording
	    while(audio_rec_buffer_state != BUFFER_OFFSET_FULL)
	    {
				HAL_Delay(1);
	    }
	    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
	     Copy the 2nd recorded block
	    memcpy((uint16_t *)(AUDIO_BUFFER_OUT + (AUDIO_BLOCK_SIZE)),
	           (uint16_t *)(AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE)),
	           AUDIO_BLOCK_SIZE);

	  }*/
	}



