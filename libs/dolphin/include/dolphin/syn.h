#ifndef _DOLPHIN_SYN_H_
#define _DOLPHIN_SYN_H_

#include <dolphin/types.h>
#include <dolphin/ax.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SYN_INPUT_BUFFER_SIZE 0x100

typedef struct WTINST {
    /* 0x00 */ u16 keyRegion[128];
} WTINST;

typedef struct WTREGION {
    /* 0x00 */ u8 unityNote;
    /* 0x01 */ u8 keyGroup;
    /* 0x02 */ s16 fineTune;
    /* 0x04 */ s32 attn;
    /* 0x08 */ u32 loopStart;
    /* 0x0C */ u32 loopLength;
    /* 0x10 */ u32 articulationIndex;
    /* 0x14 */ u32 sampleIndex;
} WTREGION;

typedef struct WTART {
    /* 0x00 */ s32 lfoFreq;
    /* 0x04 */ s32 lfoDelay;
    /* 0x08 */ s32 lfoAtten;
    /* 0x0C */ s32 lfoPitch;
    /* 0x10 */ s32 lfoMod2Atten;
    /* 0x14 */ s32 lfoMod2Pitch;
    /* 0x18 */ s32 eg1Attack;
    /* 0x1C */ s32 eg1Decay;
    /* 0x20 */ s32 eg1Sustain;
    /* 0x24 */ s32 eg1Release;
    /* 0x28 */ s32 eg1Vel2Attack;
    /* 0x2C */ s32 eg1Key2Decay;
    /* 0x30 */ s32 eg2Attack;
    /* 0x34 */ s32 eg2Decay;
    /* 0x38 */ s32 eg2Sustain;
    /* 0x3C */ s32 eg2Release;
    /* 0x40 */ s32 eg2Vel2Attack;
    /* 0x44 */ s32 eg2Key2Decay;
    /* 0x48 */ s32 eg2Pitch;
    /* 0x4C */ s32 pan;
} WTART;

typedef struct WTSAMPLE {
    /* 0x00 */ u16 format;
    /* 0x02 */ u16 sampleRate;
    /* 0x04 */ u32 offset;
    /* 0x08 */ u32 length;
    /* 0x0C */ u16 adpcmIndex;
} WTSAMPLE;

typedef struct WTADPCM {
    /* 0x00 */ u16 a[8][2];
    /* 0x20 */ u16 gain;
    /* 0x22 */ u16 pred_scale;
    /* 0x24 */ u16 yn1;
    /* 0x26 */ u16 yn2;
    /* 0x28 */ u16 loop_pred_scale;
    /* 0x2A */ u16 loop_yn1;
    /* 0x2C */ u16 loop_yn2;
} WTADPCM;

typedef struct SYNSYNTH {
    /* 0x0000 */ void* next;
    /* 0x0004 */ WTINST* percussiveInst;
    /* 0x0008 */ WTINST* melodicInst;
    /* 0x000C */ WTREGION* region;
    /* 0x0010 */ WTART* art;
    /* 0x0014 */ WTSAMPLE* sample;
    /* 0x0018 */ WTADPCM* adpcm;
    /* 0x001C */ u32 aramBaseWord;
    /* 0x0020 */ u32 aramBaseByte;
    /* 0x0024 */ u32 aramBaseNibble;
    /* 0x0028 */ u32 zeroBaseWord;
    /* 0x002C */ u32 zeroBaseByte;
    /* 0x0030 */ u32 zeroBaseNibble;
    /* 0x0034 */ u32 priorityVoiceAlloc;
    /* 0x0038 */ u32 priorityNoteOn;
    /* 0x003C */ u32 priorityNoteRelease;
    /* 0x0040 */ WTINST* inst[16];
    /* 0x0080 */ s32 masterVolume;
    /* 0x0084 */ u8 controller[16][128];
    /* 0x0844 */ u8 rpn[16];
    /* 0x0894 */ s16 dataEntry[16];
    /* 0x08B4 */ s32 pwMaxCents[16];
    /* 0x08F4 */ s32 pwCents[16];
    /* 0x0934 */ s32 volAttn[16];
    /* 0x0974 */ s32 expAttn[16];
    /* 0x09B4 */ s32 auxAAttn[16];
    /* 0x09F4 */ s32 auxBAttn[16];
    /* 0x0A34 */ u8 input[SYN_INPUT_BUFFER_SIZE][3];
    /* 0x0D34 */ u8* inputPosition;
    /* 0x0D38 */ u32 inputCounter;
    /* 0x0D3C */ u32 notes;
    /* 0x0D40 */ void* keyGroup[16][16];
    /* 0x1140 */ void* voice[16][128];
} SYNSYNTH;

