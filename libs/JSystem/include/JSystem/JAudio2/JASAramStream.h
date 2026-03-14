#ifndef JASARAMSTREAM_H
#define JASARAMSTREAM_H

#include "JSystem/JAudio2/JASTaskThread.h"
#include "JSystem/JUtility/JUTAssert.h"
#include <dvd.h>
#include "dusk/endian.h"

class JASChannel;

namespace JASDsp {
    struct TChannel;
}

#define STREAM_FORMAT_ADPCM4 0
#define STREAM_FORMAT_PCM16  1

/**
 * @ingroup jsystem-jaudio
 * Plays streamed music from DVD .ast files.
 */
class JASAramStream {
public:
    static const int CHANNEL_MAX = 6;

    typedef void (*StreamCallback)(u32, JASAramStream*, void*);

    enum CallbackType {
        /* 0 */ CB_START,
        /* 1 */ CB_STOP,
    };

    // Used internally for passing data to task functions
    struct TaskData {
        /* 0x0 */ JASAramStream* stream;
        /* 0x4 */ u32 param0;
        /* 0x8 */ int param1;
    };

    struct Header {
        /* 0x00 */ BE(u32) tag; // 'STRM'
#if TARGET_PC
        /* 0x04 */ BE(u32) soundBlockSize;
        /* 0x08 */ BE(u16) format;
        /* 0x0A */ BE(u16) bits;
#else
        /* 0x04 */ u8 field_0x4[5];
        /* 0x09 */ u8 format;
        /* 0x0A */ u8 bits;
#endif
        /* 0x0C */ BE(u16) channels;
        /* 0x0E */ BE(u16) loop;
        /* 0x10 */ BE(int) mSampleRate;
        /* 0x14 */ BE(u32) mSampleCount; // unused
        /* 0x18 */ BE(int) loop_start;
        /* 0x1C */ BE(int) loop_end;
        /* 0x20 */ BE(u32) block_size;
        /* 0x24 */ u8 _unused2[4];
        /* 0x28 */ u8 mVolume;
        /* 0x29 */ u8 _unused3[0x17];
    };  // Size: 0x40

    struct BlockHeader {
        /* 0x00 */ BE(u32) tag; // 'BLCK'
        /* 0x04 */ BE(u32) mSize;
        /* 0x08 */ struct {
            BE(s16) mpLast;
            BE(s16) mpPenult;
        } mAdpcmContinuationData[CHANNEL_MAX];
    };  // Size: 0x20

    static void initSystem(u32, u32);
    JASAramStream();
    void init(u32, u32, StreamCallback, void*);
    bool prepare(s32, int);
    bool start();
    bool stop(u16);
    bool pause(bool);
    bool cancel();

    /**
     * Calculate the amount of (decoded) audio samples in a single block of streamed audio.
     */
    u32 getBlockSamples() const;
    static void headerLoadTask(void*);
    static void firstLoadTask(void*);
    static void loadToAramTask(void*);
    static void finishTask(void*);
    static void prepareFinishTask(void*);
    bool headerLoad(u32, int);
    bool load();
    static s32 channelProcCallback(void*);
    static s32 dvdErrorCheck(void*);
    static void channelCallback(u32, JASChannel*, JASDsp::TChannel*, void*);
    void updateChannel(u32, JASChannel*, JASDsp::TChannel*);
    s32 channelProc();
    void channelStart();
    void channelStop(u16);

    void setPitch(f32 pitch) { mPitch = pitch; }
    void setVolume(f32 volume) { 
        for (int i = 0; i < 6; i++) {
            mChannelVolume[i] = volume; 
        }
    }

    void setPan(f32 pan) { 
        for (int i = 0; i < 6; i++) {
            mChannelPan[i] = pan; 
        }
    }

    void setFxmix(f32 fxMix) { 
        for (int i = 0; i < 6; i++) {
            mChannelFxMix[i] = fxMix; 
        }
    }

    void setDolby(f32 dolby) { 
        for (int i = 0; i < 6; i++) {
            mChannelDolby[i] = dolby; 
        }
    }

    void setChannelVolume(int channel, f32 volume) {
        JUT_ASSERT(290, channel < CHANNEL_MAX);
        mChannelVolume[channel] = volume;
    }

    void setChannelPan(int channel, f32 pan) {
        JUT_ASSERT(296, channel < CHANNEL_MAX);
        mChannelPan[channel] = pan;
    }

    void setChannelFxmix(int channel, f32 fxMix) {
        JUT_ASSERT(302, channel < CHANNEL_MAX);
        mChannelFxMix[channel] = fxMix;
    }

    void setChannelDolby(int channel, f32 dolby) {
        JUT_ASSERT(308, channel < CHANNEL_MAX);
        mChannelDolby[channel] = dolby;
    }

    static u32 getBlockSize() { return sBlockSize; }

