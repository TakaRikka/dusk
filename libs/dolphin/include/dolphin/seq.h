#ifndef _DOLPHIN_SEQ_H_
#define _DOLPHIN_SEQ_H_

#include <dolphin/syn.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _SEQTRACK {
    /* 0x00 */ void* sequence;
    /* 0x04 */ u8* start;
    /* 0x08 */ u8* end;
    /* 0x0C */ u8* current;
    /* 0x10 */ u8 status;
    /* 0x14 */ f32 beatsPerSec;
    /* 0x18 */ u32 defaultTicksPerFrame;
    /* 0x1C */ u32 ticksPerFrame;
    /* 0x20 */ u32 delay;
    /* 0x24 */ u32 state;
} SEQTRACK;

typedef struct _SEQSEQUENCE {
    /* 0x0000 */ void* next;
    /* 0x0004 */ u32 state;
    /* 0x0008 */ u16 nTracks;
    /* 0x000A */ s16 timeFormat;
    /* 0x000C */ u32 tracksRunning;
    /* 0x0010 */ u32 end;
    /* 0x0014 */ SYNSYNTH synth;
    /* 0x3154 */ void (*callback[128])(void*, u8);
    /* 0x3354 */ SEQTRACK track[64];
} SEQSEQUENCE;

#define SEQ_ALL_TRACKS -1

void SEQInit(void);
void SEQQuit(void);
void SEQRunAudioFrame(void);
void SEQAddSequence(SEQSEQUENCE* sequence, u8* midiStream, void* wt, u32 aramBase, u32 zeroBase, u32 priorityVoiceAlloc, u32 priorityNoteOn, u32 priorityNoteRelease);
void SEQRemoveSequence(SEQSEQUENCE* sequence);
void SEQRegisterControllerCallback(SEQSEQUENCE* sequence, u8 controller, void (*callback)(void*, u8));
void SEQSetState(SEQSEQUENCE* sequence, u32 state);
u32 SEQGetState(SEQSEQUENCE* sequence);
void SEQSetTempo(SEQSEQUENCE* sequence, u32 trackIndex, f32 bpm);
f32 SEQGetTempo(SEQSEQUENCE* sequence, u32 trackIndex);
void SEQSetVolume(SEQSEQUENCE* sequence, s32 dB);
s32 SEQGetVolume(SEQSEQUENCE* sequence);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_SEQ_H_
