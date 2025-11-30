#ifndef Z2SPEECHMGR2_H
#define Z2SPEECHMGR2_H

#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JMath/random.h"
#include "Z2AudioLib/Z2SoundStarter.h"

struct Z2SpeechStarter : public Z2SoundStarter {
    Z2SpeechStarter();

    virtual bool startSound(JAISoundID, JAISoundHandle*,
                                           JGeometry::TVec3<f32> const*, u32, f32, f32, f32, f32,
                                           f32, u32);
};

// Only here for instance sub. No known code.
struct Z2SpeechMgr : public JASGlobalInstance<Z2SpeechMgr> {
};

struct Z2SpeechMgr2 : public JASGlobalInstance<Z2SpeechMgr2> {
    Z2SpeechMgr2();
    ~Z2SpeechMgr2() {}
    void setString(u16 const*, s16, u8, u16);
    void setTextCount(s16);
    void speakOneWord(bool);
    bool isNonVerbal();
    void selectUnit(bool);
    void selectTail();
    void framework();
    void playOneShotVoice(u8, u16, Vec*, s8);
    bool isMidnaSpeak();

    /* 0x000 */ JAISoundHandle field_0x0;
    /* 0x004 */ JAISoundHandle field_0x4;
    /* 0x008 */ JMath::TRandom_<JMath::TRandom_fast_> random;
    /* 0x00C */ Z2SpeechStarter mSpeech;
    /* 0x010 */ u16 mText[500];
    /* 0x3F8 */ s16 mTextNum;
    /* 0x3FA */ s16 mTextCount;
    /* 0x3FC */ u16 mMood;
    /* 0x3FE */ u8 mVoice;
    /* 0x3FF */ u8 field_0x3ff;
    /* 0x400 */ u8 field_0x400;
    /* 0x401 */ u8 field_0x401;
    /* 0x402 */ s8 field_0x402[64];
};  // Size: 0x444

STATIC_ASSERT(sizeof(Z2SpeechMgr2) == 0x444);

inline Z2SpeechMgr2* Z2GetSpeechMgr2() {
    return Z2SpeechMgr2::getInstance();
}

#endif /* Z2SPEECHMGR2_H */
