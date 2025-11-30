#ifndef MSG_SCRN_D_MSG_SCRN_EXPLAIN_H
#define MSG_SCRN_D_MSG_SCRN_EXPLAIN_H

#include "d/d_msg_scrn_base.h"

class J2DOrthoGraph;
class STControl;
class dMsgScrnArrow_c;
class dMsgScrn3Select_c;
class dMsgString_c;

class dMsgScrnExplain_c {
public:
    dMsgScrnExplain_c(STControl*, u8, bool, u8);
    void move();
    void draw(J2DOrthoGraph*);
    void wait_init();
    void wait_proc();
    void open_request_init();
    void open_request_proc();
    void open_init();
    void open_proc();
    void move_init();
    void move_proc();
    void move_select_init();
    void move_select_proc();
    void close_init();
    void close_proc();
    bool openExplain(u32, u8, u8, u8, bool);
    f32 getAlphaRatio();
    bool checkTriggerA();
    bool checkTriggerB();

    typedef void (dMsgScrnExplain_c::*ProcFunc)(); 

    virtual ~dMsgScrnExplain_c();

    u8 getStatus() { return mStatus; }
    u8 getSelectCursor() { return mSelCursor; }
    void onForceSelect() { mForceSelect = 1; }
    void setKeyWaitTimer(s16 timer) { mKeyWaitTimer = timer; }

private:
    /* 0x04 */ dMsgString_c* mpString_c;
    /* 0x08 */ CPaneMgr* mpTm_c[2];
    /* 0x10 */ CPaneMgr* field_0x10[2];
    /* 0x18 */ CPaneMgr* mpMg_c[2];
    /* 0x20 */ CPaneMgr* mpRoot_c[2];
    /* 0x28 */ CPaneMgr* mpArw_c;
    /* 0x2C */ J2DScreen* mpScreen;
    /* 0x30 */ J2DScreen* mpTxScreen;
    /* 0x34 */ dMsgScrnArrow_c* mpArrow_c;
    /* 0x38 */ dMsgScrn3Select_c* mpSelect_c;
    /* 0x3C */ COutFont_c* mpOutFont;
    /* 0x40 */ J2DPicture* mpBackTex;
    /* 0x44 */ STControl* mpStick;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 field_0x4c;
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ u32 field_0x54;
    /* 0x58 */ u8 field_0x58;
    /* 0x5A */ s16 field_0x5a;
    /* 0x5C */ s16 field_0x5c;
    /* 0x5E */ s16 mKeyWaitTimer;
    /* 0x60 */ u8 mStatus;
    /* 0x61 */ u8 mSelCursor;
    /* 0x62 */ u8 field_0x62;
    /* 0x63 */ u8 field_0x63;
    /* 0x64 */ u8 field_0x64;
    /* 0x65 */ u8 mForceSelect;
    /* 0x66 */ u8 field_0x66;
    /* 0x67 */ u8 field_0x67;
};  // Size: 0x68

#endif /* MSG_SCRN_D_MSG_SCRN_EXPLAIN_H */
