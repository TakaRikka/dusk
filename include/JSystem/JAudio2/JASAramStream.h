#ifndef JASARAMSTREAM_H
#define JASARAMSTREAM_H

#include "JSystem/JAudio2/JASTaskThread.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/dvd.h"

class JASChannel;

namespace JASDsp {
    class TChannel;
}

#define CHANNEL_MAX 6

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASAramStream {
public:
    typedef void (*StreamCallback)(u32, JASAramStream*, void*);

    enum CallbackType {
        /* 0 */ CB_START,
        /* 1 */ CB_STOP,
    };

    // Used internally for passing data to task functions
    struct TaskData {
        /* 0x0 */ JASAramStream* stream;
        /* 0x4 */ u32 field_0x4;
        /* 0x8 */ int field_0x8;
    };

    struct Header {
        /* 0x00 */ u32 tag;
        /* 0x04 */ u8 field_0x4[5];
        /* 0x09 */ u8 format;
        /* 0x0A */ u8 bits;
        /* 0x0C */ u16 channels;
        /* 0x0E */ u16 loop;
        /* 0x10 */ int field_0x10;
        /* 0x14 */ u8 field_0x14[4];
        /* 0x18 */ int loop_start;
        /* 0x1C */ int loop_end;
        /* 0x20 */ u32 block_size;
        /* 0x24 */ u8 field_0x24[4];
        /* 0x28 */ u8 field_0x28;
        /* 0x29 */ u8 field_0x29[0x17];
    };  // Size: 0x40

    struct BlockHeader {
        /* 0x00 */ u32 tag;
        /* 0x04 */ u32 field_0x4;
        /* 0x08 */ struct {
            s16 field_0x0;
            s16 field_0x2;
        } field_0x8[6];
    };  // Size: 0x20

    static void initSystem(u32, u32);
    JASAramStream();
    void init(u32, u32, StreamCallback, void*);
    bool prepare(s32, int);
    bool start();
    bool stop(u16);
    bool pause(bool);
    bool cancel();
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

    /* 0x000 */ OSMessageQueue field_0x000;
    /* 0x020 */ OSMessageQueue field_0x020;
    /* 0x040 */ void* field_0x040[16];
    /* 0x080 */ void* field_0x080[4];
    /* 0x090 */ JASChannel* mChannels[CHANNEL_MAX];
    /* 0x0A8 */ JASChannel* field_0x0a8;
    /* 0x0AC */ bool field_0x0ac;
    /* 0x0AD */ bool field_0x0ad;
    /* 0x0AE */ u8 field_0x0ae;
    /* 0x0B0 */ int field_0x0b0;
    /* 0x0B4 */ int field_0x0b4;
    /* 0x0B8 */ u32 field_0x0b8;
    /* 0x0BC */ int field_0x0bc;
    /* 0x0C0 */ bool field_0x0c0;
    /* 0x0C4 */ u32 field_0x0c4;
    /* 0x0C8 */ f32 field_0x0c8;
    /* 0x0CC */ DVDFileInfo mDvdFileInfo;
    /* 0x108 */ u32 field_0x108;
    /* 0x10C */ int field_0x10c;
    /* 0x110 */ u32 mBlock;
    /* 0x114 */ u8 field_0x114;
    /* 0x118 */ u32 field_0x118;
    /* 0x11C */ int field_0x11c;
    /* 0x120 */ int field_0x120;
    /* 0x124 */ int field_0x124;
    /* 0x128 */ u16 field_0x128;
    /* 0x12C */ int field_0x12c;
    /* 0x130 */ s16 field_0x130[CHANNEL_MAX];
    /* 0x13C */ s16 field_0x13c[CHANNEL_MAX];
    /* 0x148 */ int field_0x148;
    /* 0x14C */ u32 field_0x14c;
    /* 0x150 */ StreamCallback mCallback;
    /* 0x154 */ void* mCallbackData;
    /* 0x158 */ u16 field_0x158;
    /* 0x15A */ u16 mChannelNum;
    /* 0x15C */ u32 mBufCount;
    /* 0x160 */ u32 field_0x160;
    /* 0x164 */ u32 field_0x164;
    /* 0x168 */ bool mLoop;
    /* 0x16C */ u32 mLoopStart;
    /* 0x170 */ u32 mLoopEnd;
    /* 0x174 */ f32 mVolume;
    /* 0x178 */ f32 mPitch;
    /* 0x17C */ f32 mChannelVolume[CHANNEL_MAX];
    /* 0x194 */ f32 mChannelPan[CHANNEL_MAX];
    /* 0x1AC */ f32 mChannelFxMix[CHANNEL_MAX];
    /* 0x1C4 */ f32 mChannelDolby[CHANNEL_MAX];
    /* 0x1DC */ u16 field_0x1dc[CHANNEL_MAX];

    static JASTaskThread* sLoadThread;
    static u8* sReadBuffer;
    static u32 sBlockSize;
    static u32 sChannelMax;
};

#endif /* JASARAMSTREAM_H */
