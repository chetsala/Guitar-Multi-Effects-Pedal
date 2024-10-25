#ifndef INC_AUDIO_INIT_H_
#define INC_AUDIO_INIT_H_


/*
 * Defines
 */


#define BUFFER_SIZE 8192
#define BIT_RES     16
#define CHAN_NUMB   2
#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE)
#define AUDIO_NB_BLOCKS    ((uint32_t)4)

#define SAMPLE_RATE 48000 // Sample rate in Hz
#define FREQUENCY 440     // Frequency of the sine wave in Hz (A4 note)
#define AMPLITUDE 16000   // Amplitude of the sine wave (half of 16-bit max value)
#define PI 3.14159265359  // Pi value for sine calculations


#define AUDIO_BUFFER_OUT   (AUDIO_REC_START_ADDR + (AUDIO_BLOCK_SIZE * 2)) /* In SDRAM */


/*
 * Includes
 */

#include "wm8994.h"
#include "main.h"




/*
 * Function Prototypes
 */
void Audio_Init(void);
void Audio_Processing(void);
static void fill_buffer_with_square_wave(int16_t *buf, uint32_t num_samples);
static void GenerateSineWave(uint16_t *buffer, size_t bufferSize, float frequency, float sampleRate);








#endif /* INC_AUDIO_INIT_H_ */


