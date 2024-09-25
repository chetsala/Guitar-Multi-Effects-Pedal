/*
 * audio_processing.c
 *
 *  Created on: Sep 25, 2024
 *      Author: Chet
 */



#include "audio_processing.h"
#include "wm8994.h"
#include "stm32746g_discovery.h"
#include "stm32f7xx_hal_sai.c"


#define MY_BUFFER_SIZE_SAMPLES 1024
#define MY_DMA_BYTES_PER_FRAME 8
#define MY_DMA_BYTES_PER_MSIZE 2
#define MY_DMA_BUFFER_SIZE_BYTES MY_BUFFER_SIZE_SAMPLES * MY_DMA_BYTES_PER_FRAME
#define MY_DMA_BUFFER_SIZE_MSIZES MY_DMA_BUFFER_SIZE_BYTES / MY_DMA_BYTES_PER_MSIZE




void BSP_Bringup(void);



static void fill_buffer_with_square_wave(int16_t * buf, uint32_t num_samples);



static int16_t playbackBuffer[MY_BUFFER_SIZE_SAMPLES];
static uint8_t saiDMATransmitBuffer[MY_DMA_BUFFER_SIZE_BYTES];
static uint32_t frequency = AUDIO_FREQUENCY_48K;
static uint8_t volume = 100;


extern SAI_HandleTypeDef		haudio_out_sai;







void BSP_Bringup()
{

	fill_buffer_with_square_wave(playbackBuffer, MY_BUFFER_SIZE_SAMPLES);
	HAL_SAI_Transmit_DMA(&haudio_out_sai, (uint8_t*) saiDMATransmitBuffer, MY_DMA_BUFFER_SIZE_MSIZES);






//wm8994_Init(AUDIO_I2C_ADDRESS, uint16_t OutputInputDevice, uint8_t Volume, uint32_t AudioFreq);

//wm8994_Init(uint16_t DeviceAddr, uint16_t OutputInputDevice, uint8_t Volume, uint32_t AudioFreq);

}



static void fill_buffer_with_square_wave(int16_t * buf, uint32_t num_samples)
{
	// Fill up a 100Hz square wave
	//48khz sample rate --> 480 samples in 100 hz --> toggle every 240 samples
	int toggle_period = 240;
	int count = 0;
	int wave_state = 1;
	int magnitude = 30000;

	for(int i = 0; i < num_samples; i++){

		buf[i] = magnitude * wave_state;

		count++;
		if(count >= toggle_perioud){
			count = 0;
			wave_state = wave_state * (-1); //toggle
		}
	}
}


static void ConvertSampleBufferToDMABuffer(int16_t * sampleBuffer, uint8_t * dmaBuffer, uint32_t num_samples)
{
	for(uint32_t i=0; i<num_samples; i++){
		int16_t * p = (int16_t *) &dmaBuffer[i*8]; //samples are spaced 8 bytes apart
		*p = sampleBuffer[i];
		*(p+2) = sampleBuffer[i];
	}





}
