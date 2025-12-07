#ifndef JASDSPINTERFACE_H
#define JASDSPINTERFACE_H

#include "dolphin/types.h"

struct JASWaveInfo;

namespace JASDsp {
    struct FxlineConfig_ {
        u8 field_0x0;
        u8 field_0x1;
        u16 field_0x2;
        s16 field_0x4;
        u16 field_0x6;
        s16 field_0x8;
        u16 field_0xa;
        u32 field_0xc;
        s16 field_0x10[8];
    };

    typedef struct {
        s16 field_0x0;
        s16 field_0x2;
        s16* field_0x4;
        s16 field_0x8;
        s16 field_0xa;
        s16 field_0xc;
        s16 field_0xe;
        s16 field_0x10[8];
    } FxBuf;

    struct TChannel {
        void init();
        void playStart();
        void playStop();
        void replyFinishRequest();
        void forceStop();
        bool isActive() const;
        bool isFinish() const;
        void setWaveInfo(JASWaveInfo const&, u32, u32);
        void setOscInfo(u32);
        void initAutoMixer();
        void setAutoMixer(u16, u8, u8, u8, u8);
        void setPitch(u16);
        void setMixerInitVolume(u8, s16);
        void setMixerVolume(u8, s16);
        void setPauseFlag(u8);
        void flush();
        void initFilter();
        void setFilterMode(u16);
        void setIIRFilterParam(s16*);
        void setFIR8FilterParam(s16*);
        void setDistFilter(s16);
        void setBusConnect(u8, u8);

        /* 0x000 */ u16 mIsActive;
        /* 0x002 */ u16 mIsFinished;
        /* 0x004 */ u16 mPitch;
        /* 0x006 */ short field_0x006;
        /* 0x008 */ u16 field_0x008;
        /* 0x00A */ u8 field_0x00A[0x00C - 0x00A];
        /* 0x00C */ s16 mPauseFlag;
        /* 0x00E */ short field_0x00E;
        /* 0x010 */ u16 field_0x010[1][4]; // array size unknown
        /* 0x018 */ u8 field_0x018[0x050 - 0x018];
        /* 0x050 */ short field_0x050;
        /* 0x052 */ u16 field_0x052;
        /* 0x054 */ u16 field_0x054;
        /* 0x056 */ u16 field_0x056;
        /* 0x058 */ u16 field_0x058;
        /* 0x05A */ u8 field_0x05A[0x060 - 0x05A];
        /* 0x060 */ short field_0x060;
        /* 0x062 */ u8 field_0x062[0x064 - 0x062];
        /* 0x064 */ u16 field_0x064;
        /* 0x066 */ short field_0x066;
        /* 0x068 */ int field_0x068;
        /* 0x06C */ u8 field_0x06C[0x070 - 0x06C];
        /* 0x070 */ int field_0x070;
        /* 0x074 */ int field_0x074;
        /* 0x078 */ short field_0x078[4];
        /* 0x080 */ short field_0x080[20];
        /* 0x0A8 */ short field_0x0a8[4];
        /* 0x0B0 */ u16 field_0x0b0[16];
        /* 0x0D0 */ u8 field_0x0D0[0x100 - 0x0D0];
        /* 0x100 */ u16 field_0x100;
        /* 0x102 */ u16 field_0x102;
        /* 0x104 */ s16 field_0x104;
        /* 0x106 */ s16 field_0x106;
        /* 0x108 */ u16 mFilterMode;
        /* 0x10A */ u16 mForcedStop;
        /* 0x10C */ int field_0x10c;
        /* 0x110 */ u32 field_0x110;
        /* 0x114 */ u32 field_0x114;
        /* 0x118 */ u32 field_0x118;
        /* 0x11C */ int field_0x11c;
        /* 0x120 */ s16 fir_filter_params[8];
        /* 0x130 */ u8 field_0x130[0x148 - 0x130];
        /* 0x148 */ s16 iir_filter_params[8];
        /* 0x158 */ u8 field_0x158[0x180 - 0x158];
    };

    void boot(void (*)(void*));
    void releaseHalt(u32);
    void finishWork(u16);
    void syncFrame(u32, u32, u32);
    void setDSPMixerLevel(f32);
    f32 getDSPMixerLevel();
    TChannel* getDSPHandle(int);
    void setFilterTable(s16*, s16*, u32);
    void flushBuffer();
    void invalChannelAll();
    void initBuffer();
    int setFXLine(u8, s16*, JASDsp::FxlineConfig_*);

    extern u8 const DSPADPCM_FILTER[64];
    extern u32 const DSPRES_FILTER[320];
    extern u16 SEND_TABLE[];
    extern TChannel* CH_BUF;
    extern FxBuf* FX_BUF;
    extern f32 sDSPVolume;

    #if DEBUG
    extern s32 dspMutex;
    #endif
};

u16 DSP_CreateMap2(u32 msg);

#endif /* JASDSPINTERFACE_H */
