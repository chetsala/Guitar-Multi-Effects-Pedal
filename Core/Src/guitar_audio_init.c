/*
 *@file           : guitar_audio_init.c
 * @brief        : Manages all audio-related setups: SAI initialization, audio buffer management, audio codec setup.
 *
 *  Created on: Sep 18, 2024
 *      Author: Chet Sala
 *
 */

#include "guitar_audio_init.h"
#include "main.h"
#include <stdio.h>
#include "display_init.h"
#include "stm32f7xx_hal_sai.h"
#include <stdio.h>


#define BUFFER_SIZE 128




uint32_t  audio_rec_buffer_state;



/*
uint32_t Audio_Peripheral_Init()
{

	 Initialize the audio in/out
	BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_INPUT_LINE_1, OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_IN_FREQ,DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);

	Display_Init();

	 Initialize SDRAM buffers

	memset((uint16_t*)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE*2);
	memset((uint16_t*)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE*2);
	audio_rec_buffer_state = BUFFER_OFFSET_NONE;

	 Start audio input
	BSP_AUDIO_IN_Record((uint16_t*)AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE);

	 Restore
	BSP_AUDIO_OUT_SetAudioFrameSlot(SAI_SLOTACTIVE_0);
	BSP_AUDIO_OUT_Play((uint16_t*)AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE * 2);

	uint32_t ret = 0;  // To capture return values from functions

	    printf("Starting Audio Peripheral Initialization...\n");

	     Initialize the audio in/out
	    ret = BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_INPUT_LINE_1, OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
	    if (ret != AUDIO_OK)
	    {
	        printf("Error: BSP_AUDIO_IN_OUT_Init failed with return value: %d\n", ret);
	        return ret;
	    }
	    else
	    {
	        printf("BSP_AUDIO_IN_OUT_Init succeeded.\n");
	    }


	     Initialize SDRAM buffers
	    printf("Initializing SDRAM buffers...\n");
	    memset((uint16_t*)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE * 2);
	    memset((uint16_t*)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE * 2);
	    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
	    printf("SDRAM buffers initialized.\n");

	     Start audio input
	    printf("Starting audio input...\n");
	    ret = BSP_AUDIO_IN_Record((uint16_t*)AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE);
	    if (ret != AUDIO_OK)
	    {
	        printf("Error: BSP_AUDIO_IN_Record failed with return value: %d\n", ret);
	        return ret;
	    }
	    else
	    {
	        printf("BSP_AUDIO_IN_Record succeeded.\n");
	    }

	     Restore audio frame slot
	    printf("Setting audio frame slot...\n");

	    if (BSP_AUDIO_OUT_SetAudioFrameSlot(SAI_SLOTACTIVE_0) != AUDIO_OK)
	    {
	        printf("Error: BSP_AUDIO_OUT_SetAudioFrameSlot failed with return value: %d\n", ret);
	        return ret;
	    }
	    else
	    {
	        printf("BSP_AUDIO_OUT_SetAudioFrameSlot succeeded.\n");
	    }

	     Start audio output
	    printf("Starting audio output...\n");
	    ret = BSP_AUDIO_OUT_Play((uint16_t*)AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE * 2);
	    if (ret != AUDIO_OK)
	    {
	        printf("Error: BSP_AUDIO_OUT_Play failed with return value: %d\n", ret);
	        return ret;
	    }
	    else
	    {
	        printf("BSP_AUDIO_OUT_Play succeeded.\n");
	    }

	    printf("Audio Peripheral Initialization completed successfully.\n");

	    return ret;

}
*/








void Signal_Processing(uint16_t *data_in, uint16_t *data_out, uint32_t count)
{
    uint16_t n;
    int16_t curr_sample;

    for (n = 0; n < count; n++)
    {
        curr_sample = data_in[n];  // Read input sample
        // Apply any effects or processing here
        data_out[n] = curr_sample; // Write processed sample to output
    }
}


