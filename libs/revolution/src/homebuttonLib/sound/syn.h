#ifndef RVL_SDK_HBMSYN_H
#define RVL_SDK_HBMSYN_H

#include <revolution/types.h>

#define HBMSYN_NUM_MIDI_CHANNELS 16
#define HBMSYN_NUM_MIDI_NOTES 128

// Modifiable; to what extent is unknown
#define HBMSYN_INPUT_BUFFER_SIZE 256

#ifdef __cplusplus
extern "C" {
#endif

// context declarations
typedef struct HBMSYNVOICE HBMSYNVOICE;  // see synprivate.h

// forward declarations
typedef struct HBMSYNSYNTH HBMSYNSYNTH;

typedef struct WTADPCM {
    u16 a[8][2];          // size 0x10, offset 0x00
    u16 gain;             // size 0x02, offset 0x20
    u16 pred_scale;       // size 0x02, offset 0x22
    u16 yn1;              // size 0x02, offset 0x24
    u16 yn2;              // size 0x02, offset 0x26
    u16 loop_pred_scale;  // size 0x02, offset 0x28
    u16 loop_yn1;         // size 0x02, offset 0x2a
    u16 loop_yn2;         // size 0x02, offset 0x2c
} WTADPCM;                // size 0x2e

typedef struct WTART {
    s32 lfoFreq;        // size 0x04, offset 0x00
    s32 lfoDelay;       // size 0x04, offset 0x04
    s32 lfoAtten;       // size 0x04, offset 0x08
    s32 lfoPitch;       // size 0x04, offset 0x0c
    s32 lfoMod2Atten;   // size 0x04, offset 0x10
    s32 lfoMod2Pitch;   // size 0x04, offset 0x14
    s32 eg1Attack;      // size 0x04, offset 0x18
    s32 eg1Decay;       // size 0x04, offset 0x1c
    s32 eg1Sustain;     // size 0x04, offset 0x20
    s32 eg1Release;     // size 0x04, offset 0x24
    s32 eg1Vel2Attack;  // size 0x04, offset 0x28
    s32 eg1Key2Decay;   // size 0x04, offset 0x2c
    s32 eg2Attack;      // size 0x04, offset 0x30
    s32 eg2Decay;       // size 0x04, offset 0x34
    s32 eg2Sustain;     // size 0x04, offset 0x38
    s32 eg2Release;     // size 0x04, offset 0x3c
    s32 eg2Vel2Attack;  // size 0x04, offset 0x40
    s32 eg2Key2Decay;   // size 0x04, offset 0x44
    s32 eg2Pitch;       // size 0x04, offset 0x48
    s32 pan;            // size 0x04, offset 0x4c
} WTART;                // size 0x50

typedef struct WTINST {
    u16 keyRegion[HBMSYN_NUM_MIDI_NOTES];
} WTINST;  // size 0x100

typedef struct WTREGION {
    u8 unityNote;           // size 0x01, offset 0x00
    u8 keyGroup;            // size 0x01, offset 0x01
    s16 fineTune;           // size 0x02, offset 0x02
    s32 attn;               // size 0x04, offset 0x04
    u32 loopStart;          // size 0x04, offset 0x08
    u32 loopLength;         // size 0x04, offset 0x0c
    u32 articulationIndex;  // size 0x04, offset 0x10
    u32 sampleIndex;        // size 0x04, offset 0x14
} WTREGION;                 // size 0x18

typedef struct WTSAMPLE {
    u16 format;      // size 0x02, offset 0x00
    u16 sampleRate;  // size 0x02, offset 0x02
    u32 offset;      // size 0x04, offset 0x04
    u32 length;      // size 0x04, offset 0x08
    u16 adpcmIndex;  // size 0x02, offset 0x0c
                     /* 2 bytes padding */
} WTSAMPLE;          // size 0x10

struct HBMSYNSYNTH {
    HBMSYNSYNTH* next;                       // size 0x0004, offset 0x0000
    WTINST* percussiveInst;                  // size 0x0004, offset 0x0004
    WTINST* melodicInst;                     // size 0x0004, offset 0x0008
    WTREGION* region;                        // size 0x0004, offset 0x000c
    WTART* art;                              // size 0x0004, offset 0x0010
    WTSAMPLE* sample;                        // size 0x0004, offset 0x0014
    WTADPCM* adpcm;                          // size 0x0004, offset 0x0018
    u32 samplesBaseWord;                     // size 0x0004, offset 0x001c
    u32 samplesBaseByte;                     // size 0x0004, offset 0x0020
    u32 samplesBaseNibble;                   // size 0x0004, offset 0x0024
    WTINST* inst[HBMSYN_NUM_MIDI_CHANNELS];  // size 0x0040, offset 0x0028
    s32 masterVolume;                        // size 0x0004, offset 0x0068
    s32 volAttn[HBMSYN_NUM_MIDI_CHANNELS];   // size 0x0040, offset 0x006c
    s32 auxAAttn[HBMSYN_NUM_MIDI_CHANNELS];  // size 0x0040, offset 0x00ac
    u8 pan[HBMSYN_NUM_MIDI_CHANNELS];        // size 0x0010, offset 0x00ec
    u8 input[HBMSYN_INPUT_BUFFER_SIZE][3];   // size 0x0300, offset 0x00fc
    u8* inputPosition;                       // size 0x0004, offset 0x03fc
    u32 inputCounter;                        // size 0x0004, offset 0x0400
    u32 notes;                               // size 0x0004, offset 0x0404
    HBMSYNVOICE* voice[HBMSYN_NUM_MIDI_CHANNELS]
                      [HBMSYN_NUM_MIDI_NOTES];  // size 0x2000, offset 0x0408
};  // size 0x2408

void HBMSYNInit(void);
void HBMSYNQuit(void);

void HBMSYNRunAudioFrame(void);

void HBMSYNInitSynth(HBMSYNSYNTH* synth, u8* wavetable, u8* samples, u8* zerobuffer);
void HBMSYNQuitSynth(HBMSYNSYNTH* synth);

void HBMSYNMidiInput(HBMSYNSYNTH* synth, u8* input);

void HBMSYNSetMasterVolume(HBMSYNSYNTH* synth, s32 dB);
s32 HBMSYNGetMasterVolume(HBMSYNSYNTH* synth);

#ifdef __cplusplus
}
#endif

#endif  // RVL_SDK_HBMSYN_H
