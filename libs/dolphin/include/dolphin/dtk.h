#ifndef _DOLPHIN_DTK_H_
#define _DOLPHIN_DTK_H_

#include <dolphin/dvd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*DTKCallback)(u32 eventMask);
typedef void (*DTKFlushCallback)(void);

typedef struct DTKTrack {
	/* 0x00 */ struct DTKTrack* prev;
    /* 0x04 */ struct DTKTrack* next;
    /* 0x08 */ char* fileName;
    /* 0x0C */ u32 eventMask;
    /* 0x10 */ DTKCallback callback;
    /* 0x14 */ DVDFileInfo dvdFileInfo;
} DTKTrack;

#define DTK_STATE_STOP    0
#define DTK_STATE_RUN     1
#define DTK_STATE_PAUSE   2
#define DTK_STATE_BUSY    3
#define DTK_STATE_PREPARE 4

#define DTK_MODE_NOREPEAT  0
#define DTK_MODE_ALLREPEAT 1
#define DTK_MODE_REPEAT1   2

void DTKInit(void);
void DTKShutdown(void);
u32 DTKQueueTrack(char* fileName, DTKTrack* track, u32 eventMask, DTKCallback callback);
u32 DTKRemoveTrack(DTKTrack* track);
int DTKFlushTracks(DTKFlushCallback callback);
void DTKSetSampleRate(u32 samplerate);
void DTKSetInterruptFrequency(u32 samples);
void DTKSetRepeatMode(u32 repeat);
int DTKSetState(u32 state);
int DTKNextTrack(void);
int DTKPrevTrack(void);
u32 DTKGetSampleRate(void);
u32 DTKGetRepeatMode(void);
u32 DTKGetState(void);
u32 DTKGetPosition(void);
u32 DTKGetInterruptFrequency(void);
DTKTrack* DTKGetCurrentTrack(void);
void DTKSetVolume(u8 left, u8 right);
u16 DTKGetVolume(void);

#ifdef __cplusplus
}
#endif

#endif
