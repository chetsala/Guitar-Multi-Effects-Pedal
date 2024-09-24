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


uint16_t data_in[AUDIO_BUFFER_SIZE];
uint16_t data_out[AUDIO_BUFFER_SIZE];
uint16_t data_out_processed[2*AUDIO_BLOCK_SIZE];
uint8_t audio_rec_buffer_state;
uint32_t count = 0;
uint8_t second_counter = 0;
uint16_t m, n;


typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;




uint32_t Audio_Peripheral_Init()
{
	/* Initialize the audio in/out */
	BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_INPUT_LINE_1, OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_IN_FREQ,DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);



	/* Initialize SDRAM buffers */
	memset((uint16_t*)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE*2);
	memset((uint16_t*)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE*4);

	Display_Init();
	Display_Draw_Text("Test", 50, 100);

	/* Initialize input and output sample buffers */
	memset(data_in, 0, AUDIO_BUFFER_SIZE);
	memset(data_out, 0, AUDIO_BUFFER_SIZE);

	/* Initialize the data buffer fill state */
	audio_rec_buffer_state = BUFFER_OFFSET_NONE;


	/* Start recording */
	BSP_AUDIO_IN_Record((uint16_t*)AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE);

	/* Play samples in parallel */
	BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
	BSP_AUDIO_OUT_Play((uint16_t*)AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE*4);



	/* Sets the signal output volume based on an application parameter */
	uint8_t current_volume = 75;
	BSP_AUDIO_OUT_SetVolume(current_volume);

	return 0;
}

void Process_Audio(void)
{



		/* Waiting for half a block to be recorded */
		if(audio_rec_buffer_state == BUFFER_OFFSET_HALF)
		{
			audio_rec_buffer_state = BUFFER_OFFSET_NONE;
			/* Copy the recorded block */

			memcpy((uint16_t *)(data_in + (count*AUDIO_BLOCK_SIZE/2)), (uint16_t *)(AUDIO_BUFFER_IN), AUDIO_BLOCK_SIZE);
			memcpy((uint16_t *)(data_out + (count*AUDIO_BLOCK_SIZE/2)), (uint16_t *)(data_in + (count*AUDIO_BLOCK_SIZE/2)), AUDIO_BLOCK_SIZE);


			/* Processing signal samples with enabled effects */
			Signal_Processing(data_in, data_out, count);

			/* Duplicate samples in the buffer so that the sound is heard on both channels */
			m = 0;
			for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
			{
				data_out_processed[m] = data_out[n];
				data_out_processed[m+1] = data_out[n];
				m += 2;
			}

		/* Recreate the output signal */
		memcpy((uint16_t *)(AUDIO_BUFFER_OUT), (uint16_t *)(data_out_processed), AUDIO_BLOCK_SIZE*2);
		count += 1;
		}


		/* Wait until the end of block recording */

		if(audio_rec_buffer_state != BUFFER_OFFSET_FULL)
		{
			audio_rec_buffer_state = BUFFER_OFFSET_NONE;

			/* Copy the second recorded block */
			memcpy((uint16_t *)(data_in+(count*AUDIO_BLOCK_SIZE/2)), (uint16_t *)(AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE)), AUDIO_BLOCK_SIZE);
			memcpy((uint16_t *)(data_out + (count*AUDIO_BLOCK_SIZE/2)), (uint16_t *)(data_in+(count*AUDIO_BLOCK_SIZE/2)), AUDIO_BLOCK_SIZE);

			/* Processing signal samples with enabled effects */
			Signal_Processing(data_in, data_out, count);

			/* Duplicate samples in the buffer so that the sound is heard on both channels */
			m = 0;
			for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
		{
			data_out_processed[m] = data_out[n];
			data_out_processed[m+1] = data_out[n];
			m += 2;
		}

		/* Recreate the output signal */
		memcpy((uint16_t *)(AUDIO_BUFFER_OUT + (AUDIO_BLOCK_SIZE*2)),  (uint16_t *)(data_out_processed), AUDIO_BLOCK_SIZE*2);

		/* Update the counter of the currently processed sample block */
		if(count == 351)
		{
			count = 0;
		}
		else
		{
			count += 1;
		}
	}
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


