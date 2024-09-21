/*
 * guitar_audio_init.c
 *
 *  Created on: Sep 18, 2024
 *      Author: Chet Sala
 */

#include "guitar_audio_init.h"
#include "audio_buffers.h"
#include "stm32746g_discovery_lcd.h"
#include "main.h"


//PV
static uint16_t  internal_buffer[AUDIO_BLOCK_SIZE];
uint32_t  block_number;
uint32_t  audio_rec_buffer_state;


// Passthrough implementation

void Audio_Init(void) {

	  /* Initialize Audio Input */
	  if (BSP_AUDIO_IN_Init(DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) == AUDIO_OK)
	  {
	    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT OK  ", CENTER_MODE);
	  }
	  else
	  {
	    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	    BSP_LCD_SetTextColor(LCD_COLOR_RED);
	    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT FAIL", CENTER_MODE);
	    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
	  }

	  audio_rec_buffer_state = BUFFER_OFFSET_NONE;


	  /* Display the state on the screen */
	  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"       Guitar Input...     ", CENTER_MODE);

	  while(1){

	  /* Start Guitar Input */
	    BSP_AUDIO_IN_Record(internal_buffer, AUDIO_BLOCK_SIZE);

	    for (block_number = 0; block_number < AUDIO_NB_BLOCKS; block_number++)
	    {
	      /* Wait end of half block audio processed */
	      while(audio_rec_buffer_state != BUFFER_OFFSET_HALF)
	      {
	          return;
	        }
	      }
	      audio_rec_buffer_state = BUFFER_OFFSET_NONE;
	      /* Copy recorded 1st half block in SDRAM */
	      memcpy((uint32_t *)(AUDIO_REC_START_ADDR + (block_number * AUDIO_BLOCK_SIZE * 2)),
	             internal_buffer,
	             AUDIO_BLOCK_SIZE);


	      while (audio_rec_buffer_state != BUFFER_OFFSET_FULL)
	      	  {

	          	  return;

	          }

	      // Reset buffer state again after processing the full block
	      audio_rec_buffer_state = BUFFER_OFFSET_NONE;
         /* Copy recorded 2nd half block to SDRAM or output */
          memcpy((uint32_t *)(AUDIO_REC_START_ADDR + (block_number * AUDIO_BLOCK_SIZE * 2) + AUDIO_BLOCK_SIZE),
               internal_buffer + AUDIO_BLOCK_SIZE / 2,
	                        AUDIO_BLOCK_SIZE);

	    }
	  }

