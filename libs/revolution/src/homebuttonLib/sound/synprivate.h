#ifndef RVL_SDK_HBMSYN_PRIVATE_H
#define RVL_SDK_HBMSYN_PRIVATE_H

#include "syn.h"  // IWYU pragma: export

#include <revolution/ax.h>
#include <revolution/types.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef u32 EnvelopeState;
enum EnvelopeState_et {
    EnvStateAttack,
    EnvStateDecay,
    EnvStateSustain,
    EnvStateRelease,

    EnvStateEnd
};

typedef u32 VoiceType;
enum VoiceType_et {
    VoiceTypeNormal,
    VoiceTypeLooping,
};

// the typedef was declared in syn.h
struct HBMSYNVOICE {
    void* next;             // size 0x04, offset 0x00
    AXVPB* axvpb;           // size 0x04, offset 0x04
    HBMSYNSYNTH* synth;     // size 0x04, offset 0x08
    u8 midiChannel;         // size 0x01, offset 0x0c
    u8 keyNum;              // size 0x01, offset 0x0d
    u8 keyVel;              // size 0x01, offset 0x0e
    u8 pan;                 // size 0x01, offset 0x0f
    WTREGION* region;       // size 0x04, offset 0x10
    WTART* art;             // size 0x04, offset 0x14
    WTSAMPLE* sample;       // size 0x04, offset 0x18
    WTADPCM* adpcm;         // size 0x04, offset 0x1c
    VoiceType type;         // size 0x04, offset 0x20
    f32 srcRatio;           // size 0x04, offset 0x24
    s32 cents;              // size 0x04, offset 0x28
    s32 attn;               // size 0x04, offset 0x2c
    EnvelopeState veState;  // size 0x04, offset 0x30
    s32 veAttn;             // size 0x04, offset 0x34
    s32 veAttack;           // size 0x04, offset 0x38
    s32 veAttackDelta;      // size 0x04, offset 0x3c
    s32 veDecay;            // size 0x04, offset 0x40
    s32 veSustain;          // size 0x04, offset 0x44
    s32 veRelease;          // size 0x04, offset 0x48
};  // size 0x4c

// TODO: is the definition of HBMSYNVOICE visible at this point?

// syn.c
extern HBMSYNVOICE* __HBMSYNVoice;

// TODO: are these declared with their complete type or no?

// synctrl.c
extern f32 __HBMSYNn128[];

// synmix.c
extern s32 __HBMSYNVolumeAttenuation[];
extern s32 __HBMSYNAttackAttnTable[];

// synctrl.c
void __HBMSYNSetController(HBMSYNSYNTH* synth, u8 midiChannel, u8 function, u8 value);
void __HBMSYNResetAllControllers(HBMSYNSYNTH* synth);
void __HBMSYNRunInputBufferEvents(HBMSYNSYNTH* synth);
BOOL __HBMSYNGetWavetableData(HBMSYNVOICE* voice);

// synenv.c
s32 __HBMSYNGetEnvelopeTime(s32 scale, s32 mod, u8 key);
void __HBMSYNSetupVolumeEnvelope(HBMSYNVOICE* voice);
void __HBMSYNRunVolumeEnvelope(HBMSYNVOICE* voice);

// synmix.c
void __HBMSYNSetupVolume(HBMSYNVOICE* voice);
void __HBMSYNSetupPan(HBMSYNVOICE* voice);
s32 __HBMSYNGetVoiceInput(HBMSYNVOICE* voice);
s32 __HBMSYNGetVoiceFader(HBMSYNVOICE* voice);
void __HBMSYNUpdateMix(HBMSYNVOICE* voice);

// synpitch.c
f32 __HBMSYNGetRelativePitch(HBMSYNVOICE* voice);
void __HBMSYNSetupPitch(HBMSYNVOICE* voice);
void __HBMSYNSetupSrc(HBMSYNVOICE* voice);
void __HBMSYNUpdateSrc(HBMSYNVOICE* voice);

// synsample.c
void __HBMSYNSetupSample(HBMSYNVOICE* voice);

// synvoice.c
void __HBMSYNClearVoiceReferences(void* p);
void __HBMSYNSetVoiceToRelease(HBMSYNVOICE* voice);
void __HBMSYNServiceVoice(int i);

#ifdef __cplusplus
}
#endif

#endif  // RVL_SDK_HBMSYN_PRIVATE_H
