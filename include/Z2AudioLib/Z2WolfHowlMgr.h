#ifndef Z2WOLFHOWLMGR_H
#define Z2WOLFHOWLMGR_H

#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JAISoundHandles.h"

/*
 * Z2WolfHowlData
 * mLineNum: Number of lines/points within the song
 * mSongData: u16 array which corresponds to each line/point
 *
 * The high byte of each u16 line corresponds to bar type (low, middle, high)
 * The low byte of each u16 line corresponds to length of note
 */

struct Z2WolfHowlLine {
    u8 type;
    u8 length;
};

#define HOWL_LINE_HIGH 1
#define HOWL_LINE_MID  2
#define HOWL_LINE_LOW  3

struct Z2WolfHowlData {
    u8 lineNum;
    Z2WolfHowlLine* lineData;
};

enum Z2WolfHowlCurveID {
    Z2WOLFHOWL_NONE = -1,
    Z2WOLFHOWL_TOBIKUSA,
    Z2WOLFHOWL_UMAKUSA,
    Z2WOLFHOWL_ZELDASONG,
    Z2WOLFHOWL_HEALINGSONG,
    Z2WOLFHOWL_SOULREQUIEM,
    Z2WOLFHOWL_LIGHTPRELUDE,
    Z2WOLFHOWL_NEWSONG1,
    Z2WOLFHOWL_NEWSONG2,
    Z2WOLFHOWL_NEWSONG3,

    #if TARGET_PC
    Z2WOLFHOWL_TIMESONG,
    #endif

    Z2WOLFHOWL_MAX
};

class Z2WolfHowlMgr : public JASGlobalInstance<Z2WolfHowlMgr> {
public:
    Z2WolfHowlMgr();

    void resetState();
    void calcVolumeMod(f32);
    f32 getNowPitch();
    f32 getNowInputValue();
    void calcPitchMod(f32, f32);
    s8 startWolfHowlSound(f32, f32 stickValue, bool isHowl, f32);
    void setCorrectData(s8 curveID, Z2WolfHowlData* data);
    Z2WolfHowlLine getCorrectLine(u8 lineIndex);
    u8 getCorrectLineNum();
    s8 checkLine();
    s8 getOnLineNum();
    void startWindStoneSound(s8 curveID, Vec* pos);
    int startGuideMelody(bool startSound);
    void skipCorrectDemo();

    void setCorrectCurve(s8 curveID) { setCorrectData(curveID, NULL); }
    u32 getTimer() { return mTimer; }
    u8 getReleaseTimer() { return mReleaseTimer; }
    u8 getCorrectCurveID() { return mCorrectCurveID; }

private:
    /* 0x00 */ JAISoundHandle mWolfHowlHandle;
    /* 0x04 */ JAISoundHandle mWindStoneHandle;
    /* 0x08 */ JAISoundHandle mGuideHandle;
    /* 0x0C */ Z2WolfHowlData* mNowHowlData;
    /* 0x10 */ Z2WolfHowlData* mGuideData;
    /* 0x14 */ f32 mNowInputValue;
    /* 0x18 */ f32 mWolfHowlVolume;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 mNowPitch;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ u8 field_0x2c[4];
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38[10];
    /* 0x60 */ f32 field_0x60[10];
    /* 0x88 */ u32 mTimer;
    /* 0x8C */ u8 mReleaseTimer;
    /* 0x8D */ u8 field_0x8d;
    /* 0x8E */ s8 mCorrectCurveID;
    /* 0x8F */ u8 field_0x8f;
    /* 0x90 */ s16 mCorrectScore;
    /* 0x92 */ u16 field_0x92[20];
    /* 0xBA */ u8 mNowLineIndex;
    /* 0xBB */ s8 field_0xbb;
    /* 0xBC */ u8 mNowLineScore;
    /* 0xBD */ s8 field_0xbd;
    /* 0xBE */ u16 mFullScore;
};

#endif /* Z2WOLFHOWLMGR_H */