typedef struct SYNVOICE {
    /* 0x00 */ void* next;
    /* 0x04 */ AXVPB* axvpb;
    /* 0x08 */ SYNSYNTH* synth;
    /* 0x0C */ u8 midiChannel;
    /* 0x0D */ u8 keyNum;
    /* 0x0E */ u8 keyVel;
    /* 0x0F */ u8 pan;
    /* 0x10 */ u8 keyGroup;
    /* 0x14 */ WTREGION* region;
    /* 0x18 */ WTART* art;
    /* 0x1C */ WTSAMPLE* sample;
    /* 0x20 */ WTADPCM* adpcm;
    /* 0x24 */ u32 hold;
    /* 0x28 */ u32 type;
    /* 0x2C */ f32 srcRatio;
    /* 0x30 */ s32 cents;
    /* 0x34 */ s32 attn;
    /* 0x38 */ s32 lfoState;
    /* 0x3C */ s32 lfoAttn;
    /* 0x40 */ s32 lfoCents;
    /* 0x44 */ s32 lfoFreq;
    /* 0x48 */ s32 lfoDelay;
    /* 0x4C */ s32 lfoAttn_;
    /* 0x50 */ s32 lfoCents_;
    /* 0x54 */ s32 lfoModAttn;
    /* 0x58 */ s32 lfoModCents;
    /* 0x5C */ u32 veState;
    /* 0x60 */ s32 veAttn;
    /* 0x64 */ s32 veAttack;
    /* 0x68 */ s32 veAttackDelta;
    /* 0x6C */ s32 veDecay;
    /* 0x70 */ s32 veSustain;
    /* 0x74 */ s32 veRelease;
    /* 0x78 */ u32 peState;
    /* 0x7C */ s32 peCents;
    /* 0x80 */ s32 peAttack;
    /* 0x84 */ s32 peDecay;
    /* 0x88 */ s32 peSustain;
    /* 0x8C */ s32 peRelease;
    /* 0x90 */ s32 pePitch;
} SYNVOICE;

// sample formats
#define SYN_SAMPLE_FORMAT_ADPCM 0
#define SYN_SAMPLE_FORMAT_PCM16 1
#define SYN_SAMPLE_FORMAT_PCM8  2

// SYN
void SYNInit(void);
void SYNQuit(void);
void SYNRunAudioFrame(void);
void SYNInitSynth(SYNSYNTH* synth, void* wavetable, u32 aramBase, u32 zeroBase, u32 priorityVoiceAlloc, u32 priorityNoteOn, u32 priorityNoteRelease);
void SYNQuitSynth(SYNSYNTH* synth);
void SYNMidiInput(SYNSYNTH* synth, u8* input);
void SYNSetMasterVolume(SYNSYNTH* synth, s32 dB);
s32 SYNGetMasterVolume(SYNSYNTH* synth);
u32 SYNGetActiveNotes(SYNSYNTH* synth);

// SYNCTRL
u8 SYNGetMidiController(SYNSYNTH* synth, u8 midiChannel, u8 function);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_SYN_H_
