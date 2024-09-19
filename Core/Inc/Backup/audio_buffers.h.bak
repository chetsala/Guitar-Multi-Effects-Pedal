/*
 * audio_buffers.h
 *
 *  Created on: Sep 18, 2024
 *      Author: Chet Sala
 */

#ifndef INC_AUDIO_BUFFERS_H_
#define INC_AUDIO_BUFFERS_H_

#include <stdint.h>


#define BUFFER_SIZE 120

// declared the buffers as extern so that they can be used across multiple files
extern int16_t adcData[BUFFER_SIZE];
extern int16_t dacData[BUFFER_SIZE];
extern uint8_t dataReadyFlag;

// declare the buffer pointes as extern
extern volatile int16_t *inBufPtr;
extern volatile int16_t *outBufPtr;




#endif /* INC_AUDIO_BUFFERS_H_ */