    /**
     * Queue used to send specific commands that will be processed on the audio thread.
     * These commands are sent from the main thread.
     */
    /* 0x000 */ OSMessageQueue mMainCommandQueue;

    /**
     * Queue used to send specific commands that will be processed on the audio thread.
     * These commands are sent from the load (DVD) thread.
     */
    /* 0x020 */ OSMessageQueue mLoadCommandQueue;

    /**
     * Backing message storage for mMainCommandQueue.
     */
    /* 0x040 */ void* mMainCommandQueueArray[16];

    /**
     * Backing message storage for mLoadCommandQueue.
     */
    /* 0x080 */ void* mLoadCommandQueueArray[4];
    /* 0x090 */ JASChannel* mChannels[CHANNEL_MAX];

    /**
     * The first audio channel initialized among mChannels.
     * Used for the majority of bookkeeping, other channels replicate its state.
     */
    /* 0x0A8 */ JASChannel* mPrimaryChannel;

    /**
     * If true, stream has finished preparing (reading headers and initial blocks),
     * and is ready to play.
     */
    /* 0x0AC */ bool mPrepareFinished;
    /* 0x0AD */ bool mLoopEndLoaded;

    /**
     * Bitflag containing pause reasons/state for the stream.
     */
    /* 0x0AE */ u8 mPauseFlags;
    /* 0x0B0 */ int field_0x0b0;

    /**
     * (adjusted) value of mSamplesLeft on the primary channel last subframe.
     * Used to calculate how many samples have been read and determine when the DSP looped.
     */
    /* 0x0B4 */ int mLastSamplesLeft;

    /**
     * How many (decoded) samples the DSP has read so far.
     */
    /* 0x0B8 */ u32 mReadSample;
    /* 0x0BC */ int field_0x0bc;

    /**
     * If true, the current end (of loop, or just finish) is very close.
     * Loop start/end positions are modified while this is set to account for this.
     */
    /* 0x0C0 */ bool mEndSetup;
    /* 0x0C4 */ volatile u32 field_0x0c4;
    /* 0x0C8 */ volatile f32 field_0x0c8;
    /* 0x0CC */ DVDFileInfo mDvdFileInfo;
    /* 0x108 */ u32 mRingEndIndex;

    /**
     * Index into the ARAM ring buffer that is currently being loaded.
     * Wrapped around when incremented.
     */
    /* 0x10C */ int mBlockRingIndex;

    /**
     * Block currently being loaded.
     */
    /* 0x110 */ u32 mBlock;
    /* 0x114 */ u8 mIsCancelled;
    /* 0x118 */ u32 mPendingLoadTasks;
    /* 0x11C */ int mUpdateSamplesLeft;
    /* 0x120 */ int mUpdateLoopStartSample;
    /* 0x124 */ int mUpdateEndSample;
    /* 0x128 */ u16 mUpdateLoopFlag;

    /**
     * Bitflags updated in the play callback to track what data needs to be synchronized
     * between all channels.
     */
    /* 0x12C */ int mChannelUpdateFlags;
    /* 0x130 */ s16 mpLasts[CHANNEL_MAX];
    /* 0x13C */ s16 mpPenults[CHANNEL_MAX];
    /* 0x148 */ int mAramAddress;
    /* 0x14C */ u32 mAramSize;
    /* 0x150 */ StreamCallback mCallback;
    /* 0x154 */ void* mCallbackData;
    /* 0x158 */ u16 mFormat;
    /* 0x15A */ u16 mChannelNum;
    /* 0x15C */ u32 mBufCount;
    /* 0x160 */ u32 mAramBlocksPerChannel;
    /* 0x164 */ u32 mSampleRate;
    /* 0x168 */ bool mLoop;
    /* 0x16C */ u32 mLoopStart;
    /* 0x170 */ u32 mLoopEnd;
    /* 0x174 */ f32 mVolume;
    /* 0x178 */ f32 mPitch;
    /* 0x17C */ f32 mChannelVolume[CHANNEL_MAX];
    /* 0x194 */ f32 mChannelPan[CHANNEL_MAX];
    /* 0x1AC */ f32 mChannelFxMix[CHANNEL_MAX];
    /* 0x1C4 */ f32 mChannelDolby[CHANNEL_MAX];
    /* 0x1DC */ u16 mMixConfig[CHANNEL_MAX];

    /**
     * Thread that will be sent DVD load commands.
     * This is the JASDvd thread in practice.
     */
    static JASTaskThread* sLoadThread;

    /**
     * Buffer used to read DVD data. Can store the size of an entire streamed audio block.
     */
    static u8* sReadBuffer;

    /**
     * Block size used by all streamed music in the game.
     * This is 0x2760 for TP.
     */
    static u32 sBlockSize;

    /**
     * Maximum amount of output channels for all streamed music in the game.
     * This is 2 for TP (stereo).
     */
    static u32 sChannelMax;
};

#endif /* JASARAMSTREAM_H */
