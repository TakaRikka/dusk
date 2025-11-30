#ifndef MSG_SCRN_D_MSG_SCRN_TALK_H
#define MSG_SCRN_D_MSG_SCRN_TALK_H

#include "dolphin/types.h"
#include "d/d_msg_scrn_base.h"

class dMsgScrnArrow_c;
class dMsgScrn3Select_c;
class dMsgScrnLight_c;

struct dMsgScrnTalk_c : public dMsgScrnBase_c {
    dMsgScrnTalk_c(u8, u8, JKRExpHeap*);
    ~dMsgScrnTalk_c();
    void exec();
    void drawSelf();
    void arwAnimeInit();
    void arwAnimeMove();
    void dotAnimeInit();
    void dotAnimeMove();
    void setSelectString(char*, char*, char*);
    void setSelectRubyString(char*, char*, char*);
    bool isSelect();
    void selectAnimeInit(u8, u8, f32, u8);
    bool selectAnimeMove(u8, u8, bool);
    bool selectAnimeEnd();
    void fukiScale(f32);
    void fukiTrans(f32, f32);
    void fukiAlpha(f32);
    void fukiPosCalc(u8);

    /* 0x0C4 */ J2DScreen* mpTxScreen;
    /* 0x0C8 */ dMsgScrnArrow_c* mpArrow_c;
    /* 0x0CC */ dMsgScrn3Select_c* mpSelect_c;
    /* 0x0D0 */ dMsgScrnLight_c* mpLight_c;
    /* 0x0D4 */ int field_0xd4;
    /* 0x0D8 */ CPaneMgr* mpArw_c;
    /* 0x0DC */ CPaneMgr* mpMg_c[2];
    /* 0x0E4 */ JKRExpHeap* field_0xe4;
    /* 0x0E8 */ f32 field_0xe8;
    /* 0x0EC */ f32 field_0xec;
    /* 0x0F0 */ f32 field_0xf0;
    /* 0x0F4 */ f32 field_0xf4;
    /* 0x0F8 */ f32 field_0xf8[150];
    /* 0x350 */ f32 field_0x350[3];
    /* 0x35C */ s16 field_0x35c[150];
    /* 0x488 */ u8 field_0x488;
};

#endif /* MSG_SCRN_D_MSG_SCRN_TALK_H */
