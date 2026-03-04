#ifndef _DOLPHIN_SYN_INTERNAL_H_
#define _DOLPHIN_SYN_INTERNAL_H_

#include <dolphin/syn.h>

#ifdef __cplusplus
extern "C" {
#endif

// syn
extern SYNSYNTH* __SYNSynthList;

// synctrl
extern f32 __SYNn128[128];

void __SYNClearAllNotes(SYNSYNTH* synth);
void __SYNSetController(SYNSYNTH* synth, u8 midiChannel, u8 function, u8 value);
void __SYNResetController0(SYNSYNTH* synth, u8 midiChannel);
void __SYNResetController(SYNSYNTH* synth, u8 midiChannel);
void __SYNResetAllControllers(SYNSYNTH* synth);
void __SYNRunInputBufferEvents(SYNSYNTH* synth);

// synenv
s32 __SYNGetEnvelopeTime(s32 scale, s32 mod, u8 key);
void __SYNSetupVolumeEnvelope(SYNVOICE* voice);
void __SYNSetupPitchEnvelope(SYNVOICE* voice);
void __SYNRunVolumeEnvelope(SYNVOICE* voice);
void __SYNRunPitchEnvelope(SYNVOICE* voice);

// synlfo
void __SYNSetupLfo(SYNVOICE* voice);
void __SYNRunLfo(SYNVOICE* voice);

// synmix
extern s32 __SYNVolumeAttenuation[128];
extern s32 __SYNAttackAttnTable[100];

void __SYNSetupVolume(SYNVOICE* voice);
void __SYNSetupPan(SYNVOICE* voice);
s32 __SYNGetVoiceInput(SYNVOICE* voice);
s32 __SYNGetVoiceFader(SYNVOICE* voice);
void __SYNUpdateMix(SYNVOICE* voice);

// synpitch
f32 __SYNGetRelativePitch(SYNVOICE* voice);
void __SYNSetupPitch(SYNVOICE* voice);
void __SYNSetupSrc(SYNVOICE* voice);
void __SYNUpdateSrc(SYNVOICE* voice);

// synsample
void __SYNSetupSample(SYNVOICE* voice);

// synvoice
extern SYNVOICE __SYNVoice[64];

void __SYNClearVoiceReferences(void* p);
void __SYNSetVoiceToRelease(SYNVOICE* voice, u32 priority);
void __SYNServiceVoice(int i);

// synwt
int __SYNGetWavetableData(SYNVOICE* voice);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_SYN_INTERNAL_H_
