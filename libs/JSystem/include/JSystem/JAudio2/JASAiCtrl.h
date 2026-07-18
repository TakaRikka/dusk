#ifndef JASAICTRL_H
#define JASAICTRL_H

#include <types.h>

enum JASOutputRate {
    OUTPUT_RATE_0,
};

enum JASMixMode {
    MIX_MODE_MONO,
    MIX_MODE_MONO_WIDE,
    MIX_MODE_EXTRA,
    MIX_MODE_INTERLEAVE,
};

namespace JASDriver {
    typedef s16* (*MixCallback)(s32);
    typedef void (*MixFunc)(s16*, u32, MixCallback);
    typedef void (*DSPBufCallback)(s16*, u32);

    void initAI(void (*)(void));
    void startDMA();
    void stopDMA();
    void setOutputRate(JASOutputRate);
    void updateDac();
    void updateDSP();
    void readDspBuffer(s16*, u32);
    void finishDSPFrame();
    void registerMixCallback(MixCallback, JASMixMode);
    void registDSPBufCallback(DSPBufCallback);
    f32 getDacRate();
    u32 getSubFrames();
    u32 getDacSize();
    u32 getFrameSamples();
    void mixMonoTrack(s16*, u32, MixCallback);
    void mixMonoTrackWide(s16*, u32, MixCallback);
    void mixExtraTrack(s16*, u32, MixCallback);
    void mixInterleaveTrack(s16*, u32, MixCallback);
    u32 getSubFrameCounter();
    void subframeCallback();
    void DSPSyncCallback();

    DUSK_GAME_EXTERN const MixFunc sMixFuncs[4];
    DUSK_GAME_EXTERN s16* sDmaDacBuffer[3];
    DUSK_GAME_EXTERN JASMixMode sMixMode;
    DUSK_GAME_EXTERN f32 sDacRate;
    DUSK_GAME_EXTERN u32 sSubFrames;
    DUSK_GAME_EXTERN s16** sDspDacBuffer;
    DUSK_GAME_EXTERN s32 sDspDacWriteBuffer;
    DUSK_GAME_EXTERN s32 sDspDacReadBuffer;
    DUSK_GAME_EXTERN s32 sDspStatus;
    DUSK_GAME_EXTERN DSPBufCallback sDspDacCallback;
    DUSK_GAME_EXTERN s16* lastRspMadep;
    DUSK_GAME_EXTERN void (*dacCallbackFunc)(s16*, u32);
    DUSK_GAME_EXTERN MixCallback extMixCallback;
    DUSK_GAME_EXTERN u32 sOutputRate;
    DUSK_GAME_EXTERN u32 sSubFrameCounter;
};

#endif /* JASAICTRL_H */
