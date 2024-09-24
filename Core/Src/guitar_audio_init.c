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


#define BUFFER_SIZE 128



int16_t adcData[BUFFER_SIZE];
int16_t dacData[BUFFER_SIZE];

static volatile int16_t *inBufPtr;
static volatile int16_t *outBufPtr = &dacData[0];

uint8_t dataReadyFlag;

uint32_t Audio_Peripheral_Init()
{
	/* Initialize the audio in/out */
	BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_INPUT_LINE_1, OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_IN_FREQ,DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);

	Display_Init();
	Display_Draw_Text("Test", 50, 100);








	return 0;
}







void Signal_Processing(uint16_t *data_in, uint16_t *data_out, uint32_t count)
{
	// Template to use when writing effects
	uint16_t n;
	int16_t curr_sample;
	int16_t out_sample;

	for (n = count*AUDIO_BLOCK_SIZE/2; n < (count + 1)*AUDIO_BLOCK_SIZE/2;n++)
	{
		curr_sample = (int16_t) data_out[n];
		out_sample = curr_sample;

		data_out[n] = (uint16_t)out_sample;

	}
}


