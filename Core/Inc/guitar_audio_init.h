/*
 * guitar_audio_init.h
 *
 *Includes only audio-specific headers, defines, and prototypes relevant to audio initialization and handling.
 *
 *
 *  Created on: Sep 18, 2024
 *      Author: Chet Sala
 *
 */

#ifndef INC_GUITAR_AUDIO_INIT_H_
#define INC_GUITAR_AUDIO_INIT_H_

/*
 * Includes
 */

#include "stm32746g_discovery_audio.h"
#include "stm32746g_discovery.h"
#include "string.h"
#include "stm32746g_discovery_sdram.h"
#include "rk043fn48h.h"
#include "wm8994.h"


/*
 * Defines
 */





#define AUDIO_BLOCK_SIZE   ((uint32_t)256)
#define AUDIO_BUFFER_SIZE ((uint32_t)45056)
#define AUDIO_REC_START_ADDR         SDRAM_WRITE_READ_ADDR
#define AUDIO_BUFFER_IN    AUDIO_REC_START_ADDR     /* In SDRAM */
#define AUDIO_BUFFER_OUT   (AUDIO_REC_START_ADDR + (AUDIO_BLOCK_SIZE * 2)) /* In SDRAM */




/*
 * Function Prototypes
 */

/**
 * @brief  Initializes audio peripherals including SAI, I2S, and codec.
 * @param  DeviceAddr:  This is the I2C address of the audio codec (WM8994).
 * #define AUDIO_I2C_ADDRESS                ((uint16_t)0x34)
 * @param  Volume: Volume level (0 - 100%).
 * @param  AudioFreq: Audio frequency (8000, 16000, 32000, 48000, etc.).
 * @retval 0 if success, otherwise error code.
 */
uint32_t Audio_Peripheral_Init(void);
void Process_Audio(void);
void Signal_Processing(uint16_t *data_in, uint16_t *data_out, uint32_t count);


#endif /* INC_GUITAR_AUDIO_INIT_H_ */
