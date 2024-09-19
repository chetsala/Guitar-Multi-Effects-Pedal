/*
 * guitar_audio_init.c
 *
 *  Created on: Sep 18, 2024
 *      Author: Chet Sala
 */

#include "guitar_audio_init.h"
#include "wm8994.h"
#include "audio_buffers.h"


#define WM8994_I2C_ADDRESS 0x34  // I2C address for WM8994 codec

#define INT16_TO_FLOAT(x)  ((float)(x) / 32768.0f)
#define FLOAT_TO_INT16(x)  ((x) >= 1.0f ? 32767 : ((x) <= -1.0f ? -32768 : (int16_t)((x) * 32768.0f)))



// Use CubeMX-generated handles
extern SAI_HandleTypeDef hsai_BlockA2;   // Handle for SAI Block A
extern SAI_HandleTypeDef hsai_BlockB2;   // Handle for SAI Block B
extern DMA_HandleTypeDef hdma_sai2_a;    // DMA handle for SAI2_A
extern DMA_HandleTypeDef hdma_sai2_b;    // DMA handle for SAI2_B

// Function prototypes
void Audio_Init(void);
void WM8994_Init(void);
void Audio_Play(void);
void Audio_Stop(void);
void HAL_SAI_TxRxHalfCpltCallback(SAI_HandleTypeDef *hsai);
void HAL_SAI_TxRxCpltCallback(SAI_HandleTypeDef *hsai);
void processData();

/*
 * HAL_StatusTypeDef HAL_SAI_Transmit_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SAI_Receive_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size);
 */


void Audio_Init(void)
{

/*
 * sai2 a -- receive
sai2 b -- transmit
 */

	wm8994_Init(AUDIO_I2C_ADDRESS, INPUT_DEVICE_INPUT_LINE_1 || OUTPUT_DEVICE_HEADPHONE, DEFAULT_VOL_70_PERCENT, AUDIO_FREQUENCY_48K);


}


void Audio_Play(void)
{




}

void HAL_SAI_TxRxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
	inBufPtr = &adcData[0];
	outBufPtr = &dacData[0];

	dataReadyFlag = 1;
}

void HAL_SAI_TxRxCpltCallback(SAI_HandleTypeDef *hsai)
{
	inBufPtr = &adcData[BUFFER_SIZE/2];
	outBufPtr = &dacData[BUFFER_SIZE/2];

	dataReadyFlag = 1;

}

void processData(){ // fn gets called whenever dataReadyFlag = 1

	static float leftIn, leftOut;
	static float rightIn, rightOut;

	for (uint8_t n = 0; n < (BUFFER_SIZE/2) - 1; n+=2){

		/*
		 * Left Channel
		 */

		/* Get ADC Input and convert to float */
		leftIn = INT16_TO_FLOAT(inBufPtr[n]);
		if (leftIn > 1.0f){
			leftIn -= 2.0f;
		}

		/* Compute new output sample */

		leftOut = leftIn;

		/* Convert back to signed int and set DAC output */

		outBufPtr[n] = (int16_t) (FLOAT_TO_INT16(leftOut));

		/*
		 * Right Channel
		 */

			/* Get ADC Input and convert to float */
			rightIn = INT16_TO_FLOAT(inBufPtr[n+1]);
			if (rightIn > 1.0f){
				rightIn -= 2.0f;
			}

			/* Compute new output sample */

			rightOut = rightIn;

			/* Convert back to signed int and set DAC output */

			outBufPtr[n+1] = (int16_t) (FLOAT_TO_INT16(rightOut));



	}

}







