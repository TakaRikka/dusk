#ifndef JAISOUND_H
#define JAISOUND_H

#include "JSystem/JAudio2/JAISoundParams.h"
#include "JSystem/JAudio2/JAIAudible.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "global.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAISoundID {
public:
    operator u32() const { return id_.composite_; }

    JAISoundID(u32 id) { id_.composite_ = id; };

    JAISoundID(const JAISoundID& other) { id_.composite_ = other.id_.composite_; };

    JAISoundID() {}

    bool isAnonymous() const { return id_.composite_ == -1; }
    void setAnonymous() { id_.composite_ = -1; }

    union {
        u32 composite_;
        struct {
            union {
                u16 value;
                struct {
                    u8 sectionID;
                    u8 groupID;
                } parts;
            } type;
            u16 waveID;
        } info;
    } id_;
};

class JASTrack;

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundStatus_ {
    s32 lockWhenPrepared();
    s32 unlockIfLocked();

    void init() {
        field_0x0.value = 0;
        field_0x1.value = 0;
        *((u16*)&state) = 0;
        userdata_ = 0;
    }

    bool isAlive() const { return state.unk != 6; }
    bool isDead() const { return state.unk == 6;}

    bool isPlaying() const { return state.unk == 5; }
    bool isPrepared() const { return state.unk >= 3; }
    bool isMute() const { return field_0x0.flags.mute; }
    bool isPaused() const { return field_0x0.flags.paused; }
    void pauseWhenOut() {
        field_0x1.flags.flag6 = 1;
    }

    /* 0x0 */ union {
        u8 value;
        struct {
            u8 mute : 1;
            u8 paused : 1;
            u8 flag3 : 1;
            u8 flag4 : 1;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
    } field_0x0;
    /* 0x1 */ union {
        u8 value;
        struct {
            u8 flag1 : 1;
            u8 flag2 : 1;
            u8 flag3 : 1;
            u8 flag4 : 1;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
    } field_0x1;
    /* 0x2 */ struct {
        u8 unk;
        struct {
            u8 flag1 : 1;
            u8 calcedOnce : 1;
            u8 animationState : 2;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
    } state;
    /* 0x4 */ u32 userdata_;
};  // Size: 0x8

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundFader {
    void forceIn() {
        value_ = 1.0f;
        mTransition.zero();
    }

    void forceOut() {
        value_ = 0.0f;
        mTransition.zero();
    }

    void fadeOut(u32 maxSteps) {
        if (maxSteps != 0) {
            mTransition.set(0.0f, value_, maxSteps);
        } else {
            forceOut();
        }
    }

    void fadeIn(u32 maxSteps) {
        if (maxSteps != 0) {
            mTransition.set(1.0f, value_, maxSteps);
        } else {
            forceIn();
        }
    }

    void fadeInFromOut(u32 maxSteps) {
        value_ = 0.0f;
        fadeIn(maxSteps);
    }

    bool isOut() const {
        return (mTransition.remainingSteps_ == 0 && value_ < 0.01f);
    }

    void calc() { value_ = mTransition.apply(value_); }
    f32 getIntensity() const { return value_; }

    /* 0x00 */ f32 value_;
    /* 0x04 */ JAISoundParamsTransition::TTransition mTransition;
};  // Size: 0x10

/**
 * @ingroup jsystem-jaudio
 * 
 */
template <typename A0>
struct JAISoundStrategyMgr__unknown {
    virtual void virtual2();
    virtual void calc(A0*);
    virtual void virtual4(A0*, const JASSoundParams&);
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
template <typename A0>
struct JAISoundStrategyMgr {
    virtual void virtual2();
    virtual JAISoundStrategyMgr__unknown<A0>* calc(JAISoundID);
    virtual void virtual4(JAISoundStrategyMgr__unknown<A0>*);
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAISoundActivity {
public:
    void init() { field_0x0.value = 0; }

    /* 0x0 */ union {
        u8 value;
        struct {
            u8 flag1 : 1;
            u8 flag2 : 1;
            u8 flag3 : 1;
            u8 flag4 : 1;
            u8 flag5 : 1;
            u8 flag6 : 1;
            u8 flag7 : 1;
            u8 flag8 : 1;
        } flags;
    } field_0x0;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAITempoMgr {
public:
    /* 0x00 */ f32 tempo_;
    /* 0x04 */ JAISoundParamsTransition::TTransition transition_;

    JAITempoMgr() { init(); }
    void init() { setTempo(1.0f); }
    void setTempo(f32 tempo) {
        tempo_ = tempo;
        transition_.zero();
    }
    f32 getTempo() const { return tempo_; }
    void calc() { tempo_ = transition_.apply(tempo_); }
};

class JAISoundHandle;
class JAIAudible;
class JAIAudience;
class JAISe;
class JAISeq;
class JAISoundChild;
class JAIStream;
class JAITempoMgr;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAISound {
public:
    void releaseHandle();
    void attachHandle(JAISoundHandle* handle);
    JAISound();
    void start_JAISound_(JAISoundID id, const JGeometry::TVec3<f32>* posPtr, JAIAudience* audience);
    bool acceptsNewAudible() const;
    void newAudible(const JGeometry::TVec3<f32>&, JGeometry::TVec3<f32> const*, u32,
                                   JAIAudience*);
    void stop();
    void stop(u32 fadeTime);
    void die_JAISound_();
    void increasePrepareCount_JAISound_();
    bool calc_JAISound_();
    void initTrack_JAISound_(JASTrack* track);

    virtual s32 getNumChild() const = 0;
    virtual JAISoundChild* getChild(int) = 0;
    virtual void releaseChild(int) = 0;
    virtual JAISe* asSe();
    virtual JAISeq* asSeq();
    virtual JAIStream* asStream();
    virtual JASTrack* getTrack() = 0;
    virtual JASTrack* getChildTrack(int) = 0;
    virtual JAITempoMgr* getTempoMgr() = 0;
    virtual bool JAISound_tryDie_() = 0;

    JAISoundID getID() const { return soundID_; }
    u32 getAnimationState() const { return status_.state.flags.animationState; }
    bool isAnimated() const { return getAnimationState() != 0; }
    void setAnimationState(u32 state) {
        status_.state.flags.animationState = state;
    }
    u32 getUserData() const { return status_.userdata_; }
    void setUserData(u32 userData) { status_.userdata_ = userData; }
    JAIAudible* getAudible() { return audible_; }
    bool isHandleAttached() const { return handle_ != NULL; }
    bool hasLifeTime() const { return status_.field_0x1.flags.flag2; }

    void removeLifeTime_() {
        status_.field_0x1.flags.flag1 = false;
        status_.field_0x1.flags.flag2 = 0;
    }

    bool isPrepared() const { return status_.isPrepared(); }
    void unlockIfLocked() { status_.unlockIfLocked(); }
    void lockWhenPrepared() { status_.lockWhenPrepared(); }
    void stop_JAISound_() {
        status_.state.flags.flag5 = 0;
        status_.state.flags.flag1 = 1;
    }

    bool isStopping() const {
        return status_.state.flags.flag1 && (!status_.state.flags.flag5 || fader_.isOut());
    }

    void pause(bool param_0) {
        status_.field_0x0.flags.paused = param_0;
    }

    void updateLifeTime(u32 lifeTime) {
        if (lifeTime > lifeTime_) {
            lifeTime_ = lifeTime;
        }
    }

    void setLifeTime(u32 lifeTime, bool param_1) {
        JUT_ASSERT(333, status_.state.flags.calcedOnce == 0);
        lifeTime_ = lifeTime;
        setComesBack(param_1);
        status_.field_0x1.flags.flag2 = 1;
    }

    void setComesBack(bool param_0) {
        JUT_ASSERT(354, status_.state.flags.calcedOnce == 0);
        status_.field_0x1.flags.flag1 = 1;
        if (param_0) {
            status_.pauseWhenOut();
        }
    }

    bool setPos(const JGeometry::TVec3<f32>& pos) {
        if (audible_ != NULL) {
            audible_->setPos(pos);
        }
        return audible_ != NULL;
    }

    JAISoundFader& getFader() { return fader_; }
    void fadeIn(u32 maxSteps) { fader_.fadeInFromOut(maxSteps); }
    void fadeOut(u32 maxSteps) { fader_.fadeOut(maxSteps); }

    JAISoundParamsProperty& getProperty() { return params_.property_; }

    s32 getCount() const { return count_; }

    JAISoundParamsMove& getAuxiliary() { return params_.move_; }

    /* 0x04 */ JAISoundHandle* handle_;
    /* 0x08 */ JAIAudible* audible_;
    /* 0x0C */ JAIAudience* audience_;
    /* 0x10 */ u32 lifeTime_;
    /* 0x14 */ s32 prepareCount_;
    /* 0x18 */ JAISoundID soundID_;
    /* 0x1C */ JAISoundStatus_ status_;
    /* 0x24 */ JAISoundFader fader_;
    /* 0x34 */ u32 priority_;
    /* 0x38 */ s32 count_;
    /* 0x3C */ JAISoundParams params_;
};  // Size: 0x98

STATIC_ASSERT(sizeof(JAISound) == 0x98);

#endif /* JAISOUND_H */
