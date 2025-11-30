#ifndef JASCHANNEL_H
#define JASCHANNEL_H

#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASLfo.h"
#include "JSystem/JAudio2/JASOscillator.h"
#include "JSystem/JAudio2/JASSoundParams.h"
#include "JSystem/JAudio2/JASWaveInfo.h"
#include <dolphin/os.h>

class JASDSPChannel;

namespace JASDsp {
    struct TChannel;
}

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASChannelParams {
public:
    JASChannelParams() {
        mVolume = 1.0f;
        mPitch = 1.0f;
        field_0x8 = 0.0f;
        mPan = 0.5f;
        mFxMix = 0.0f;
        mDolby = 0.0f;
    }
    void init() {
        mVolume = 1.0f;
        mPitch = 1.0f;
        field_0x8 = 0.0f;
        mPan = 0.5f;
        mFxMix = 0.0f;
        mDolby = 0.0f;
    }

    /* 0x00 */ f32 mVolume;
    /* 0x04 */ f32 mPitch;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 mPan;
    /* 0x10 */ f32 mFxMix;
    /* 0x14 */ f32 mDolby;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASChannel : public JASPoolAllocObject_MultiThreaded<JASChannel> {
public:
    typedef void (*Callback)(u32, JASChannel*, JASDsp::TChannel*, void*);

    enum CallbackType {
        /* 0 */ CB_PLAY,
        /* 1 */ CB_START,
        /* 2 */ CB_STOP,
        /* 3 */ CB_TIMER,
    };

    enum Status {
        /* 0 */ STATUS_INACTIVE,
        /* 1 */ STATUS_ACTIVE,
        /* 2 */ STATUS_RELEASE,
    };

    struct PanVector {
        /* 0x0 */ f32 mSound;
        /* 0x4 */ f32 mEffect;
        /* 0x8 */ f32 mChannel;
    };

    union MixConfig {
        u16 whole;
        struct {
            u8 upper;
            u8 lower0 : 4;
            u8 lower1 : 4;
        } parts;
    };

    JASChannel(Callback, void*);
    ~JASChannel();
    int play();
    int playForce();
    void release(u16);
    void setOscInit(u32, JASOscillator::Data const*);
    void setMixConfig(u32, u16);
    static f32 calcEffect(JASChannel::PanVector const*);
    static f32 calcPan(JASChannel::PanVector const*);
    void effectOsc(u32, JASOscillator::EffectParams*);
    void setKeySweepTarget(s32, u32);
    void updateEffectorParam(JASDsp::TChannel*, u16*,
                                            JASOscillator::EffectParams const&);
    static s32 dspUpdateCallback(u32, JASDsp::TChannel*, void*);
    s32 initialUpdateDSPChannel(JASDsp::TChannel*);
    s32 updateDSPChannel(JASDsp::TChannel*);
    void updateAutoMixer(JASDsp::TChannel*, f32, f32, f32, f32);
    void updateMixer(f32, f32, f32, f32, u16*);
    void free();
    static void initBankDisposeMsgQueue();
    static void receiveBankDisposeMsg();
    bool checkBankDispose() const;

    void setPauseFlag(bool param_0) { mPauseFlag = param_0; }
    void setUpdateTimer(u32 param_0) { mUpdateTimer = param_0; }
    void setBankDisposeID(const void* param_0) { mBankDisposeID = param_0; }
    void setDirectRelease(u16 param_0) { mOscillators[0].setDirectRelease(param_0); }
    void setVibrate(f32 param_0, f32 param_1) {
        mVibrate.setDepth(param_0);
        mVibrate.setPitch(param_1);
    }
    void setVibrateDelay(u16 param_0) { mVibrate.setDelay(param_0); }
    void setTremolo(f32 param_0, f32 param_1) {
        mTremolo.setDepth(param_0);
        mTremolo.setPitch(param_1);
    }
    void setTremoloDelay(u16 param_0) { mTremolo.setDelay(param_0); }
    void setPriority(u16 param_0) { mPriority = param_0; }
    void setParams(const JASChannelParams& param_0) { mParams = param_0; }
    void setInitVolume(f32 param_0) { mSoundParams.mVolume = param_0; }
    void setInitFxmix(f32 param_0) { mSoundParams.mFxMix = param_0; }
    void setInitPitch(f32 param_0) { mSoundParams.mPitch = param_0; }
    void setInitPan(f32 param_0) { mSoundParams.mPan = param_0; }
    void setInitDolby(f32 param_0) { mSoundParams.mDolby = param_0; }
    void setKey(s32 param_0) { mKey = param_0; }
    void setVelocity(u32 param_0) { mVelocity = param_0; }
    void setSkipSamples(u32 param_0) { mSkipSamples = param_0; }
    bool isDolbyMode() { return mMixConfig[0].whole == 0xffff; }

    /* 0x00 */ int mStatus;
    /* 0x04 */ bool mPauseFlag;
    /* 0x08 */ JASDSPChannel* mDspCh;
    /* 0x0C */ Callback mCallback;
    /* 0x10 */ void* mCallbackData;
    /* 0x14 */ u32 mUpdateTimer;
    /* 0x18 */ const void* mBankDisposeID;
    /* 0x1C */ JASOscillator mOscillators[2];
    /* 0x5C */ JASLfo mVibrate;
    /* 0x74 */ JASLfo mTremolo;
    /* 0x8C */ MixConfig mMixConfig[6];
    /* 0x98 */ u16 mPriority;
    /* 0x9C */ JASChannelParams mParams;
    /* 0xB4 */ JASSoundParams mSoundParams;
    /* 0xC8 */ s16 mKey;
    /* 0xCA */ u16 mVelocity;
    /* 0xCC */ f32 mKeySweep;
    /* 0xD0 */ f32 mKeySweepTarget;
    /* 0xD4 */ u32 mKeySweepCount;
    /* 0xD8 */ u32 mSkipSamples;
    struct {
        u32 field_0x0;
        JASWaveInfo field_0x4;
    } field_0xdc;
    int field_0x104;

    static OSMessageQueue sBankDisposeMsgQ;
    static OSMessage sBankDisposeMsg[16];
    static OSMessage sBankDisposeList[16];
    static int sBankDisposeListSize;
};

#endif /* JASCHANNEL_H */
