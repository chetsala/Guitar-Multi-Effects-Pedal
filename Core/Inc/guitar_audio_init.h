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
#include <stdio.h>
#include "main.h"


/*
 * DEFINES
 */


#define AUDIO_BLOCK_SIZE             ((uint32_t)512)
#define AUDIO_BLOCK_HALFSIZE         ((uint32_t)256)
#define AUDIO_I2C_ADDRESS			 ((uint16_t)0x34)
#define DEFAULT_VOL_70_PERCENT        0xB3

// Sizes of the data block, sample buffer, and buffer address
#define AUDIO_BUFFER_SIZE ((uint32_t)45056)
#define AUDIO_BUFFER_IN    AUDIO_REC_START_ADDR     /* In SDRAM */
#define AUDIO_BUFFER_OUT   (AUDIO_REC_START_ADDR + (AUDIO_BLOCK_SIZE * 2)) /* In SDRAM */
#define AUDIO_REC_START_ADDR         SDRAM_WRITE_READ_ADDR

#define INFO_HEIGHT 20
#define INFO_XPOS 0
#define INFO_YPOS 0
#define INFO_WIDTH 480

/*
 * Variables
 */

extern char current_info_text[40];


/*------------------------------------------------------------------------------
                           Audio functions
------------------------------------------------------------------------------*/

void My_Audio_Init(void);




#endif /* INC_GUITAR_AUDIO_INIT_H_ */
