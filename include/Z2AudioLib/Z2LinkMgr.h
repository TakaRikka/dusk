#ifndef Z2LINKMGR_H
#define Z2LINKMGR_H

#include "SSystem/SComponent/c_xyz.h"
#include "Z2AudioLib/Z2Creature.h"

class Z2LinkSoundStarter : public Z2SoundStarter {
public:
    Z2LinkSoundStarter();

    inline virtual ~Z2LinkSoundStarter();
    virtual bool startSound(JAISoundID soundID, JAISoundHandle* handle,
                            const JGeometry::TVec3<f32>* posPtr, u32 mapinfo,
                            f32 fxMix, f32 pitch, f32 volume, f32 pan,
                            f32 dolby, u32 moveSteps);
};

class Z2CreatureLink : public Z2Creature, public Z2LinkSoundStarter {
public:
    Z2CreatureLink();
    ~Z2CreatureLink();

    void setLinkGroupInfo(u8);
    void setLinkHp(s32 linkHp, s32);
    void setLinkSwordType(s32 swordType, s32);
    void setLinkShieldType(s32 shieldType, s32);
    void setLinkBootsType(s32 bootsType);
    virtual void deleteObject();
    void init(Vec* animePosPtr, Vec* simplePosPtr, Vec* simple2PosPtr);
    void initKantera(Vec* posPtr);
    void deleteKantera();
    void setKanteraState(u8 state);
    virtual void framework(u32 mapinfo, s8 reverb);
    void setSinkDepth(s8 sinkDepth);
    void setRiding(bool isRiding);
    void setMagnetized(bool isMagnetized);
    void setWolfEyeOpen(bool wolfEyeOpen);
    Z2SoundHandlePool* startLinkSound(JAISoundID, u32 mapinfo, s8 reverb);
    JAISoundHandle* startLinkSoundLevel(JAISoundID, u32 mapinfo, s8 reverb);
    Z2SoundHandlePool* startLinkVoice(JAISoundID, s8 reverb);
    Z2SoundHandlePool* startLinkVoiceLevel(JAISoundID, s8 reverb);
    void startLinkSwordSound(JAISoundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCollisionSE(u32 hitID, u32 mapinfo);
    Z2SoundHandlePool* startHitItemSE(u32 soundID, u32 mapinfo, Z2SoundObjBase* other, f32 speed);
    void setResumeAttack(bool resumeAttack);

    void setInWater(bool inWater) { mInWater = inWater; }
    bool isInWater() { return mInWater; }
    u8 getLinkState() { return mLinkState; }
    u8 getMoveSpeed() { return mMoveSpeed; }
    bool isRiding() { return mRiding; }
    void setLinkState(u8 state) { mLinkState = state; }
    Z2SoundObjSimple& getKantera() { return mKantera; }
    u8 getLinkHp() { return mLinkHp; }
    u8 getMovingTime() { return mMovingTime; }
    bool isWolfSmellOpen() { return mWolfEyeOpen; }
    void setUsingIronBall(bool isUsingIronBall) { mUsingIronBall = isUsingIronBall; }
    void setMarkState(u8 state) { mMarkState = state; }

    static Z2CreatureLink* mLinkPtr;

    friend class Z2LinkSoundStarter;
    friend class Z2RideSoundStarter;

private:
    /* 0x90 Z2LinkSoundStarter */
    /* 0x94 */ Z2SoundObjSimple mKantera;
    /* 0xB4 */ Vec field_0xb4;
    /* 0xC0 */ u8 mLinkState;
    /* 0xC1 */ u8 field_0xc1;
    /* 0xC2 */ u8 mMarkState;
    /* 0xC3 */ u8 mLinkHp;
    /* 0xC4 */ u8 mLinkBootsType;
    /* 0xC5 */ u8 mKanteraState;
    /* 0xC6 */ u8 mLinkSwordType;
    /* 0xC7 */ u8 mLinkShieldType;
    /* 0xC8 */ u8 mMoveSpeed;
    /* 0xC9 */ u8 mMovingTime;
    /* 0xCA */ s8 mSinkDepth;
    /* 0xCB */ bool mWolfEyeOpen : 1;
    /* 0xCB */ bool mInWater : 1;
    /* 0xCB */ bool mResumeAttack : 1;
    /* 0xCB */ bool mFlag3 : 1;
    /* 0xCB */ bool mFlag4 : 1;
    /* 0xCB */ bool mFlag5 : 1;
    /* 0xCB */ bool mRiding : 1;
    /* 0xCB */ bool mMagnetized : 1;
    /* 0xCC */ bool mFlag8 : 1;
    /* 0xCC */ bool mUsingIronBall : 1;
    /* 0xCC */ bool mFlag10 : 1;
    /* 0xCC */ bool mFlag11 : 1;
    /* 0xCC */ bool mFlag12 : 1;
    /* 0xCC */ bool mFlag13 : 1;
    /* 0xCC */ bool mFlag14 : 1;
    /* 0xCC */ bool mFlag15 : 1;
};  // Size: 0xD0

inline Z2CreatureLink* Z2GetLink() {
    return Z2CreatureLink::mLinkPtr;
}

class Z2CreatureRide;
struct Z2RideSoundStarter : public Z2SoundStarter {
    Z2RideSoundStarter(Z2CreatureRide* ride);

    virtual bool startSound(JAISoundID soundID, JAISoundHandle* handle,
                                           const JGeometry::TVec3<f32>* posPtr, u32 mapinfo,
                                           f32 fxMix, f32 pitch, f32 volume, f32 pan,
                                           f32 dolby, u32 moveSteps);

    /* 0x4 */ Z2CreatureRide* mRide;
};

class Z2CreatureRide : public Z2Creature, public Z2RideSoundStarter {
public:
    Z2CreatureRide();
    ~Z2CreatureRide();
    void init(Vec* animePosPtr, Vec* simplePosPtr, u8 animeHandleNum, u8 simpleHandleNum);
    void setLinkRiding(bool isRiding);

    bool isLinkRiding() { return mLinkRiding; }

    virtual void deleteObject();

private:
    /* 0x90 */  // Z2RideSoundStarter mSoundStarter;
    /* 0x98 */ bool mLinkRiding;
};  // Size: 0x9C

class Z2SoundObjCoach : public Z2SoundObjBase {
public:
    Z2SoundObjCoach();
    void init(Vec* posPtr, u8 handleNum);
    Z2SoundHandlePool* startWheelSound(f32);
    Z2SoundHandlePool* startFireSound(u16);
    Z2SoundHandlePool* startSound(JAISoundID soundID, u32 mapinfo, s8 reverb);

private:
    /* 0x20 */ f32 field_0x20;
};

#endif /* Z2LINKMGR_H */
