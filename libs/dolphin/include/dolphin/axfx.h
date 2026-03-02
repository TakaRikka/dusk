#ifndef _DOLPHIN_AXFX_H_
#define _DOLPHIN_AXFX_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_REVSTD_DELAYLINE {
    /* 0x00 */ s32 inPoint;
    /* 0x04 */ s32 outPoint;
    /* 0x08 */ s32 length;
    /* 0x0C */ f32* inputs;
    /* 0x10 */ f32 lastOutput;
} AXFX_REVSTD_DELAYLINE;

typedef struct AXFX_REVSTD_WORK {
    /* 0x000 */ AXFX_REVSTD_DELAYLINE AP[6];
    /* 0x078 */ AXFX_REVSTD_DELAYLINE C[6];
    /* 0x0F0 */ f32 allPassCoeff;
    /* 0x0F4 */ f32 combCoef[6];
    /* 0x10C */ f32 lpLastout[3];
    /* 0x118 */ f32 level;
    /* 0x11C */ f32 damping;
    /* 0x120 */ s32 preDelayTime;
    /* 0x124 */ f32* preDelayLine[3];
    /* 0x130 */ f32* preDelayPtr[3];
} AXFX_REVSTD_WORK;

typedef struct AXFX_REVERBSTD {
    /* 0x000 */ AXFX_REVSTD_WORK rv;
    /* 0x13C */ u8 tempDisableFX;
    /* 0x140 */ f32 coloration;
    /* 0x144 */ f32 mix;
    /* 0x148 */ f32 time;
    /* 0x14C */ f32 damping;
    /* 0x150 */ f32 preDelay;
} AXFX_REVERBSTD;

typedef struct AXFX_BUFFERUPDATE {
    /* 0x00 */ s32* left;
    /* 0x04 */ s32* right;
    /* 0x08 */ s32* surround;
} AXFX_BUFFERUPDATE;

typedef struct AXFX_BUFFERUPDATE_DPL2 {
    /* 0x00 */ s32* L;
    /* 0x04 */ s32* R;
    /* 0x08 */ s32* Ls;
    /* 0x0C */ s32* Rs;
} AXFX_BUFFERUPDATE_DPL2;

// REVHI Structs

typedef struct AXFX_REVHI_DELAYLINE {
    /* 0x00 */ s32 inPoint;
    /* 0x04 */ s32 outPoint;
    /* 0x08 */ s32 length;
    /* 0x0C */ f32* inputs;
    /* 0x10 */ f32 lastOutput;
} AXFX_REVHI_DELAYLINE;

typedef struct AXFX_REVHI_WORK {
    /* 0x000 */ AXFX_REVHI_DELAYLINE AP[9];
    /* 0x0B4 */ AXFX_REVHI_DELAYLINE C[9];
    /* 0x168 */ f32 allPassCoeff;
    /* 0x16C */ f32 combCoef[9];
    /* 0x190 */ f32 lpLastout[3];
    /* 0x19C */ f32 level;
    /* 0x1A0 */ f32 damping;
    /* 0x1A4 */ s32 preDelayTime;
    /* 0x1A8 */ f32 crosstalk;
    /* 0x1AC */ f32* preDelayLine[3];
    /* 0x1B8 */ f32* preDelayPtr[3];
} AXFX_REVHI_WORK;

typedef struct AXFX_REVHI_WORK_DPL2 {
    /* 0x000 */ AXFX_REVHI_DELAYLINE AP[12];
    /* 0x0F0 */ AXFX_REVHI_DELAYLINE C[12];
    /* 0x1E0 */ f32 allPassCoeff;
    /* 0x1E4 */ f32 combCoef[12];
    /* 0x214 */ f32 lpLastout[4];
    /* 0x224 */ f32 level;
    /* 0x228 */ f32 damping;
    /* 0x22C */ s32 preDelayTime;
    /* 0x230 */ f32 crosstalk;
    /* 0x234 */ f32* preDelayLine[4];
    /* 0x244 */ f32* preDelayPtr[4];
} AXFX_REVHI_WORK_DPL2;

typedef struct AXFX_REVERBHI {
    /* 0x000 */ AXFX_REVHI_WORK rv;
    /* 0x1C4 */ u8 tempDisableFX;
    /* 0x1C8 */ f32 coloration;
    /* 0x1CC */ f32 mix;
    /* 0x1D0 */ f32 time;
    /* 0x1D4 */ f32 damping;
    /* 0x1D8 */ f32 preDelay;
    /* 0x1DC */ f32 crosstalk;
} AXFX_REVERBHI;

