#ifndef JASDSPINTERFACE_H
#define JASDSPINTERFACE_H

#include <types.h>

/**
 * Amount of separate audio channels (i.e. individual playbacks, voices) the DSP can mix at once.
 */
#define DSP_CHANNELS        64

/**
 * Amount of audio channels the DSP can calculate outputs for.
 */
#define DSP_OUTPUT_CHANNELS 6 // Presumed 5.1 surround

/**
 * Amount of audio samples rendered by the DSP in a single sub frame.
 */
#define DSP_SUBFRAME_SIZE   0x50

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
        u16 field_0x0;
        u16 field_0x2;
        s16* field_0x4;
        u16 field_0x8;
        s16 field_0xa;
        u16 field_0xc;
        s16 field_0xe;
        u16 field_0x10[8];
    } FxBuf;

    struct OutputChannelConfig {
        u16 mBusConnect;
        u16 mTargetVolume;
        u16 mCurrentVolume;

        /**
         * Gets upper 8 bits cleared when audio volume is changed mid-playback.
         * Presumed to be some kind of progress used by the DSP to calculate position between
         * mTargetVolume and mCurrentVolume.
         */
        u16 mVolumeProgress;
    };

    /**
     * DSP memory for each playback channel ("voice").
     * The DSP can read and write this memory. It is used as configuration, feedback,
     * and working memory.
     */
    struct TChannel {
        void init();
        void playStart();
        void playStop();
        void replyFinishRequest();
        void forceStop();
        bool isActive() const;

        /**
         * Check whether the DSP has finished playing this channel.
         */
        bool isFinish() const;
        void setWaveInfo(JASWaveInfo const&, u32, u32);
        void setOscInfo(u32);
        void initAutoMixer();
        void setAutoMixer(u16, u8, u8, u8, u8);
        void setPitch(u16);
        void setMixerInitVolume(u8 outputChannel, s16 volume);
        void setMixerVolume(u8 outputChannel, s16 volume);
        void setPauseFlag(u8);

        /**
         * Flushes backing memory of channel out of the data cache.
         */
        void flush();
        void initFilter();
        void setFilterMode(u16);
        void setIIRFilterParam(s16*);
        void setFIR8FilterParam(s16*);
        void setDistFilter(s16);
        void setBusConnect(u8 outputChannel, u8 param_1);

        /* 0x000 */ u16 mIsActive;
        /* 0x002 */ u16 mIsFinished;
        /* 0x004 */ u16 mPitch;
        /* 0x006 */ short _unused1;
        /* 0x008 */ u16 field_0x008;
        /* 0x00A */ u8 _unused2[0x00C - 0x00A];
        /* 0x00C */ s16 mPauseFlag;
        /* 0x00E */ short _unused3;
        /* 0x010 */ OutputChannelConfig mOutputChannels[DSP_OUTPUT_CHANNELS];
        /* 0x040 */ u8 _unused4[0x050 - 0x040];
        /* 0x050 */ u16 mAutoMixerPanDolby; // pan is upper 8 bits, dolby lower 8.
        /* 0x052 */ u16 mAutoMixerFxMix;
        /* 0x054 */ u16 mAutoMixerInitVolume;
        /* 0x056 */ u16 mAutoMixerVolume;
        /* 0x058 */ u16 mAutoMixerBeenSet;
        /* 0x05A */ u8 _unused5[0x060 - 0x05A];
        /* 0x060 */ short field_0x060; // Only cleared to zero, presumed used by DSP.
        /* 0x062 */ u8 _unused6[0x064 - 0x062];
        /* 0x064 */ u16 mSamplesPerBlock;
        /* 0x066 */ short field_0x066; // Only cleared to zero, presumed used by DSP.
        /* 0x068 */ int mSamplePosition; // Only ever initialized by code, name is guess.
        /* 0x06C */ u8 _unused7[0x070 - 0x06C];
        /* 0x070 */ int mAramStreamPosition; // Seems written by DSP, used for audio streaming.
        /* 0x074 */ int field_0x074;
        /* 0x078 */ short field_0x078[4];  // Only cleared to zero, presumed used by DSP.
        /* 0x080 */ short field_0x080[20]; // Only cleared to zero, presumed used by DSP.
        /* 0x0A8 */ short field_0x0a8[4];  // Only cleared to zero, presumed used by DSP.
        /* 0x0B0 */ u16 field_0x0b0[16];   // Only cleared to zero, presumed used by DSP.
        /* 0x0D0 */ u8 _unused8[0x100 - 0x0D0];
        /* 0x100 */ u16 mBytesPerBlock;
        /* 0x102 */ u16 field_0x102;

        /**
         * Used for decoding ADPCM data around loop edges.
         */
        /* 0x104 */ s16 mpLast;

        /**
         * Used for decoding ADPCM data around loop edges.
         */
        /* 0x106 */ s16 mpPenult;
        /* 0x108 */ u16 mFilterMode;
        /* 0x10A */ u16 mForcedStop;
        /* 0x10C */ int field_0x10c;
        /* 0x110 */ u32 mLoopStartSample;
        /* 0x114 */ u32 mEndSample;
        /* 0x118 */ u32 mWaveAramAddress;
        /* 0x11C */ int mSampleCount;
        /* 0x120 */ s16 fir_filter_params[8];
        /* 0x130 */ u8 _unused9[0x148 - 0x130];
        /* 0x148 */ s16 iir_filter_params[8];
        /* 0x158 */ u8 _unused10[0x180 - 0x158];
    };

    void boot(void (*)(void*));
    void releaseHalt(u32);
    void finishWork(u16);
    void syncFrame(u32, u32, u32);
    void setDSPMixerLevel(f32);
    f32 getDSPMixerLevel();
    TChannel* getDSPHandle(int);
    TChannel* getDSPHandleNc(int);
    void setFilterTable(s16*, s16*, u32);
    void flushBuffer();
    void invalChannelAll();
    void initBuffer();
    int setFXLine(u8, s16*, JASDsp::FxlineConfig_*);
    BOOL changeFXLineParam(u8, u8, uintptr_t);

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
