
/* Includes ------------------------------------------------------------------*/
#include "audio_init.h"
#include "main.h"
#include "wm8994.h"

/* Global variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Audio_Init(void);
void Audio_Processing(void);
static void fill_buffer_with_square_wave(int16_t *buf, uint32_t num_samples);
static void GenerateSineWave(uint16_t *buffer, size_t bufferSize, float frequency, float sampleRate);


/* Private typedef -----------------------------------------------------------*/

typedef enum
{
    BUFFER_OFFSET_NONE = 0,
    BUFFER_OFFSET_HALF = 1,
    BUFFER_OFFSET_FULL = 2,
} BUFFER_StateTypeDef;

/* Private variables ---------------------------------------------------------*/
static uint16_t bufferIn[AUDIO_BLOCK_SIZE];
static uint16_t bufferOut[AUDIO_BLOCK_SIZE];
static volatile uint16_t *inBufptr;
static volatile uint16_t *outBufptr;
BUFFER_StateTypeDef buffer_status;

void Audio_Init(void)
{
    BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_INPUT_LINE_1, OUTPUT_DEVICE_HEADPHONE, AUDIO_FREQ, BIT_RES, CHAN_NUMB);
    BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
    HAL_SAI_Receive_DMA(&haudio_in_sai, (uint8_t *)bufferIn, AUDIO_BLOCK_SIZE);
    HAL_SAI_Transmit_DMA(&haudio_out_sai, (uint8_t *)bufferOut, AUDIO_BLOCK_SIZE);
}

/*
 *   DSP Processing
 */

void Audio_Processing(void)
{
    for (uint32_t n = 0; n < AUDIO_BLOCK_SIZE; n++)
    {
        bufferIn[n] = bufferOut[n];
    }
}

void Audio_Stream(void)
{
    Audio_Processing();

    while (1)
    {
        if (buffer_status == BUFFER_OFFSET_HALF)
        {

            inBufptr = &bufferIn[0];
            outBufptr = &bufferOut[BUFFER_SIZE / 2];

            buffer_status = BUFFER_OFFSET_NONE;
        }
        else if (buffer_status == BUFFER_OFFSET_FULL)
        {

            inBufptr = &bufferIn[BUFFER_SIZE / 2];
            outBufptr = &bufferOut[0];

            buffer_status = BUFFER_OFFSET_NONE;
        }
    }
}

void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
    inBufptr = &bufferIn[BUFFER_SIZE / 2];
    outBufptr = &bufferOut[0];
}

void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
    inBufptr = &bufferIn[0];
    outBufptr = &bufferOut[BUFFER_SIZE / 2];
}

static void fill_buffer_with_square_wave(int16_t *buf, uint32_t num_samples)
{
  // Fill up a 100 Hz square wave
  // 48 kHz sample rate -> 480 samples in 100 Hz -> toggle every 240 samples
  int toggle_period = 240;
  int count = 0;
  int wave_state = 1;
  int magnitude = 30000;

  for (int i = 0; i < num_samples; i++)
  {
    // Set the buffer value based on the wave state
    buf[i] = magnitude * wave_state;

    count++;
    if (count >= toggle_period)
    {
      count = 0;
      wave_state = wave_state * (-1); // Toggle the wave state between 1 and -1
    }
  }
}

void GenerateSineWave(uint16_t *buffer, size_t bufferSize, float frequency, float sampleRate)
{
  // Variables to keep track of the sine wave phase
  float phaseIncrement = 2 * PI * frequency / sampleRate;
  float phase = 0.0f;

  for (size_t i = 0; i < bufferSize; i += 2)
  {
    // Generate sine wave value (scaled to amplitude)
    int16_t sampleValue = (int16_t)(AMPLITUDE * sin(phase));

    // Fill both left and right channels with the same sample
    buffer[i] = sampleValue;     // Left channel
    buffer[i + 1] = sampleValue; // Right channel

    // Update the phase
    phase += phaseIncrement;

    // Wrap around if phase exceeds 2π
    if (phase >= 2 * PI)
    {
      phase -= 2 * PI;
    }
  }
}