#ifndef D_A_D_A_ITEM_STATIC_H
#define D_A_D_A_ITEM_STATIC_H

#include "SSystem/SComponent/c_lib.h"
#include "d/actor/d_a_itembase.h"
#include "d/d_particle.h"
#include "d/actor/d_a_player.h"

class daItem_c;
typedef void (daItem_c::*procFunc)();

class daItem_c : public daItemBase_c {
public:
    enum Type_e {
        TYPE_LAUNCH_e,
        TYPE_FIXED_PLACE_e,
        TYPE_UNK2_e,
        TYPE_WAIT_e,
        TYPE_SIMPLE_GET_e,
        TYPE_BOOM_HIT_e,
        TYPE_LAUNCH_NO_RND_e,
        TYPE_FORCE_GET_e,
        TYPE_LAUNCH_SMALL_e,
        TYPE_LAUNCH_FROM_PLAYER_e,
    };

    enum Flag_e {
        FLAG_UNK_2_e         = (1 << 1),
        FLAG_NO_MOVE_e       = (1 << 2),
        FLAG_NO_TIMER_e      = (1 << 3),
        FLAG_INIT_GET_ITEM_e = (1 << 4),
        FLAG_DELETE_ITEM_e   = (1 << 5),
    };

    enum Mode_e {
        MODE_WAIT_e,
        MODE_WATER_e,
    };

    enum Status_e {
        STATUS_NORMAL_e,
        STATUS_ENEMY_CARRY_e,
        STATUS_SIMPLE_GET_DEMO_e,
        STATUS_WAIT_GET_DEMO_EVENT_e,
        STATUS_GET_DEMO_EVENT_e,
        STATUS_BOOMERANG_CARRY_e,
        STATUS_SW_ON_WAIT_e,
        STATUS_BOOM_HIT_WAIT_e,
        STATUS_FORCE_GET_e,
    };

    daItem_c() : mSparkleEmtr(0, 0) {}
    void initBaseMtx();
    void setBaseMtx();
    void setBaseMtx_0();
    void setBaseMtx_1();
    void CreateInit();
    void setCullInfo();
    int _daItem_create();
    int _daItem_execute();
    int _daItem_draw();
    int _daItem_delete();
    void procInitNormal();
    void procMainNormal();
    void procMainEnemyCarry();
    void procInitForceGet();
    void procMainForceGet();
    void procInitSimpleGetDemo();
    void procMainSimpleGetDemo();
    void procInitGetDemoEvent();
    void procWaitGetDemoEvent();
    void procMainGetDemoEvent();
    void procInitBoomerangCarry();
    void procMainBoomerangCarry();
    void procInitSwOnWait();
    void procMainSwOnWait();
    void procInitBoomHitWait();
    void procMainBoomHitWait();
    void move_proc_call();
    void mode_wait_init();
    void mode_water_init();
    void mode_wait();
    void mode_water();
    void itemGetNextExecute();
    void itemGet();
    BOOL checkCountTimer();
    BOOL checkPlayerGet();
    void checkYogan();
    void deleteItem();
    int itemActionForRupee();
    int itemActionForHeart();
    int itemActionForArrow();
    int itemActionForBoomerang();
    void bg_check();
    void set_bound_se();
    int CountTimer();
    int initAction();
    void initFlag();
    void initScale();
    void initSpeed(int);
    void initAngle();
    u32 startCtrl();
    u32 startControl();
    u32 endControl();

    virtual void setTevStr();

    void setStatus(u8 status) { mStatus = status; }
    int getStatus() { return mStatus; }

    void setFlag(u8 pFlag) { cLib_onBit(mFlag, pFlag); }
    bool checkFlag(u8 flag) { return cLib_checkBit(mFlag, flag); }
    void clrFlag(u8 flag) { cLib_offBit(mFlag, flag); }

    bool checkBoomWindTgTimer() { return mBoomWindTgTimer == 0; }

    static procFunc mFuncPtr[];
    static dCcD_SrcCyl m_cyl_src;
    static s32 m_timer_max;

    /* 0x92C */ s16 field_0x92c;
    /* 0x92E */ u16 field_0x92e;
    /* 0x930 */ cXyz mItemScale;
    /* 0x93C */ int mSwitchNo;
    /* 0x940 */ u16 field_0x940;
    /* 0x942 */ s16 mWaitTimer;
    /* 0x944 */ s16 mDisappearTimer;
    /* 0x946 */ s16 mSimpleExistTimer;
    /* 0x948 */ u8 mFlag;
    /* 0x949 */ u8 mMode;
    /* 0x94A */ u8 mStatus;
    /* 0x94B */ u8 field_0x94b;
    /* 0x94C */ u32 m_item_id;
    /* 0x950 */ cXyz field_0x950;
    /* 0x95C */ u8 mSwOnWaitTimer;
    /* 0x95D */ bool field_0x95d;
    /* 0x95E */ u8 mBoomWindTgTimer;
    /* 0x95F */ u8 field_0x95f;
    /* 0x960 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x96C */ cXyz mLastPos;
    /* 0x978 */ Z2SoundObjSimple mSound;
    /* 0x998 */ dPa_followEcallBack mSparkleEmtr;
    /* 0x9AC */ cXyz mSparklePos;
    /* 0x9B8 */ u32 field_0x9b8;
    /* 0x9BC */ u8 field_0x9bc[4];
    /* 0x9C0 */ u8 field_0x9c0;
    /* 0x9C1 */ u8 field_0x9c1;
};  // Size: 0x9C4

#endif /* D_A_D_A_ITEM_STATIC_H */
