#ifndef D_SHOP_D_SHOP_SYSTEM_H
#define D_SHOP_D_SHOP_SYSTEM_H

#include "d/actor/d_a_npc.h"
#include "d/d_shop_item_ctrl.h"
#include "d/d_shop_camera.h"

class dSelect_cursor_c;
class STControl;

class dShopSystem_c : public daNpcT_c {
public:
    enum {
        /* 0x0 */ SEQ_WAIT,
        /* 0x1 */ SEQ_START,
        /* 0x2 */ SEQ_SELECT_WAIT,
        /* 0x3 */ SEQ_SELECT_START,
        /* 0x4 */ SEQ_SELECT,
        /* 0x5 */ SEQ_MOVING,
        /* 0x6 */ SEQ_DECIDE,
        /* 0x7 */ SEQ_CHOOSE,
        /* 0x8 */ SEQ_DECIDE_YES,
        /* 0x9 */ SEQ_DECIDE_NO,
        /* 0xA */ SEQ_FINISH,
    };

    enum {
        ITEM_MAX_e = 7,
    };

    dShopSystem_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
             daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
             daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6, daNpcT_evtData_c const* param_7,
             char** param_8) : daNpcT_c(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8) {
            OS_REPORT("|%06d:%x|dShopSystem_c -> コンストラクト\n", g_Counter.mCounter0, this);
            initShopSystem();
    }

    void initShopSystem();
    void onFlag(int);
    void offFlag(int);
    BOOL isFlag(int);
    void onSoldOutItemFlag(int);
    void offSoldOutItemFlag(int);
    BOOL isSoldOutItemFlag(int);
    int checkController(u8, dMsgFlow_c*);
    int chooseItem3(u8);
    int chooseItem5(u8);
    int chooseItem4(u8);
    int moveCursor(int, u8);
    int moveCursor0(int, u8);
    int moveCursor1(int, u8);
    int drawCursor();
    int itemRotate();
    int itemZoom(cXyz*);
    int seq_wait(fopAc_ac_c*, dMsgFlow_c*);
    int seq_start(fopAc_ac_c*, dMsgFlow_c*);
    int seq_select_wait(fopAc_ac_c*, dMsgFlow_c*);
    int seq_select_start(fopAc_ac_c*, dMsgFlow_c*);
    int seq_select(fopAc_ac_c*, dMsgFlow_c*);
    int seq_moving(fopAc_ac_c*, dMsgFlow_c*);
    int seq_decide(fopAc_ac_c*, dMsgFlow_c*);
    int seq_choose(fopAc_ac_c*, dMsgFlow_c*);
    int seq_decide_yes(fopAc_ac_c*, dMsgFlow_c*);
    int seq_decide_no(fopAc_ac_c*, dMsgFlow_c*);
    int seq_finish(fopAc_ac_c*, dMsgFlow_c*);
    bool seq_event(fopAc_ac_c*, dMsgFlow_c*);
    int shop_init(bool);
    int shop_process(fopAc_ac_c*, dMsgFlow_c*);
    void createShopItem(int);
    void setSeq(u8);
    void setSoldOutFlag();
    void setSoldOut();
    void setSoldOutItemHide();
    void deleteObject();
    bool searchItemActor();
    int getFlowNodeNum();
    void setSellItemMax(u8);
    BOOL checkShopOpen();
    bool checkLeftTrigger(STControl*);
    bool checkRightTrigger(STControl*);
    bool dpdMove();

    virtual ~dShopSystem_c();
    virtual bool getResName2(int) { return false; }
    virtual int beforeStartSeqAction(dMsgFlow_c*, int) { return 1; }
    virtual int beforeSelectSeqAction(dMsgFlow_c*, int) { return 1; }

    bool chkSpMode() { return mSpMode == 1; }
    void onSpMode() { mSpMode = 1; }
    void offSpMode() { mSpMode = 0; }
    u8 getEventParamU8(int param_0) { return (mEventParam >> (3 - param_0) * 8) & 0xFF; }
    void setEventParam(u32 param) { mEventParam = param; }
    void setMasterType(u8 master_type) { mMasterType = master_type; }
    u8 getCursorPos() { return mCursorPos; }
    dShopItemCtrl_c* getShopItemCtrlP() { return &mItemCtrl; }

    u8 chkExplainItem() {
        u8 rv = 0;
        if (mSeq == 2 || mSeq == 3 || mSeq == 4 || mSeq == 5) {
            rv = 1;
        }
        return rv;
    }

public:
    /* 0xE40 */ STControl* mpStick;
    /* 0xE44 */ dSelect_cursor_c* mpDrawCursor;
    /* 0xE48 */ dShopItemCtrl_c mItemCtrl;
    /* 0xE78 */ ShopCam_action_c mShopCamAction;
    /* 0xF54 */ u32 mEventParam;
    /* 0xF58 */ int field_0xf58;
    /* 0xF5C */ int field_0xf5c;
    /* 0xF60 */ int field_0xf60;
    /* 0xF64 */ int field_0xf64;
    /* 0xF68 */ s16 field_0xf68;
    /* 0xF6A */ u16 mFlag;
    /* 0xF6C */ u16 field_0xf6c;
    /* 0xF6E */ u8 mCursorPos;
    /* 0xF6F */ u8 mLastCursorPos;
    /* 0xF70 */ u8 mSeq;
    /* 0xF71 */ u8 field_0xf71;
    /* 0xF72 */ u8 mWaitTimer;
    /* 0xF73 */ u8 mSoldOutFlag;
    /* 0xF74 */ u8 mSoldOutItemFlags;
    /* 0xF75 */ u8 mSpMode;
    /* 0xF76 */ u8 field_0xf76;
    /* 0xF77 */ u8 mMasterType;
    /* 0xF78 */ u8 field_0xf78;
    /* 0xF79 */ u8 field_0xf79;
    /* 0xF7A */ u8 field_0xf7a;
};

typedef int (dShopSystem_c::*processFunc)(fopAc_ac_c*, dMsgFlow_c*);

#endif /* D_SHOP_D_SHOP_SYSTEM_H */
