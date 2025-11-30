#ifndef Z2SOUNDOBJECT_H
#define Z2SOUNDOBJECT_H

#include "Z2AudioLib/Z2SoundHandles.h"
#include "JSystem/JAudio2/JAUSoundAnimator.h"

class Z2SoundStarter;

class Z2SoundObjBase : public Z2SoundHandles
#if VERSION == VERSION_SHIELD_DEBUG
, public JSULink<Z2SoundObjBase>
#endif
{
public:
    Z2SoundObjBase();
    ~Z2SoundObjBase();

    void deleteObject();
    Z2SoundHandlePool* startCollisionSE(u32 hitID, u32 mapinfo, Z2SoundObjBase* other);

    bool isAlive() { return alive_; }
    void setSoundStarter(Z2SoundStarter* soundStarter) { soundStarter_ = soundStarter; }

    virtual void framework(u32 mapinfo, s8 reverb);
    virtual void dispose();
    virtual bool stopOK(Z2SoundHandlePool& handlePool);
    virtual void init(Vec* posPtr, u8 handleNum);
    virtual Z2SoundHandlePool* startSound(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startLevelSound(JAISoundID soundID, u32 mapinfo, s8 reverb);

    /* 0x14 */ Z2SoundStarter* soundStarter_;
    /* 0x18 */ JGeometry::TVec3<f32>* pos_;
    /* 0x1C */ u16 field_0x1c;
    /* 0x1E */ s8 reverb_;
    /* 0x1F */ bool alive_;
};

class Z2SoundObjSimple : public Z2SoundObjBase {
public:
    Z2SoundObjSimple();
    ~Z2SoundObjSimple() {}

    virtual void init(Vec* posPtr, u8 handleNum);
    virtual Z2SoundHandlePool* startSound(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startLevelSound(JAISoundID soundID, u32 mapinfo, s8 reverb);
};

class Z2SoundObjAnime : public Z2SoundObjBase {
public:
    Z2SoundObjAnime();
    void initAnime(void* animation, bool, f32 startFrame, f32);
    void ageSounds_();
    void updateAnime(f32, f32);
    void updateSoundLifeTime_(f32, f32);
    void startSoundInner(const JGeometry::TVec3<f32>& pos, f32 param_1,
                                        Z2SoundStarter* soundStarter, u32 mapinfo, s8 reverb);
    u32 getSoundID(const JAUSoundAnimationSound* animationSound, const JGeometry::TVec3<f32>&, f32);
    bool playsSound(const JAUSoundAnimationSound* animationSound, const JGeometry::TVec3<f32>&, f32);

    virtual void init(Vec* posPtr, u8 handleNum);

    void stopAnime() { animation_ = NULL; }

    void setLoopStartFrame(f32 frame) {
        JUT_ASSERT(119, animation_);
        startSoundFrame_ = frame;
        startSoundIndex_ = animation_->getStartSoundIndex(frame);
    }

    void setLoopEndFrame(f32 frame) {
        JUT_ASSERT(125, animation_);
        endSoundFrame_ = frame;
        endSoundIndex_ = animation_->getEndSoundIndex(frame);
    }

    void setLoopFrame(f32 startFrame, f32 endFrame) {
        setLoopStartFrame(startFrame);
        setLoopEndFrame(endFrame);
    }

    /* 0x20 */ JAUSoundAnimation* animation_;
    /* 0x24 */ int field_0x24;
    /* 0x28 */ f32 curSoundFrame_;
    /* 0x2C */ f32 startSoundFrame_;
    /* 0x30 */ f32 endSoundFrame_;
    /* 0x34 */ int startSoundIndex_;
    /* 0x38 */ int endSoundIndex_;
    /* 0x3C */ int field_0x3c;
    /* 0x40 */ int curSoundIndex_;
    /* 0x44 */ bool reverse_;
};

class Z2DopplerSoundObjBase : public Z2SoundObjBase {
public:
    Z2DopplerSoundObjBase();
    ~Z2DopplerSoundObjBase();

    virtual void framework(u32 mapinfo, s8 reverb);
    virtual void init(Vec* posPtr, u8 handleNum);
    virtual Z2SoundHandlePool* startSound(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startLevelSound(JAISoundID soundID, u32 mapinfo, s8 reverb);

    /* 0x20 */ JGeometry::TVec3<f32> field_0x20;
    /* 0x2C */ JGeometry::TVec3<f32> field_0x2c;
};

class Z2SoundObjArrow : public Z2DopplerSoundObjBase {
public:
    Z2SoundObjArrow();
    virtual void init(Vec* posPtr, u8 handleNum);
};

#endif /* Z2SOUNDOBJECT_H */
