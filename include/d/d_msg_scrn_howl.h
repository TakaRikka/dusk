#ifndef MSG_SCRN_D_MSG_SCRN_HOWL_H
#define MSG_SCRN_D_MSG_SCRN_HOWL_H

#include "d/d_msg_scrn_base.h"

struct dMsgScrnHowl_c : public dMsgScrnBase_c {
    dMsgScrnHowl_c();
    ~dMsgScrnHowl_c();
    void exec();
    void drawSelf();
    void guide_on_init();
    void guide_on_proc();
    void guide_off_init();
    void guide_off_proc();
    void guide_stop_init();
    void guide_stop_proc();
    void guide_demo_play_init();
    void guide_demo_play_proc();
    void guide_off_test_init();
    void guide_off_test_proc();
    void fukiScale(f32);
    void fukiTrans(f32, f32);
    void fukiAlpha(f32);
    bool isKeyCheck();
    void resetLine();
    void drawWave();
    void drawGuide();
    void drawGuide2();
    void drawEffect();
    void calcMain();
    void calcWave();
    void calcGuide();
    void moveLineV(bool);
    void moveBaseLength(bool);
    s8 getOnLineNum(int);
    s16 addCount(s16);
    s16 addCountGuide(s16);
    f32 getNowPlotPitch(f32);
    f32 getPlotPitch(f32);
    void calcPitchLevel();
    void initGuideData();
    u16 getGuideDataSize();
    u8 getGuideDataType(int);

    bool isHearingMode() { return field_0x2798 == 3;}

    /* 0x00C4 */ CPaneMgr* mpLineH[3];
    /* 0x00D0 */ CPaneMgr* mpLineV[7];
    /* 0x00EC */ CPaneMgr* mpLineAll;
    /* 0x00F0 */ CPaneMgr* mpABase;
    /* 0x00F4 */ CPaneMgr* mpButtonIcon[2];
    /* 0x00FC */ CPaneMgr* mpButtonText[2];
    /* 0x0104 */ J2DPicture* mpTopBall;
    /* 0x0108 */ J2DPicture* mpTopBallTail[5];
    /* 0x011C */ J2DPicture* mpDot;
    /* 0x0120 */ J2DPicture* mpGuideDot;
    /* 0x0124 */ J2DPicture* mpWaveTex;
    /* 0x0128 */ Vec field_0x128;
    /* 0x0134 */ Vec field_0x134;
    /* 0x0140 */ Vec field_0x140;
    /* 0x014C */ Vec field_0x14c;
    /* 0x0158 */ Vec field_0x158;
    /* 0x0164 */ Vec field_0x164;
    /* 0x0170 */ Vec field_0x170;
    /* 0x017C */ u32 field_0x17c;
    /* 0x0180 */ f32 field_0x180[0x300];
    /* 0x0D80 */ f32 field_0xd80[0x300];
    /* 0x1980 */ f32 field_0x1980;
    /* 0x1984 */ f32 field_0x1984;
    /* 0x1988 */ f32 field_0x1988;
    /* 0x198C */ f32 field_0x198c;
    /* 0x1990 */ f32 field_0x1990;
    /* 0x1994 */ f32 field_0x1994;
    /* 0x1998 */ f32 field_0x1998;
    /* 0x199C */ f32 field_0x199c;
    /* 0x19A0 */ f32 field_0x19a0[30];
    /* 0x1A18 */ f32 field_0x1a18[30];
    /* 0x1A90 */ f32 field_0x1a90[30];
    /* 0x1B08 */ f32 field_0x1b08[3];
    /* 0x1B14 */ s16 field_0x1b14[0x300];
    /* 0x2114 */ u32 field_0x2114;
    /* 0x2118 */ u32 field_0x2118;
    /* 0x211C */ u32 field_0x211c;
    /* 0x2120 */ u32 field_0x2120;
    /* 0x2124 */ s16 field_0x2124;
    /* 0x2126 */ u16 field_0x2126;
    /* 0x2128 */ s16 field_0x2128;
    /* 0x212A */ s16 mPlotTime;
    /* 0x212C */ s16 field_0x212c;
    /* 0x212E */ s16 field_0x212e;
    /* 0x2130 */ s16 field_0x2130;
    /* 0x2132 */ s16 field_0x2132;
    /* 0x2134 */ s16 field_0x2134;
    /* 0x2136 */ s16 field_0x2136;
    /* 0x2138 */ u16 field_0x2138;
    /* 0x213A */ u8 field_0x213a[30];
    /* 0x2158 */ u8 field_0x2158[30];
    /* 0x2176 */ u8 field_0x2176[30];
    /* 0x2194 */ s8 field_0x2194;
    /* 0x2195 */ u8 field_0x2195;
    /* 0x2196 */ u8 mCorrectLineMax;
    /* 0x2197 */ u8 field_0x2197;
    /* 0x2198 */ u8 field_0x2198[0x300];
    /* 0x2498 */ u8 field_0x2498[0x300];
    /* 0x2798 */ u8 field_0x2798;
    /* 0x2799 */ u8 field_0x2799;
    /* 0x279A */ u8 field_0x279a;
    /* 0x279B */ u8 field_0x279b;
    /* 0x279C */ f32 field_0x279c;
    /* 0x27A0 */ f32 field_0x27a0;
    /* 0x27A4 */ f32 field_0x27a4;
    /* 0x27A8 */ f32 field_0x27a8;
};

#define LINE_MAX 30
#define PLOT_BUFFER_MAX_e 0x300

#endif /* MSG_SCRN_D_MSG_SCRN_HOWL_H */
