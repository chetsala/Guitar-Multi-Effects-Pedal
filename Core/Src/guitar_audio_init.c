/*
 * guitar_audio_init.c
 *
 *  Created on: Sep 18, 2024
 *      Author: Chet Sala
 */

#include "guitar_audio_init.h"
#include "wm8994.h"

#define WM8994_I2C_ADDRESS 0x34  // I2C address for WM8994 codec



// Use CubeMX-generated handles
extern SAI_HandleTypeDef hsai_BlockA2;   // Handle for SAI Block A
extern SAI_HandleTypeDef hsai_BlockB2;   // Handle for SAI Block B
extern DMA_HandleTypeDef hdma_sai2_a;    // DMA handle for SAI2_A
extern DMA_HandleTypeDef hdma_sai2_b;    // DMA handle for SAI2_B

// Function prototypes
void Audio_Init(void);
void WM8994_Init(void);
void Audio_Play(uint16_t* pBuffer, uint32_t Size);
void Audio_Stop(void);


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
	HAL_StatusTypeDef HAL_SAI_Transmit_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size);
	HAL_StatusTypeDef HAL_SAI_Receive_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size);

}