typedef struct AXFX_REVERBHI_DPL2 {
    /* 0x000 */ AXFX_REVHI_WORK_DPL2 rv;
    /* 0x254 */ u8 tempDisableFX;
    /* 0x258 */ f32 coloration;
    /* 0x25C */ f32 mix;
    /* 0x260 */ f32 time;
    /* 0x264 */ f32 damping;
    /* 0x268 */ f32 preDelay;
} AXFX_REVERBHI_DPL2;

typedef struct AXFX_DELAY {
    /* 0x00 */ u32 currentSize[3];
    /* 0x0C */ u32 currentPos[3];
    /* 0x18 */ u32 currentFeedback[3];
    /* 0x24 */ u32 currentOutput[3];
    /* 0x30 */ s32* left;
    /* 0x34 */ s32* right;
    /* 0x38 */ s32* sur;
    /* 0x3C */ u32 delay[3];
    /* 0x48 */ u32 feedback[3];
    /* 0x54 */ u32 output[3];
} AXFX_DELAY;

typedef struct AXFX_CHORUS_SRCINFO {
    /* 0x00 */ s32* dest;
    /* 0x04 */ s32* smpBase;
    /* 0x08 */ s32* old;
    /* 0x0C */ u32 posLo;
    /* 0x10 */ u32 posHi;
    /* 0x14 */ u32 pitchLo;
    /* 0x18 */ u32 pitchHi;
    /* 0x1C */ u32 trigger;
    /* 0x20 */ u32 target;
} AXFX_CHORUS_SRCINFO;

typedef struct AXFX_CHORUS_WORK {
    /* 0x00 */ s32* lastLeft[3];
    /* 0x0C */ s32* lastRight[3];
    /* 0x18 */ s32* lastSur[3];
    /* 0x24 */ u8 currentLast;
    /* 0x28 */ s32 oldLeft[4];
    /* 0x38 */ s32 oldRight[4];
    /* 0x48 */ s32 oldSur[4];
    /* 0x58 */ u32 currentPosLo;
    /* 0x5C */ u32 currentPosHi;
    /* 0x60 */ s32 pitchOffset;
    /* 0x64 */ u32 pitchOffsetPeriodCount;
    /* 0x68 */ u32 pitchOffsetPeriod;
    /* 0x6C */ AXFX_CHORUS_SRCINFO src;
} AXFX_CHORUS_WORK;

typedef struct AXFX_CHORUS {
    /* 0x00 */ AXFX_CHORUS_WORK work;
    /* 0x90 */ u32 baseDelay;
    /* 0x94 */ u32 variation;
    /* 0x98 */ u32 period;
} AXFX_CHORUS;

// chorus
int AXFXChorusInit(AXFX_CHORUS* c);
int AXFXChorusShutdown(AXFX_CHORUS* c);
int AXFXChorusSettings(AXFX_CHORUS* c);
void AXFXChorusCallback(AXFX_BUFFERUPDATE* bufferUpdate, AXFX_CHORUS* chorus);

// delay
void AXFXDelayCallback(AXFX_BUFFERUPDATE* bufferUpdate, AXFX_DELAY* delay);
int AXFXDelaySettings(AXFX_DELAY* delay);
int AXFXDelayInit(AXFX_DELAY* delay);
int AXFXDelayShutdown(AXFX_DELAY* delay); 

// reverb_hi
void DoCrossTalk(s32* l, s32* r, f32 cross, f32 invcross);
int AXFXReverbHiInit(AXFX_REVERBHI* rev);
int AXFXReverbHiShutdown(AXFX_REVERBHI* rev);
int AXFXReverbHiSettings(AXFX_REVERBHI* rev);
void AXFXReverbHiCallback(AXFX_BUFFERUPDATE* bufferUpdate, AXFX_REVERBHI* reverb);

// reverb_hi_4ch
int AXFXReverbHiInitDpl2(AXFX_REVERBHI_DPL2* reverb);
int AXFXReverbHiShutdownDpl2(AXFX_REVERBHI_DPL2* reverb);
int AXFXReverbHiSettingsDpl2(AXFX_REVERBHI_DPL2* rev);
void AXFXReverbHiCallbackDpl2(AXFX_BUFFERUPDATE_DPL2* bufferUpdate, AXFX_REVERBHI_DPL2* reverb);

// reverb_std
int AXFXReverbStdInit(AXFX_REVERBSTD* rev);
int AXFXReverbStdShutdown(AXFX_REVERBSTD* rev);
int AXFXReverbStdSettings(AXFX_REVERBSTD* rev);
void AXFXReverbStdCallback(AXFX_BUFFERUPDATE* bufferUpdate, AXFX_REVERBSTD* reverb);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_AXFX_H_
