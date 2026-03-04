#ifndef Z2AUDIOCS_SPKSOUND_H
#define Z2AUDIOCS_SPKSOUND_H

#include "Z2AudioCS/SpkTable.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JSupport/JSUList.h"
#include <revolution/types.h>

class SpkSound;

class SpkSoundHandle {
public:
    SpkSoundHandle(void) : mSound(NULL) {}
    ~SpkSoundHandle(void) {
        releaseSound();
    }
    void releaseSound(void);

    inline bool isSoundAttached() const {
        return mSound != NULL;
    }
    inline SpkSound* operator->(void) const {
        JUT_ASSERT(62, mSound != NULL);
        return mSound;
    }
    inline operator bool() const {
        return isSoundAttached();
    }

    /* 0x00 */ SpkSound* mSound;
};

class SpkSoundVolume {
public:
    SpkSoundVolume(void);
    void setRelease(s32);
    void setFadeOut(s32);
    f32 calc(bool&);
    void setTableVolume(f32);

    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 mTableVolume;
};

class SpkSound : public JASPoolAllocObject<SpkSound>, public JSULink<SpkSound> {
public:
    SpkSound(void);
    ~SpkSound(void);
    void update(f32 vol);
    void stop(s32 msec);
    void startRelease(void);
    bool isHandleAttached(void) const;
    void attachHandle(SpkSoundHandle* handle);
    void releaseHandle(void);
    bool start(s32 chan, s32 soundNum);
    void kill(void);
    bool isStopping(void) const;
    bool isDead(void) const;
    bool isLocked(void) const;
    void unlock(void);
    s32 getLifeTime(void);
    void setLifeTime(s32 lifeTime);
    void updateLifeTime(void);
    s32 convertMsecToFrames(s32 msec);
    bool isLoopWave(void);
    bool setWaveData(s32 soundNum);

    inline s32 getPriority(void) const { return mPriority; }
    inline void setPriority(s32 priority) { mPriority = priority; }
    inline s32 getSoundNum(void) const { return mSoundNum; }
    inline void setSoundNum(s32 soundNum) { mSoundNum = soundNum; }

    /* 0x10 */ SpkSoundHandle* mHandle;
    /* 0x14 */ s32 mSoundNum;
    /* 0x18 */ s16* mWaveData;
    /* 0x1C */ s32 mCurPos;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ s32 mPriority;
    /* 0x28 */ u32 field_0x28;
    /* 0x2C */ s32 mWaveSize;
    /* 0x30 */ s32 mWaveLoopStart;
    /* 0x34 */ s32 mWaveLoopEnd;
    /* 0x38 */ s32 mState;
    /* 0x3C */ s32 mLifeTime;
    /* 0x40 */ SpkSoundVolume mVolume;
};

class SpkSoundHolder : JASGlobalInstance<SpkSoundHolder> {
public:
    SpkSoundHolder(void);
    bool startSound(s32 chan, s32 soundNum, SpkSoundHandle* handle);
    bool startLevelSound(s32 chan, s32 soundNum, SpkSoundHandle* handle);
    void stopAll(s32 chan, s32 msec);
    bool update(s32 chan);
    bool updateEachSound(s32 chan);
    void freeDeadSound(s32 chan);
    void appendSound(s32 chan, SpkSound* sound);
    void framework(void);

    inline f32 getMasterVolume(void) const { return mMasterVolume; }
    inline void setMasterVolume(f32 vol) { mMasterVolume = vol; }
    inline s32 getConfigVolume(void) const { return mConfigVolume; }
    inline void setConfigVolume(s32 vol) { mConfigVolume = vol; }
    inline f32 getConfigVolumeF32(void) const { return (f32)mConfigVolume / 15.0f; }

    /* 0x00 */ JSUList<SpkSound> mSoundList[WPAD_MAX_CONTROLLERS];
    /* 0x30 */ f32 mSoundVolumes[WPAD_MAX_CONTROLLERS];
    /* 0x40 */ f32 mMasterVolume;
    /* 0x44 */ s32 mConfigVolume;
};

#endif /* Z2AUDIOCS_SPKSOUND_H */
