/*
 * guitar_audio_init.h
 *
 *  Created on: Sep 18, 2024
 *      Author: Chet Sala
 */

#ifndef INC_GUITAR_AUDIO_INIT_H_
#define INC_GUITAR_AUDIO_INIT_H_

#include "wm8994.h"
#include "stm32746g_discovery_audio.h"
#include "stm32f7xx_hal_sai.h"



/*
 * DEFINES
 */


#define AUDIO_BLOCK_SIZE             ((uint32_t)512)
#define AUDIO_BLOCK_HALFSIZE         ((uint32_t)256)
#define AUDIO_I2C_ADDRESS			 ((uint16_t)0x34)
#define DEFAULT_VOL_70_PERCENT        0xB3


/*------------------------------------------------------------------------------
                           Audio functions
------------------------------------------------------------------------------*/

void My_Audio_Init(void);




#endif /* INC_GUITAR_AUDIO_INIT_H_ */
