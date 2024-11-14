#include "gui.h"

#include "arm_math.h"
#include "arm_common_tables.h"
#include "stm32746g_discovery_audio.h"

#define SQUARE     0
#define TRIANGULAR 1
#define RAMP_UP    2
#define RAMP_DOWN  3
#define SINUSOIDAL 4
#define ENVELOPE   5

#define SR         DEFAULT_AUDIO_IN_FREQ

//Pedales Individuales
void initAppGUI(void);
void InitEffects(void);    //void InitEfectos(void);

// Delay.c
void delayInit(void);
void delayParameters(void); //void delayParametros(void);

int32_t delayEffect(int32_t); //int32_t delayEfecto(int32_t);
void delayTime(GUIElement *); // void delayTime(GUIElement *);
void delayFeedback(GUIElement *); // void delayFeedback(GUIElement *);
void delayLevel(GUIElement *); // void delayLevel(GUIElement *);

// Autowah.c
void autowahInit(void);
void autowahParameters(void); //void autowahParametros(void);

int32_t autowahEffect(int32_t); //int32_t autowahEfecto(int32_t);
int32_t autowahBP2(float32_t, int32_t);
float32_t autowahLFO(int32_t);
void autowahRate(GUIElement *);
void autowahDepth(GUIElement *);
void autowahVolume(GUIElement *);
void autowahMod(GUIElement *);

// Distortion.c
void distortionInit(void); //void distorsionInit(void);
void distortionParameters(void); //void distorsionParametros(void);

int32_t distortionEffect(int32_t);
int32_t distortionShelve(int32_t, int32_t);
void distortionGain(GUIElement *);
void distortionBlend(GUIElement *);

/*
int32_t distorsionEfecto(int32_t);
int32_t distorsionShelve(int32_t, int32_t);
void distorsionGain(GUIElement *);
void distorsionBlend(GUIElement *);
*/



// Vibrato.c
void vibratoInit(void);
void vibratoParameters(void); //void vibratoParametros(void);

int32_t vibratoEffect(int32_t); //int32_t vibratoEfecto(int32_t);
float32_t vibratoLFO(int32_t);
void vibratoRate(GUIElement *);
void vibratoDepth(GUIElement *);
void vibratoMod(GUIElement *);


// Tremolo.c
void tremoloInit(void);
void tremoloParameters(void); // void tremoloParametros(void);

int tremoloEffect(int32_t); //int tremoloEfecto(int32_t);
float32_t tremoloLFO(void);
void tremoloRate(GUIElement *);
void tremoloDepth(GUIElement *);
void tremoloMod(GUIElement *);

// Ringmod.c
void ringmodInit(void);
void ringmodParameters(void); //void ringmodParametros(void);

int32_t ringmodEffect(int32_t); // int32_t ringmodEfecto(int32_t);
void ringmodRate(GUIElement *);

//Chorus.c
void chorusInit(void);
void chorusParameters(void); // void chorusParametros(void);

int32_t chorusEffect(int32_t); //int32_t chorusEfecto(int32_t);
float32_t chorusLFO(int32_t);
void chorusRate(GUIElement *e);
void chorusDepth(GUIElement *e);

//Flanger.c
void flangerInit(void);
void flangerParameters(void); // void flangerParametros(void);

int flangerEffect(int32_t); // int flangerEfecto(int32_t);
float32_t flangerLFO(int32_t);
void flangerManual(GUIElement *e);
void flangerDepth(GUIElement *e);
void flangerRate(GUIElement *e);
void flangerFeedback(GUIElement *e);

//Octavador.c
void octavadorInit(void);
void octaverParameters(void); // void octavadorParametros(void);
int octaverEffect(int); // int octavadorEfecto(int);

//Reverb
void reverbInit(void);
void reverbParameters(void); // void reverbParametros(void);
int32_t reverbEffect(int32_t input); // int32_t reverbEfecto(int32_t entrada);
int32_t reverbAP(int32_t in, int32_t *line, int32_t delay, int32_t cont);
int32_t reverbFBCF(int32_t in, int32_t line[], int32_t delay, int32_t cont, int32_t inst);
void reverbMix(GUIElement *e);
void reverbDecay(GUIElement *e);

// Phaser.c
void phaserInit(void);
void phaserParameters(void); // void phaserParametros(void);

int32_t phaserEffect(int32_t); // int32_t phaserEfecto(int32_t);
int32_t phaserAP1(int32_t, int32_t);
float32_t phaserLFO(int32_t);
void phaserRate(GUIElement *);

// Ecualizador.c
void eqInit(void);
void eqParameters(void);  //void eqParametros(void);

int32_t eqEffect(int32_t); // int32_t eqEfecto(int32_t);
int32_t eqPeak(int32_t, int32_t);
int32_t eqShelve(int32_t, int32_t);
void eqGain0(GUIElement *);
void eqGain1(GUIElement *);
void eqGain2(GUIElement *);
void eqGain3(GUIElement *);
void eqGain4(GUIElement *);
void eqGain5(GUIElement *);

//wav_recorder.c
typedef struct                /* Caso Generico */
{
    uint8_t ChunkID[4];       /*  Contains the letters "RIFF" in ASCII form */
    uint8_t ChunkSize[4];     /* 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size) o (36 + NumSamples * NumChannels * BitsPerSample/8)*/
    uint8_t Format[4];        /* Contains the letters "WAVE" */
    uint8_t Subchunk1ID[4];   /*  Contains the letters "fmt " */
    uint8_t Subchunk1Size[4]; /* 16 for PCM.  This is the size of the rest of the Subchunk which follows this number. */
    uint8_t AudioFormat[2];   /* PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some  form of compression. */
    uint8_t NumChannels[2];   /*  Mono = 1, Stereo = 2, etc. */
    uint8_t SampleRate[4];    /* 8000, 44100, etc. */
    uint8_t ByteRate[4];      /* == SampleRate * NumChannels * BitsPerSample/8 */
    uint8_t BlockAlign[2];    /* == NumChannels * BitsPerSample/8 */
    uint8_t BitsPerSample[2]; /* 8 bits = 8, 16 bits = 16, etc. */
    uint8_t ExtraParamSize[2]; /* if PCM, then doesn't exist */
    uint8_t Subchunk2ID[4];   /* Contains the letters "data" */
    uint8_t Subchunk2Size[4]; /* == NumSamples * NumChannels * BitsPerSample/8
                               *                                                                                                       This is the number of bytes in the data.
                               *                                                                                                       You can also think of this as the size
                               *                                                                                                       of the read of the subchunk following this
                               *                                                                                                       number. */
}wav_header;

void init_wav_header(uint8_t *buffer);
int record_wav(int input); // int grabar_wav(int enetrada);

//void init_wav_header (wav_header * wav_hdr_ptr);
