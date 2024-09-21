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
#include <string.h>
#include <stdint.h>


//Type Def
typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;


/*
 * DEFINES
 */


#define AUDIO_NB_BLOCKS    			((uint32_t)4)



/* Private variables ---------------------------------------------------------*/



/* Global variables ---------------------------------------------------------*/
extern uint32_t  audio_rec_buffer_state;



/*------------------------------------------------------------------------------
                           Audio functions
------------------------------------------------------------------------------*/

void Audio_Init(void);



#endif /* INC_GUITAR_AUDIO_INIT_H_ */
