#ifndef Z2CREATURE_H
#define Z2CREATURE_H

#include "Z2AudioLib/Z2SoundObject.h"
#include "Z2AudioLib/Z2SoundStarter.h"

class Z2Creature {
public:
    Z2Creature();
    ~Z2Creature();

    void init(Vec* animePosPtr, Vec* simplePosPtr, u8 animeHandleNum, u8 simpleHandleNum);
    void init(Vec* animePosPtr, Vec* simplePosPtr, Vec* simple2PosPtr, u8 animeHandleNum, u8 simpleHandleNum, u8 simple2HandleNum);
    void initAnime(void* animation, bool, f32 startFrame, f32);
    void updateAnime(f32, f32);
    void stopAnime();

    virtual void deleteObject();
    virtual void setSoundStarter(Z2SoundStarter* soundStarter);
    virtual void framework(u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureSound(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureVoice(JAISoundID soundID, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureVoiceLevel(JAISoundID soundID, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureExtraSound(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureExtraSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCollisionSE(u32 hitID, u32 mapinfo);

    Vec* getCurrentPos() const { return mpPos; }

    /* 0x04 */ Vec* mpPos;
    /* 0x08 */ Z2SoundObjAnime mSoundObjAnime;
    /* 0x50 */ Z2SoundObjSimple mSoundObjSimple1;
    /* 0x70 */ Z2SoundObjSimple mSoundObjSimple2;
};  // Size: 0x90

class Z2CreatureEnemy : public Z2Creature, public JSULink<Z2CreatureEnemy> {
public:
    Z2CreatureEnemy();
    virtual void deleteObject();
    void init(Vec* animePosPtr, Vec* simplePosPtr, u8 animeHandleNum, u8 simpleHandleNum);
    void init(Vec*, Vec*, Vec*, u8, u8, u8);
    virtual void framework(u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureSound(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureVoice(JAISoundID soundID, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureVoiceLevel(JAISoundID soundID, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureExtraSound(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCreatureExtraSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb);
    virtual Z2SoundHandlePool* startCollisionSE(u32 hitID, u32 mapinfo);
    void setLinkSearch(bool isLinkSearch);
    void setEnemyName(const char* enemyName);

    /* 0xA0 */ u8 mEnemyID;
    /* 0xA1 */ s8 field_0xa1;
    /* 0xA2 */ bool mLinkSearch;
    /* 0xA3 */ u8 field_0xa3;
};  // Size: 0xA4

class Z2CreatureCitizen : public Z2Creature, public JSULink<Z2CreatureCitizen> {
public:
    Z2CreatureCitizen();
    ~Z2CreatureCitizen();
    void init(Vec* animePosPtr, Vec* simplePosPtr, u8 animeHandleNum, u8 simpleHandleNum);
    virtual void deleteObject();
    void setMdlType(s8, bool, bool);
    void playVoice(int);
    virtual Z2SoundHandlePool* startCreatureVoice(JAISoundID soundID, s8 reverb);

private:
    /* 0xA0 */ s8 mCitizenID;
    /* 0xA1 */ s8 field_0xa1;
    /* 0xA2 */ bool mLinkSearch;
    /* 0xA3 */ u8 field_0xa3;
};  // Size: 0xA4

class Z2CreatureSumomo : public Z2Creature {
public:
    Z2CreatureSumomo();
    void playSumomoBgm(u32 dist);
    void deleteObject();
};  // Size: 0x90

class Z2SoundObjBeeGroup : public Z2SoundObjBase {
public:
    Z2SoundObjBeeGroup();
    void init(Vec* posPtr, u8 handleNum);
    void playBeeGroupSound(JAISoundID soundID, u8);
};  // Size: 0x20

class Z2CreatureFM : public Z2CreatureEnemy {
public:
    Z2CreatureFM();
    void deleteObject();
    void init(Vec*, Vec*, Vec*, Vec*, Vec*, Vec*, u8, u8, u8, u8, u8, u8);
    void framework(u32 mapinfo, s8 reverb);
    Z2SoundHandlePool* startChainSound(JAISoundID soundID, u8, f32, u32 mapinfo, s8 reverb);
    Z2SoundHandlePool* startChainSoundLevel(JAISoundID soundID, u8, f32, u32 mapinfo, s8 reverb);

    /* 0x0A4 */ Z2SoundObjSimple field_0xa4;
    /* 0x0C4 */ Z2SoundObjSimple field_0xc4;
    /* 0x0E4 */ Z2SoundObjSimple field_0xe4;
    /* 0x104 */ Z2SoundObjSimple field_0x104;
};  // Size: 0x124

class Z2CreatureGob : public Z2Creature {
public:
    void init(Vec*, Vec*, Vec*, u8, u8, u8);
    Z2SoundHandlePool* startCreatureVoiceLevel(JAISoundID soundID, s8 reverb);
};

class Z2CreatureOI : public Z2CreatureEnemy {
public:
    Z2CreatureOI();
    void deleteObject();
    void init(Vec*, Vec*, Vec*, Vec*, Vec*, Vec*, Vec*, Vec*, Vec*, Vec*, Vec*, Vec*,
                             Vec*);
    void framework(u32 mapinfo, s8 reverb);
    Z2SoundHandlePool* startCreatureSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb);
    Z2SoundHandlePool* startTentacleSound(JAISoundID soundID, u8, u32 mapinfo, s8 reverb);
    Z2SoundHandlePool* startTentacleSoundLevel(JAISoundID soundID, u8, f32, u32 mapinfo, s8 reverb);

    /* 0x0A4 */ Z2SoundObjSimple field_0xa4;
    /* 0x0C4 */ Z2SoundObjSimple field_0xc4;
    /* 0x0E4 */ Z2SoundObjSimple field_0xe4;
    /* 0x104 */ Z2SoundObjSimple mTentacleSounds[8];
};  // Size: 0x204

#endif /* Z2CREATURE_H */
