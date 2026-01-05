#ifndef D_A_D_A_PLAYER_H
#define D_A_D_A_PLAYER_H

#include "d/d_drawlist.h"
#include "f_op/f_op_actor.h"
#include "d/d_com_inf_game.h"

struct ResTIMG;

class daPy_sightPacket_c : public dDlst_base_c {
public:
    daPy_sightPacket_c() {}
    virtual void draw();
    virtual ~daPy_sightPacket_c() {}

    void setSight();
    void setSightImage(ResTIMG*);

    bool getDrawFlg() { return mDrawFlag; }
    void onDrawFlg() { mDrawFlag = true; }
    void offDrawFlg() { mDrawFlag = false; }
    void setPos(const cXyz* i_pos) { mPos = *i_pos; }
    cXyz* getPosP() { return &mPos; }

    /* 0x04 */ bool mDrawFlag;
    /* 0x05 */ u8 field_0x5[3];
    /* 0x08 */ cXyz mPos;
    /* 0x14 */ Mtx field_0x14;
    /* 0x44 */ ResTIMG* mpImg;
    /* 0x48 */ u8* mpData;
};

class daPy_boomerangMove_c {
public:
    void initOffset(cXyz const*);
    void initOffset(cXyz const* xyz, dCcD_GObjInf const*) { initOffset(xyz); }
    int posMove(cXyz*, s16*, fopAc_ac_c*, s16);
    void bgCheckAfterOffset(cXyz const*);

    static void initDropAngleY() { m_dropAngleY = 0x4000; }
    static void offEventKeepFlg() { m_eventKeepFlg = 0; }
    static void onEventKeepFlg() { m_eventKeepFlg = 1; }

    static s16 m_dropAngleY;
    static s16 m_eventKeepFlg;

private:
    /* 0x0 */ u8 field_0x0;
    /* 0x2 */ s16 field_0x2;
    /* 0x4 */ f32 field_0x4;
    /* 0x8 */ f32 field_0x8;
};  // Size: 0xC

class daPy_anmHeap_c {
public:
    enum daAlinkHEAP_TYPE {
        HEAP_TYPE_1 = 1,
        HEAP_TYPE_2,
        HEAP_TYPE_3,
        HEAP_TYPE_4,
        HEAP_TYPE_5,
    };

    daPy_anmHeap_c(u32 param_0 = 0);
    ~daPy_anmHeap_c();
    void initData();
    void* mallocBuffer();
    void createHeap(daPy_anmHeap_c::daAlinkHEAP_TYPE);
    void* loadData(u16);
    void* loadDataIdx(u16);
    void* loadDataPriIdx(u16);
    void* loadDataDemoRID(u16, u16);
    JKRHeap* setAnimeHeap();

    u16 getIdx() const { return mIdx; }
    void resetIdx() { mIdx = 0xffff; }
    void resetPriIdx() { mPriIdx = 0xffff; }
    void resetArcNo() { mArcNo = 0xffff; }
    bool checkNoSetArcNo() const { return mArcNo == 0xFFFF; }
    void setBufferSize(u32 size) { mBufferSize = size; }
    void setBuffer(u8* buf) { mBuffer = buf; }
    u32 getBufferSize() { return mBufferSize; }
    u8* getBuffer() { return mBuffer; }
    bool checkNoSetIdx() const { return mIdx == 0xFFFF; }
    bool checkNoSetPriIdx() const { return mPriIdx == 0xFFFF; }
    u16 getArcNo() const { return mArcNo; }
    void setIdx(u16 i_idx) { mIdx = i_idx; }

private:
    /* 0x00 */ u16 mIdx;
    /* 0x02 */ u16 mPriIdx;
    /* 0x04 */ u16 mArcNo;
    /* 0x06 */ u16 field_0x06;
    /* 0x08 */ u32 mBufferSize;
    /* 0x0C */ u8* mBuffer;
    /* 0x10 */ JKRSolidHeap* mAnimeHeap;
};  // Size = 0x14

class daPy_actorKeep_c {
public:
    daPy_actorKeep_c() { clearData(); }

    void setActor();
    void setData(fopAc_ac_c*);
    void clearData();

    fpc_ProcID getID() const { return mID; }
    void setID(fpc_ProcID id) { mID = id; }
    fopAc_ac_c* getActor() const { return mActor; }
    fopAc_ac_c* getActorConst() const { return mActor; }

private:
    /* 0x0 */ fpc_ProcID mID;
    /* 0x4 */ fopAc_ac_c* mActor;
};  // Size: 0x8

class daPy_frameCtrl_c : public J3DFrameCtrl {
public:
    virtual ~daPy_frameCtrl_c() {}
    daPy_frameCtrl_c() {}
    bool checkAnmEnd();
    void updateFrame();
    void setFrameCtrl(u8, short, short, f32, f32);

    u16 getEndFlg() { return mEndFlg; }
    u16 getNowSetFlg() { return mNowSetFlg; }
    void onEndFlg() { mEndFlg = 1; }
    void onNowSetFlg() { mNowSetFlg = 1; }
    void offNowSetFlg() { mNowSetFlg = 0; }
    void offEndFlg() {
        mEndFlg = 0;
        mNowSetFlg = 0;
    }

private:
    /* 0x14 */ u16 mEndFlg;
    /* 0x16 */ u16 mNowSetFlg;
};

class Z2WolfHowlMgr;
class daBoomerang_c;

class daPy_demo_c {
public:
    enum {
        DEMO_UNK_0_e,
        DEMO_UNK_1_e,
        DEMO_UNK_2_e,
        DEMO_UNK_3_e,
        DEMO_UNK_4_e,
        DEMO_WAIT_TURN_e,
        DEMO_UNK_6_e,
        DEMO_UNK_7_e,
        DEMO_UNK_8_e,
        DEMO_UNK_9_e,
        DEMO_OPEN_TREASURE_e,
        DEMO_GET_ITEM_e,
        DEMO_UNEQUIP_e,
        DEMO_GRAB_PUT_e,
        DEMO_UNK_14_e,
        DEMO_UNK_15_e,
        DEMO_UNK_16_e,
        DEMO_UNK_17_e,
        DEMO_UNK_18_e,
        DEMO_MONKEY_MOVE_e,
        DEMO_LOOK_AROUND_e,
        DEMO_UNK_21_e,
        DEMO_UNK_22_e,
        DEMO_UNK_23_e,
        DEMO_UNK_24_e,
        DEMO_TURN_BACK_e,
        DEMO_UNK_26_e,
        DEMO_UNK_27_e,
        DEMO_BOOMERANG_CATCH_e,
        DEMO_HAWK_CATCH_e,
        DEMO_SWORD_UNEQUIP_SP_e,
        DEMO_UNK_31_e,
        DEMO_PUSH_PULL_WAIT_e,
        DEMO_PUSH_MOVE_e,
        DEMO_BOSS_ATN_WAIT_e,
        DEMO_DOOR_OPEN_e,
        DEMO_UNK_36_e,
        DEMO_TRADE_ITEM_OUT_e,
        DEMO_UNK_38_e,
        DEMO_KANDELAAR_SWING_e,
        DEMO_FRONT_ROLL_e,
        DEMO_CROUCH_e,
        DEMO_UNK_42_e,
        DEMO_UNK_43_e,
        DEMO_UNK_44_e,
        DEMO_UNK_45_e,
        DEMO_CAUGHT_e,
        DEMO_LOOK_UP_e,
        DEMO_LOOK_UP_TO_GET_ITEM_e,
        DEMO_HAND_PAT_e,
        DEMO_WOLF_MIDNA_RIDE_SHOCK_e,
        DEMO_SUMOU_SHIKO_e,
        DEMO_FOG_DEAD_e,
        DEMO_WOLF_SMELL_WAIT_e,
        DEMO_UNK_54_e,
        DEMO_UNK_55_e,
        DEMO_WOLF_CARGO_CARRY_e,
        DEMO_METAMORPHOSE_UNK1_e,
        DEMO_METAMORPHOSE_UNK2_e,
        DEMO_HORSE_GET_KEY_e,
        DEMO_NOD_e,
        DEMO_GLARE_e,
        DEMO_EYE_AWAY_e,
        DEMO_GOAT_STOP_READY_e,
        DEMO_GET_READY_SIT_e,
        DEMO_UNK_65_e,
        DEMO_TWGATE_e,
        DEMO_FM_CHAIN_STRONG_PULL_e,
        DEMO_WOLF_SNOW_ESCAPE_e,
        DEMO_ZORA_MOVE_e,
        DEMO_METAMORPHOSE_ONLY_UNK1_e,
        DEMO_METAMORPHOSE_ONLY_UNK2_e,
        DEMO_LOOK_AROUND_TURN_e,
        DEMO_UNK_73_e,
        DEMO_QUAKE_INIT_e,
        DEMO_GUARD_ATTACK_e,
        DEMO_SWORD_READY_e,
        DEMO_DUNGEON_WARP_e,
        DEMO_DUNGEON_WARP_SCENE_START_e,
        DEMO_MASTER_SWORD_STICK_e,
        DEMO_MASTER_SWORD_PULL_e,
        DEMO_CUT_DOWN_e,
        DEMO_UNK_82_e,
        DEMO_CUT_HEAD_e,
        DEMO_UNK_84_e,
        DEMO_UNK_85_e,
        DEMO_CUT_LARGE_JUMP_e,
        DEMO_CUT_FAST_READY_e,
        DEMO_COPY_ROD_REVIVE_e,
        DEMO_SWORD_PUSH_e,
        DEMO_GANON_FINISH_e,
        DEMO_UNK_91_e,
        DEMO_UNK_92_e,
        DEMO_HORSE_LOOK_DOWN_e,
        DEMO_UNK_94_e,
        DEMO_LAST_e,

        DEMO_NEW_ANM0_e = 0x200,
    };

    enum {
        DEMO_TYPE_TOOL_e = 1,
        DEMO_TYPE_SYSTEM_e,
        DEMO_TYPE_ORIGINAL_e,
        DEMO_TYPE_START_e,
        DEMO_TYPE_SPECIAL_e,
    };

    void setDemoType(u16 pType) { mDemoType = pType; }
    int getDemoType() const { return mDemoType; }
    void setDemoMode(u32 mode) { mDemoMode = mode; }
    u32 getDemoMode() const { return mDemoMode; }
    int getParam1() const { return mParam1; }
    s16 getParam2() const { return mParam2; }
    void setOriginalDemoType() { setDemoType(DEMO_TYPE_ORIGINAL_e); }
    void setSpecialDemoType() { setDemoType(DEMO_TYPE_SPECIAL_e); }
    void setSystemDemoType() { setDemoType(DEMO_TYPE_SYSTEM_e); }
    void setStick(f32 stick) { mStick = stick; }
    void setMoveAngle(s16 angle) { mDemoMoveAngle = angle; }
    s16 getMoveAngle() const { return mDemoMoveAngle; }
    f32 getStick() { return mStick; }
    int getParam0() const { return mParam0; }
    void setParam0(int value) { mParam0 = value; }
    void setParam1(int value) { mParam1 = value; }
    void setParam2(s16 value) { mParam2 = value; }
    void setPos0(const cXyz* pos) { mDemoPos0 = *pos; }
    void setToolDemoType() { setDemoType(DEMO_TYPE_TOOL_e); }
    s16 getTimer() const { return mTimer; }
    void decTimer() { mTimer--; }
    void setTimer(s16 i_timer) { mTimer = i_timer; }
    cXyz* getPos0() { return &mDemoPos0; }
    void resetDemoType() { setDemoType(0); }
    void setStartDemoType() { setDemoType(DEMO_TYPE_START_e); }

private:
    /* 0x00 */ u16 mDemoType;
    /* 0x02 */ s16 mDemoMoveAngle;
    /* 0x04 */ s16 mTimer;
    /* 0x06 */ s16 mParam2;
    /* 0x08 */ int mParam0;
    /* 0x0C */ int mParam1;
    /* 0x10 */ u32 mDemoMode;
    /* 0x14 */ f32 mStick;
    /* 0x18 */ cXyz mDemoPos0;
};  // Size: 0x24

class daMidna_c;
class daSpinner_c;
class daPy_py_c;
inline daPy_py_c* dComIfGp_getLinkPlayer();
inline BOOL dComIfGs_isEventBit(const u16);
inline u32 dComIfGs_getLastSceneMode();

class daPy_py_c : public fopAc_ac_c {
public:
    /* 0x0568 */ u8 mCutType;
    /* 0x0569 */ u8 mComboCutCount;
    /* 0x056A */ u8 mSpecialMode;  // maybe needs better name
    /* 0x056B */ u8 field_0x56b;
    /* 0x056C */ s16 mDamageTimer;
    /* 0x056E */ u16 mSwordUpTimer;
    /* 0x0570 */ u32 mNoResetFlg0;
    /* 0x0574 */ u32 mNoResetFlg1;
    /* 0x0578 */ u32 mNoResetFlg2;
    /* 0x057C */ u32 mNoResetFlg3;
    /* 0x0580 */ u32 mResetFlg0;
    /* 0x0584 */ u32 mResetFlg1;
    /* 0x0588 */ u32 mEndResetFlg0;
    /* 0x058C */ u32 mEndResetFlg1;
    /* 0x0590 */ u32 mEndResetFlg2;
    /* 0x0594 */ f32 field_0x594;
    /* 0x0598 */ f32 field_0x598;
    /* 0x059C */ csXyz mBodyAngle;
    /* 0x05A4 */ cXyz mHeadTopPos;
    /* 0x05B0 */ cXyz mItemPos;
    /* 0x05BC */ cXyz mSwordTopPos;
    /* 0x05C8 */ cXyz mLeftHandPos;
    /* 0x05D4 */ cXyz mRightHandPos;
    /* 0x05E0 */ cXyz mLeftFootPos;
    /* 0x05EC */ cXyz mRightFootPos;
    /* 0x05F8 */ cXyz field_0x5f8;
    /* 0x0604 */ daPy_demo_c mDemo;

public:
    enum daPy_FLG0 {
        FLG0_UNK_40000000 = 0x40000000,
        FLG0_EQUIP_HVY_BOOTS = 0x2000000,
        FLG0_PLAYER_NO_DRAW = 0x8000000,
        FLG0_UNK_10000000 = 0x10000000,
        FLG0_UNK_1000000 = 0x1000000,
        FLG0_UNDERWATER = 0x800000,
        FLG0_UNK_400000 = 0x400000,
        FLG0_UNK_200000 = 0x200000,
        FLG0_UNK_100000 = 0x100000,
        FLG0_UNK_80000 = 0x80000,
        FLG0_UNK_40000 = 0x40000,
        FLG0_DK_CAUGHT = 0x20000,
        FLG0_UNK_10000 = 0x10000,
        FLG0_UNK_8000 = 0x8000,
        FLG0_UNK_4000 = 0x4000,
        FLG0_UNK_2000 = 0x2000,
        FLG0_MAGNE_BOOTS_ON = 0x1000,
        FLG0_PUSH_PULL_KEEP = 0x800,
        FLG0_UNK_400 = 0x400,
        FLG0_SWIM_UP = 0x100,
        FLG0_UNK_80 = 0x80,
        FLG0_UNK_40 = 0x40,
        FLG0_UNK_20 = 0x20,
        FLG0_UNK_10 = 0x10,
        FLG0_UNK_8 = 0x8,
        FLG0_MIDNA_RIDE = 4,
        FLG0_UNK_2 = 2,

        FLG0_HVY_STATE = FLG0_UNK_40000000 | FLG0_EQUIP_HVY_BOOTS | FLG0_DK_CAUGHT,
        FLG0_UNK_14000 = 0x14000,
        FLG0_UNK_10400 = 0x10400,
        FLG0_UNK_18 = FLG0_UNK_10 | FLG0_UNK_8,
    };

    enum daPy_FLG1 {
        FLG1_UNK_80000000 = 0x80000000,
        FLG1_ICE_FREEZE = 0x40000000,
        FLG1_UNK_20000000 = 0x20000000,
        FLG1_UNK_10000000 = 0x10000000,
        FLG1_UNK_4000000 = 0x4000000,
        FLG1_IS_WOLF = 0x2000000,
        FLG1_UNK_1000000 = 0x1000000,
        FLG1_UNK_800000 = 0x800000,
        FLG1_DASH_MODE = 0x400000,
        FLG1_UNK_200000 = 0x200000,
        FLG1_UNK_100000 = 0x100000,
        FLG1_MIDNA_ATN_POS = 0x80000,
        FLG1_UNK_40000 = 0x40000,
        FLG1_UNK_20000 = 0x20000,
        FLG1_UNK_10000 = 0x10000,
        FLG1_UNK_8000 = 0x8000,
        FLG1_THROW_DAMAGE = 0x4000,
        FLG1_UNK_2000 = 0x2000,
        FLG1_UNK_1000 = 0x1000,
        FLG1_UNK_800 = 0x800,
        FLG1_UNK_200 = 0x200,
        FLG1_MIDNA_HAIR_ATN_POS = 0x100,
        FLG1_UNK_80 = 0x80,
        FLG1_UNK_40 = 0x40,
        FLG1_UNK_20 = 0x20,
        FLG1_UNK_10 = 0x10,
        FLG1_UNK_8 = 8,
        FLG1_UNK_4 = 4,
        FLG1_UNK_2 = 2,
        FLG1_DK_CAUGHT = 1,

        FLG1_UNK_1800 = 0x1800,
        FLG1_UNK_10100000 = FLG1_UNK_10000000 | FLG1_UNK_100000,
    };

    enum daPy_FLG2 {
        FLG2_HORSE_ZELDA = 0x40000000,
        FLG2_UNK_20000000 = 0x20000000,
        FLG2_UNK_10000000 = 0x10000000,
        FLG2_UNK_4080000 = 0x4080000,
        FLG2_UNK_4000000 = 0x4000000,
        FLG2_UNK_2080000 = 0x2080000,
        FLG2_UNK_2000000 = 0x2000000,
        FLG2_UNK_8000000 = 0x8000000,
        FLG2_UNK_1000000 = 0x1000000,
        FLG2_BOAR_SINGLE_BATTLE = 0x800000,
        FLG2_STATUS_WINDOW_DRAW = 0x400000,
        FLG2_UNK_280000 = 0x280000,
        FLG2_UNK_200000 = 0x200000,
        FLG2_UNK_100000 = 0x100000,
        FLG2_UNK_80000 = 0x80000,
        FLG2_FORCE_GAMEOVER = 0x40000,
        FLG2_UNK_20000 = 0x20000,
        FLG2_WOLF_DASH_AUTO_JUMP = 0x10000,
        FLG2_SCN_CHG_START = 0x8000,
        FLG2_UNK_4000 = 0x4000,
        FLG2_UNK_2000 = 0x2000,
        FLG2_PRESSED_DAMAGE = 0x1000,
        FLG2_UNK_800 = 0x800,
        FLG2_PLAYER_SHADOW = 0x400,
        FLG2_UNK_200 = 0x200,
        FLG2_UNK_100 = 0x100,
        FLG2_UNK_80 = 0x80,
        FLG2_WOLF_ENEMY_HANG_BITE = 0x40,
        FLG2_WOLF_ENEMY_LEFT_THROW = 0x20,
        FLG2_UNK_10 = 0x10,
        FLG2_UNK_8 = 8,
        FLG2_UNK_4 = 4,
        FLG2_UNK_2 = 2,
        FLG2_UNK_1 = 1,

        FLG2_UNK_58 = FLG2_WOLF_ENEMY_HANG_BITE | FLG2_UNK_10 | FLG2_UNK_8,
        FLG2_UNK_10000001 = FLG2_UNK_10000000 | FLG2_UNK_1,
    };

    enum daPy_FLG3 {
        FLG3_UNK_80000000 = 0x80000000,
        FLG3_UNK_40000000 = 0x40000000,
        FLG3_UNK_20000000 = 0x20000000,
        FLG3_COPY_ROD_ATN_KEEP = 0x10000000,
        FLG3_BOOMERANG_ATN_KEEP = 0x8000000,
        FLG3_UNK_4000000 = 0x4000000,
        FLG3_WARP_OBJ_DEMO = 0x2000000,
        FLG3_UNK_1000000 = 0x1000000,
        FLG3_UNK_800000 = 0x800000,
        FLG3_UNK_400000 = 0x400000,
        FLG3_UNK_200000 = 0x200000,
        FLG3_UNK_100000 = 0x100000,
        FLG3_UNK_80000 = 0x80000,
        FLG3_COPY_ROD_THROW_AFTER = 0x40000,
        FLG3_MIDNA_TALK_POLY_SPEED = 0x10000,
        FLG3_UNK_8000 = 0x8000,
        FLG3_UNK_4000 = 0x4000,
        FLG3_UNK_2000 = 0x2000,
        FLG3_UNK_1000 = 0x1000,
        FLG3_UNK_800 = 0x800,
        FLG3_UNK_400 = 0x400,
        FLG3_UNK_200 = 0x200,
        FLG3_UNK_100 = 0x100,
        FLG3_UNK_80 = 0x80,
        FLG3_UNK_40 = 0x40,
        FL3_TRANING_CUT_BACK = 0x20,
        FLG3_UNK_10 = 0x10,
        FLG3_UNK_8 = 8,
        FLG3_UNK_4 = 4,
        FLG3_UNK_1 = 1,
    };

    enum daPy_ERFLG0 {
        ERFLG0_FORCE_SUBJECT_CANCEL = 0x80000000,
        ERFLG0_UNK_40000000 = 0x40000000,
        ERFLG0_UNK_20000000 = 0x20000000,
        ERFLG0_WOLF_FCHAIN_PULL = 0x10000000,
        ERFLG0_UNK_8000000 = 0x8000000,
        ERFLG0_UNK_4000000 = 0x4000000,
        ERFLG0_BOSS_ROOM_WAIT = 0x2000000,
        ERFLG0_ENEMY_DEAD = 0x1000000,
        ERFLG0_UNK_800000 = 0x800000,
        ERFLG0_BEE_FOLLOW = 0x400000,
        ERFLG0_UNK_200000 = 0x200000,
        ERFLG0_UNK_100000 = 0x100000,
        ERFLG0_SINGLE_BOAR_AVOID = 0x80000,
        ERFLG0_FISHING_ROD_GET_FISH = 0x40000,
        ERFLG0_UNK_20000 = 0x20000,
        ERFLG0_UNK_10000 = 0x10000,
        ERFLG0_UNK_4000 = 0x4000,
        ERFLG0_UNK_2000 = 0x2000,
        ERFLG0_UNK_1000 = 0x1000,
        ERFLG0_UNK_400 = 0x400,
        ERFLG0_FORCE_AUTO_JUMP = 0x200,
        ERFLG0_UNK_100 = 0x100,
        ERFLG0_UNK_80 = 0x80,
        ERFLG0_UNK_40 = 0x40,
        ERFLG0_NOT_AUTO_JUMP = 0x20,
        ERFLG0_NOT_HANG = 0x10,
        ERFLG0_UNK_8 = 8,
        ERFLG0_UNK_4 = 4,
        ERFLG0_UNK_2 = 2,
        ERFLG0_UNK_1 = 1,
    };

    enum daPy_ERFLG1 {
        ERFLG1_GANON_FINISH = 0x80000000,
        ERFLG1_UNK_40000000 = 0x40000000,
        ERFLG1_UNK_20000000 = 0x20000000,
        ERFLG1_UNK_10000000 = 0x10000000,
        ERFLG1_LARGE_DAMAGE_UP_STOP = 0x8000000,
        ERFLG1_UNK_4000000 = 0x4000000,
        ERFLG1_UNK_2000000 = 0x2000000,
        ERFLG1_UNK_1000000 = 0x1000000,
        ERFLG1_UNK_400000 = 0x400000,
        ERFLG1_UNK_200000 = 0x200000,
        ERFLG1_UNK_100000 = 0x100000,
        ERFLG1_UNK_80000 = 0x80000,
        ERFLG1_UNK_40000 = 0x40000,
        ERFLG1_UNK_20000 = 0x20000,
        ERFLG1_UNK_10000 = 0x10000,
        ERFLG1_UNK_8000 = 0x8000,
        ERFLG1_UNK_4000 = 0x4000,
        ERFLG1_UNK_2000 = 0x2000,
        ERFLG1_UNK_1000 = 0x1000,
        ERFLG1_UNK_800 = 0x800,
        ERFLG1_UNK_400 = 0x400,
        ERFLG1_WOLF_EYE_KEEP = 0x200,
        ERFLG1_UNK_100 = 0x100,
        ERFLG1_UNK_80 = 0x80,
        ERFLG1_MIDNA_RIDE_SHOCK = 0x40,
        ERFLG1_UNK_20 = 0x20,
        ERFLG1_UNK_10 = 0x10,
        ERFLG1_UNK_8 = 8,
        ERFLG1_UNK_4 = 4,
        ERFLG1_UNK_2 = 2,
        ERFLG1_UNK_1 = 1,
    };

    enum daPy_ERFLG2 {
        ERFLG2_UNK_100 = 0x100,
        ERFLG2_PORTAL_WARP_MIDNA_ATN_KEEP = 0x80,
        ERFLG2_UNK_40 = 0x40,
        ERFLG2_UNK_20 = 0x20,
        ERFLG2_UNK_10 = 0x10,
        ERFLG2_UNK_8 = 8,
        ERFLG2_UNK_2 = 2,
        ERFLG2_UNK_1 = 1,
    };

    enum daPy_RFLG0 {
        RFLG0_WOLF_BARK = 0x80000000,
        RFLG0_UNK_40000000 = 0x40000000,
        RFLG0_UNK_20000000 = 0x20000000,
        RFLG0_UNK_10000000 = 0x10000000,
        RFLG0_UNK_8000000 = 0x8000000,
        RFLG0_UNK_4000000 = 0x4000000,
        RFLG0_UNK_2000000 = 0x2000000,
        RFLG0_GRAB_PUT_START = 0x400000,
        RFLG0_WOLF_GROWL = 0x200000,
        RFLG0_UNK_80000 = 0x80000,
        RFLG0_UNK_40000 = 0x40000,
        RFLG0_UNK_20000 = 0x20000,
        RFLG0_UNK_10000 = 0x10000,
        RFLG0_UNK_8000 = 0x8000,
        RFLG0_UNK_4000 = 0x4000,
        RFLG0_FRONT_ROLL_CRASH = 0x2000,
        RFLG0_ENEMY_ATTN_LOCK = 0x1000,
        RFLG0_UNK_400 = 0x400,
        RFLG0_COW_GAME_LEASH = 0x200,
        RFLG0_UNK_100 = 0x100,
        RFLG0_UNK_80 = 0x80,
        RFLG0_UNK_40 = 0x40,
        RFLG0_GRAB_UP_END = 0x20,
        RFLG0_UNK_10 = 0x10,
        RFLG0_UNK_8 = 0x8,
        RFLG0_GRAB_THROW = 0x4,
        RFLG0_UNK_2 = 0x2,
        RFLG0_UNK_1 = 0x1,
    };

    enum daPy_RFLG1 {
        RFLG1_DAMAGE_IMPACT = 0x400,
        RFLG1_UNK_200 = 0x200,
        RFLG1_UNK_100 = 0x100,
        RFLG1_UNK_80 = 0x80,
        RFLG1_UNK_40 = 0x40,
        RFLG1_UNK_30 = 0x30,
        RFLG1_UNK_20 = 0x20,
        RFLG1_UNK_10 = 0x10,
        RFLG1_UNK_8 = 0x8,
        RFLG1_UNK_4 = 0x4,
        RFLG1_UNK_2 = 0x2,
        RFLG1_WOLF_ATTACK_REVERSE = 0x1,

        RFLG1_UNK_C = 0xC,
    };

    enum {
        /* 0x01 */ SMODE_SUMO_READY = 1,
        /* 0x25 */ SMODE_SUMO_LOSE = 37,
        /* 0x27 */ SMODE_WOLF_PUZZLE = 39,
        /* 0x2A */ SMODE_GOAT_STOP = 42,
        /* 0x2B */ SMODE_GORON_THROW,
        /* 0x2C */ SMODE_CARGO_CARRY,
    };

    enum CutType {
        /* 0x00 */ CUT_TYPE_NONE,
        /* 0x01 */ CUT_TYPE_NM_VERTICAL,
        /* 0x02 */ CUT_TYPE_NM_STAB,
        /* 0x03 */ CUT_TYPE_NM_RIGHT,
        /* 0x04 */ CUT_TYPE_NM_LEFT,
        /* 0x05 */ CUT_TYPE_HEAD_JUMP,  // Helm Splitter
        /* 0x06 */ CUT_TYPE_FINISH_LEFT,
        /* 0x07 */ CUT_TYPE_FINISH_VERTICAL,
        /* 0x08 */ CUT_TYPE_TURN_RIGHT,
        /* 0x09 */ CUT_TYPE_UNK_9,
        /* 0x0A */ CUT_TYPE_JUMP,
        /* 0x0B */ CUT_TYPE_DASH_UNK_B,
        /* 0x0C */ CUT_TYPE_DASH_UNK_C,
        /* 0x0D */ CUT_TYPE_DASH_UNK_D,
        /* 0x0E */ CUT_TYPE_UNK_E,
        /* 0x0F */ CUT_TYPE_UNK_F,
        /* 0x10 */ CUT_TYPE_AIR,
        /* 0x11 */ CUT_TYPE_DASH_UNK_11,
        /* 0x12 */ CUT_TYPE_LARGE_JUMP_INIT,
        /* 0x13 */ CUT_TYPE_LARGE_JUMP,
        /* 0x14 */ CUT_TYPE_LARGE_JUMP_FINISH,
        /* 0x15 */ CUT_TYPE_FINISH_RIGHT,
        /* 0x16 */ CUT_TYPE_TURN_LEFT,
        /* 0x17 */ CUT_TYPE_LARGE_TURN_LEFT,
        /* 0x18 */ CUT_TYPE_LARGE_TURN_RIGHT,
        /* 0x19 */ CUT_TYPE_UNK_19,
        /* 0x1A */ CUT_TYPE_MORTAL_DRAW_A,
        /* 0x1B */ CUT_TYPE_UNK_1B,
        /* 0x1C */ CUT_TYPE_UNK_1C,
        /* 0x1D */ CUT_TYPE_UNK_1D,
        /* 0x1E */ CUT_TYPE_TWIRL,  // Back Slice
        /* 0x1F */ CUT_TYPE_MORTAL_DRAW_B,
        /* 0x20 */ CUT_TYPE_FINISH_STAB,
        /* 0x21 */ CUT_TYPE_COMBO_STAB,
        /* 0x22 */ CUT_TYPE_HORSE_UNK_22,
        /* 0x23 */ CUT_TYPE_HORSE_UNK_23,
        /* 0x24 */ CUT_TYPE_UNK_24,
        /* 0x25 */ CUT_TYPE_DASH_UNK_25,
        /* 0x26 */ CUT_TYPE_DASH_UNK_26,
        /* 0x27 */ CUT_TYPE_DOWN,
        /* 0x28 */ CUT_TYPE_UNK_28,
        /* 0x29 */ CUT_TYPE_GUARD_ATTACK,
        /* 0x2A */ CUT_TYPE_HORSE_UNK_2A,
        /* 0x2B */ CUT_TYPE_HORSE_TURN,
        /* 0x2C */ CUT_TYPE_WOLF_B_LEFT,
        /* 0x2D */ CUT_TYPE_WOLF_B_RIGHT,
        /* 0x2E */ CUT_TYPE_WOLF_B_FRONT,
        /* 0x2F */ CUT_TYPE_WOLF_B_BACK,
        /* 0x30 */ CUT_TYPE_WOLF_UNK_30,
        /* 0x31 */ CUT_TYPE_WOLF_JUMP_S,
        /* 0x32 */ CUT_TYPE_WOLF_JUMP_S_FINISH,
        /* 0x33 */ CUT_TYPE_WOLF_TURN_LEFT,
        /* 0x34 */ CUT_TYPE_WOLF_TURN_RIGHT,
        /* 0x35 */ CUT_TYPE_WOLF_UNK_35,
        /* 0x36 */ CUT_TYPE_WOLF_LOCK,
        /* 0x37 */ CUT_TYPE_WOLF_UNK_37,
        /* 0x38 */ CUT_TYPE_DASH_UNK_38,
        /* 0x39 */ CUT_TYPE_WOLF_JUMP,
    };

    static u32 setParamData(int, int, int, int);
    static BOOL checkFishingRodItem(int);
    static BOOL checkBombItem(int);
    static BOOL checkBottleItem(int);
    static BOOL checkDrinkBottleItem(int);
    static BOOL checkOilBottleItem(int);
    static BOOL checkOpenBottleItem(int);
    static BOOL checkBowItem(int);
    static BOOL checkHookshotItem(int);
    static BOOL checkTradeItem(int);
    static BOOL checkDungeonWarpItem(int);
    static BOOL checkMasterSwordEquip();
    static BOOL checkWoodShieldEquip();
    static f32 getAttentionOffsetY();
    static int checkNowWolfEyeUp();
    static void forceRestartRoom(int, u32, int);
    static BOOL setFmChainPos(fopAc_ac_c*, cXyz*, int);
    static void cancelFmChainGrab();
    static void setLookPos(cXyz*);
    static void setPlayerSe(u32);
    static bool linkGrabSubjectNoDraw(fopAc_ac_c*);
    static bool wolfGrabSubjectNoDraw(fopAc_ac_c*);
    static bool checkRoomRestartStart();
    static u32 checkCarryStartLightBallA();
    static u32 checkCarryStartLightBallB();
    f32 getSpinnerRideSpeed() const;
    bool checkSpinnerReflectEffect();
    static bool checkBoomerangCharge();
    static bool checkBoomerangChargeTime();
    static daBoomerang_c* getThrowBoomerangActor();
    static void cancelBoomerangLockActor(fopAc_ac_c*);
    static void setPlayerDamage(int, int);
    static void setMidnaMotionNum(int);
    static void setMidnaFaceNum(int);
    static BOOL checkShieldGet() { return dComIfGs_getSelectEquipShield() != fpcNm_ITEM_NONE; }
    static BOOL checkSwordGet() { return dComIfGs_getSelectEquipSword() != fpcNm_ITEM_NONE; }

    cXyz getHeadTopPos() const { return mHeadTopPos; }
    u32 checkThrowDamage() const { return checkNoResetFlg1(FLG1_THROW_DAMAGE); }
    bool checkGoronSideMove() const { return mSpecialMode == 0x2B; }
    cXyz* getRightFootPosP() { return &mRightFootPos; }
    cXyz* getLeftFootPosP() { return &mLeftFootPos; }
    cXyz getLeftFootPos() const { return mLeftFootPos; }
    cXyz getRightFootPos() const { return mRightFootPos; }
    u32 checkCopyRodThrowAfter() const { return checkNoResetFlg3(FLG3_COPY_ROD_THROW_AFTER); }
    u32 checkRide() const { return checkHorseRide() || checkBoarRide() || checkSpinnerRide() || checkCanoeRide() || checkBoardRide(); }
    cXyz getRightHandPos() const { return mRightHandPos; }
    const cXyz getLeftHandPos() const { return mLeftHandPos; }
    const cXyz getItemPos() const { return mItemPos; }
    BOOL getDkCaught() const { return checkNoResetFlg1(FLG1_DK_CAUGHT); }
    BOOL getDkCaught2() const { return checkNoResetFlg0(FLG0_DK_CAUGHT); }
    BOOL getStCaught() const { return checkNoResetFlg1(FLG1_UNK_80000000); }

    /* vt 0X008 */ virtual cXyz* getMidnaAtnPos() const { return NULL; }
    /* vt 0X00C */ virtual void setMidnaMsgNum(fopAc_ac_c*, u16) {}
    /* vt 0X010 */ virtual MtxP getModelMtx() { return cullMtx; }
    /* vt 0X014 */ virtual MtxP getInvMtx() { return cullMtx; }
    /* vt 0X018 */ virtual cXyz* getShadowTalkAtnPos() { return &current.pos; }
    /* vt 0X01C */ virtual f32 getGroundY() = 0;
    /* vt 0X020 */ virtual MtxP getLeftItemMatrix() { return cullMtx; }
    /* vt 0X024 */ virtual MtxP getRightItemMatrix() { return cullMtx; }
    /* vt 0X028 */ virtual MtxP getLeftHandMatrix() { return cullMtx; }
    /* vt 0X02C */ virtual MtxP getRightHandMatrix() { return cullMtx; }
    /* vt 0X030 */ virtual MtxP getLinkBackBone1Matrix() { return cullMtx; }
    /* vt 0X034 */ virtual MtxP getWolfMouthMatrix() { return cullMtx; }
    /* vt 0X038 */ virtual MtxP getWolfBackbone2Matrix() { return cullMtx; }
    /* vt 0X03C */ virtual MtxP getBottleMtx() { return NULL; }
    /* vt 0X040 */ virtual BOOL checkPlayerGuard() const { return FALSE; }
    /* vt 0X044 */ virtual u32 checkPlayerFly() const { return 0; }
    /* vt 0X048 */ virtual BOOL checkFrontRoll() const { return FALSE; }
    /* vt 0X04C */ virtual BOOL checkWolfDash() const { return FALSE; }
    /* vt 0X050 */ virtual BOOL checkAutoJump() const { return FALSE; }
    /* vt 0X054 */ virtual bool checkSideStep() const { return FALSE; }
    /* vt 0X058 */ virtual bool checkWolfTriggerJump() const { return FALSE; }
    /* vt 0X05C */ virtual BOOL checkGuardBreakMode() const { return FALSE; }
    /* vt 0X060 */ virtual bool checkLv3Slide() const { return FALSE; }
    /* vt 0X064 */ virtual bool checkWolfHowlDemoMode() const { return FALSE; }
    /* vt 0X068 */ virtual bool checkChainBlockPushPull() { return FALSE; }
    /* vt 0X06C */ virtual BOOL checkElecDamage() const { return FALSE; }
    /* vt 0X070 */ virtual BOOL checkEmptyBottleSwing() const { return FALSE; }
    /* vt 0X074 */ virtual BOOL checkBottleSwingMode() const { return FALSE; }
    /* vt 0X078 */ virtual BOOL checkHawkWait() const { return FALSE; }
    /* vt 0X07C */ virtual BOOL checkGoatThrow() const { return FALSE; }
    /* vt 0X080 */ virtual BOOL checkGoatThrowAfter() const { return FALSE; }
    /* vt 0X084 */ virtual BOOL checkWolfTagLockJump() const { return FALSE; }
    /* vt 0X088 */ virtual BOOL checkWolfTagLockJumpLand() const { return FALSE; }
    /* vt 0X08C */ virtual BOOL checkWolfRope() { return FALSE; }
    /* vt 0X090 */ virtual BOOL checkWolfRopeHang() const { return FALSE; }
    /* vt 0X094 */ virtual BOOL checkRollJump() const { return FALSE; }
    /* vt 0X098 */ virtual BOOL checkGoronRideWait() const { return FALSE; }
    /* vt 0X09C */ virtual BOOL checkWolfChain() const { return FALSE; }
    /* vt 0X0A0 */ virtual BOOL checkWolfWait() const { return FALSE; }
    /* vt 0X0A4 */ virtual BOOL checkWolfJumpAttack() const { return FALSE; }
    /* vt 0X0A8 */ virtual BOOL checkWolfRSit() const { return FALSE; }
    /* vt 0X0AC */ virtual bool checkBubbleFly() const { return FALSE; }
    /* vt 0X0B0 */ virtual BOOL checkBottleDrinkEnd() const { return FALSE; }
    /* vt 0X0B4 */ virtual BOOL checkWolfDig() const { return FALSE; }
    /* vt 0X0B8 */ virtual BOOL checkCutCharge() const { return FALSE; }
    /* vt 0X0BC */ virtual BOOL checkCutTurnCharge() const { return FALSE; }
    /* vt 0X0C0 */ virtual BOOL checkCutLargeJumpCharge() const { return FALSE; }
    /* vt 0X0C4 */ virtual BOOL getBokoFlamePos(cXyz*) { return FALSE; }
    /* vt 0X0C8 */ virtual BOOL checkComboCutTurn() const { return FALSE; }
    /* vt 0X0CC */ virtual BOOL checkClimbMove() const { return FALSE; }
    /* vt 0X0D0 */ virtual BOOL checkGrassWhistle() const { return FALSE; }
    /* vt 0X0D4 */ virtual BOOL checkBoarRun() const { return FALSE; }
    /* vt 0X0D8 */ virtual bool checkFmChainPut() const { return FALSE; }
    /* vt 0X0DC */ virtual BOOL checkHorseElecDamage() const { return FALSE; }
    /* vt 0X0E0 */ virtual f32 getBaseAnimeFrameRate() const { return 1.0f; }
    /* vt 0X0E4 */ virtual f32 getBaseAnimeFrame() const { return 0.0f; }
    /* vt 0X0E8 */ virtual void setAnimeFrame(f32) {}
    /* vt 0X0EC */ virtual BOOL checkWolfLock(fopAc_ac_c*) const { return FALSE; }
    /* vt 0X0F0 */ virtual bool cancelWolfLock(fopAc_ac_c*) { return FALSE; }
    /* vt 0X0F4 */ virtual fpc_ProcID getAtnActorID() const { return fpcM_ERROR_PROCESS_ID_e; }
    /* vt 0X0F8 */ virtual fpc_ProcID getItemID() const { return fpcM_ERROR_PROCESS_ID_e; }
    /* vt 0X0FC */ virtual fpc_ProcID getGrabActorID() const { return fpcM_ERROR_PROCESS_ID_e; }
    /* vt 0X100 */ virtual BOOL exchangeGrabActor(fopAc_ac_c*) { return FALSE; }
    /* vt 0X104 */ virtual BOOL setForceGrab(fopAc_ac_c*, int, int) { return FALSE; }
    /* vt 0X108 */ virtual void setForcePutPos(cXyz const&) {}
    /* vt 0X10C */ virtual u32 checkPlayerNoDraw() { return FALSE; }
    /* vt 0X110 */ virtual bool checkRopeTag() { return FALSE; }
    /* vt 0X114 */ virtual void voiceStart(u32) {}
    /* vt 0X118 */ virtual void seStartOnlyReverb(u32) {}
    /* vt 0X11C */ virtual void seStartOnlyReverbLevel(u32) {}
    /* vt 0X120 */ virtual void setOutPower(f32, short, int) {}
    /* vt 0X124 */ virtual void setGrabCollisionOffset(f32, f32, cBgS_PolyInfo*) {}
    /* vt 0X128 */ virtual void onMagneGrab(f32, f32) {}
    /* vt 0X12C */ virtual void onFrollCrashFlg(u8, int) {}
    /* vt 0X130 */ virtual MtxP getModelJointMtx(u16) { return NULL; }
    /* vt 0X134 */ virtual MtxP getHeadMtx() { return NULL; }
    /* vt 0X138 */ virtual bool setHookshotCarryOffset(fpc_ProcID, cXyz const*) { return FALSE; }
    /* vt 0X13C */ virtual BOOL checkCutJumpCancelTurn() const { return FALSE; }
    /* vt 0X140 */ virtual bool checkIronBallReturn() const { return FALSE; }
    /* vt 0X144 */ virtual bool checkIronBallGroundStop() const { return FALSE; }
    /* vt 0X148 */ virtual BOOL checkSingleBoarBattleSecondBowReady() const { return FALSE; }
    /* vt 0X14C */ virtual bool checkPointSubWindowMode() const { return FALSE; }
    /* vt 0X150 */ virtual void setClothesChange(int) {}
    /* vt 0X154 */ virtual void setPlayerPosAndAngle(cXyz const*, short, int) {}
    /* vt 0X158 */ virtual void setPlayerPosAndAngle(cXyz const*, csXyz const*) {}
    /* vt 0X15C */ virtual void setPlayerPosAndAngle(f32 (*)[4]) {}
    /* vt 0X160 */ virtual bool setThrowDamage(short, f32, f32, int, int, int) { return FALSE; }
    /* vt 0X164 */ virtual bool checkSetNpcTks(cXyz*, int, int) { return FALSE; }
    /* vt 0X168 */ virtual int setRollJump(f32, f32, short) { return FALSE; }
    /* vt 0X16C */ virtual void playerStartCollisionSE(u32, u32) {}
    /* vt 0X170 */ virtual void changeTextureAnime(u16, u16, int) {}
    /* vt 0X174 */ virtual void cancelChangeTextureAnime() {}
    /* vt 0X178 */ virtual void cancelDungeonWarpReadyNeck() {}
    /* vt 0X17C */ virtual void onSceneChangeArea(u8, u8, fopAc_ac_c*) {}
    /* vt 0X180 */ virtual void onSceneChangeAreaJump(u8, u8, fopAc_ac_c*) {}
    /* vt 0X184 */ virtual void onSceneChangeDead(u8, int) {}
    /* vt 0X188 */ virtual u32 checkHorseRide() const { return false; }
    /* vt 0X18C */ virtual u32 checkBoarRide() const { return 0; }
    /* vt 0X190 */ virtual u32 checkCanoeRide() const { return 0; }
    /* vt 0X194 */ virtual u32 checkBoardRide() const { return 0; }
    /* vt 0X198 */ virtual u32 checkSpinnerRide() const { return 0; }
    /* vt 0X19C */ virtual daSpinner_c* getSpinnerActor() { return NULL; }
    /* vt 0X1A0 */ virtual BOOL checkHorseRideNotReady() const { return FALSE; }
    /* vt 0X1A4 */ virtual bool checkArrowChargeEnd() const { return FALSE; }
    /* vt 0X1A8 */ virtual f32 getSearchBallScale() const { return 0.0f; }
    /* vt 0X1AC */ virtual int checkFastShotTime() { return FALSE; }
    /* vt 0X1B0 */ virtual bool checkNoEquipItem() const { return TRUE; }
    /* vt 0X1B4 */ virtual bool checkFireMaterial() const { return FALSE; }
    /* vt 0X1B8 */ virtual bool checkKandelaarSwing(int) const { return FALSE; }
    /* vt 0X1BC */ virtual s16 getBoardCutTurnOffsetAngleY() const { return 0; }
    /* vt 0X1C0 */ virtual cXyz* getMagneHitPos() { return &mSwordTopPos; }
    /* vt 0X1C4 */ virtual cXyz* getMagneBootsTopVec() { return &current.pos; }
    /* vt 0X1C8 */ virtual cXyz* getKandelaarFlamePos() { return NULL; }
    /* vt 0X1CC */ virtual bool checkUseKandelaar(int) { return FALSE; }
    /* vt 0X1D0 */ virtual void setDkCaught(fopAc_ac_c*) {}
    /* vt 0X1D4 */ virtual void onPressedDamage(cXyz const&, short) {}
    /* vt 0X1D8 */ virtual bool checkPriActorOwn(fopAc_ac_c const*) const { return FALSE; }
    /* vt 0X1DC */ virtual bool onWolfEnemyBiteAll(fopAc_ac_c*, daPy_FLG2) { return FALSE; }
    /* vt 0X1E0 */ virtual bool checkWolfEnemyBiteAllOwn(fopAc_ac_c const*) const { return FALSE; }
    /* vt 0X1E4 */ virtual void setWolfEnemyHangBiteAngle(short) {}
    /* vt 0X1E8 */ virtual void setKandelaarMtx(f32 (*)[4], int, int) {}
    /* vt 0X1EC */ virtual bool getStickAngleFromPlayerShape(short*) const { return FALSE; }
    /* vt 0X1F0 */ virtual bool checkSpinnerPathMove() { return FALSE; }
    /* vt 0X1F4 */ virtual bool checkSpinnerTriggerAttack() { return FALSE; }
    /* vt 0X1F8 */ virtual void onSpinnerPathForceRemove() {}
    /* vt 0X1FC */ virtual int getIronBallBgHit() const { return FALSE; }
    /* vt 0X200 */ virtual cXyz* getIronBallCenterPos() { return NULL; }
    /* vt 0X204 */ virtual bool checkCanoeFishingGetLeft() const { return FALSE; }
    /* vt 0X208 */ virtual bool checkCanoeFishingGetRight() const { return FALSE; }
    /* vt 0X20C */ virtual u8 checkBeeChildDrink() const { return FALSE; }
    /* vt 0X210 */ virtual void skipPortalObjWarp() {}
    /* vt 0X214 */ virtual BOOL checkTreasureRupeeReturn(int) const { return FALSE; }
    /* vt 0X218 */ virtual void setSumouReady(fopAc_ac_c*) {}
    /* vt 0X21C */ virtual bool checkAcceptDungeonWarpAlink(int) { return FALSE; }
    /* vt 0X220 */ virtual s16 getSumouCounter() const { return 0; }
    /* vt 0X224 */ virtual s16 checkSumouWithstand() const { return 0; }
    /* vt 0X228 */ virtual void cancelGoronThrowEvent() {}
    /* vt 0X22C */ virtual void setSumouGraspCancelCount(int) {}
    /* vt 0X230 */ virtual void setSumouPushBackDirection(short) {}
    /* vt 0X234 */ virtual void setSumouLoseHeadUp() {}
    /* vt 0X238 */ virtual s16 getGiantPuzzleAimAngle() const { return shape_angle.y; }
    /* vt 0X23C */ virtual void setGoronSideMove(fopAc_ac_c*) {}
    /* vt 0X240 */ virtual void setCargoCarry(fopAc_ac_c*) {}
    /* vt 0X244 */ virtual bool getDpdFarFlg() const { return 0; }
    /* vt 0X248 */ virtual cXyz* getHookshotTopPos() { return NULL; }
    /* vt 0X24C */ virtual bool checkHookshotReturnMode() const { return FALSE; }
    /* vt 0X250 */ virtual bool checkHookshotShootReturnMode() const { return FALSE; }
    /* vt 0X254 */ virtual bool checkOctaIealHang() const { return FALSE; }
    /* vt 0X258 */ virtual void cancelOctaIealHang() {}
    /* vt 0X25C */ virtual void cancelDragonHangBackJump() {}
    /* vt 0X260 */ virtual void setOctaIealWildHang() {}
    /* vt 0X264 */ virtual bool checkDragonHangRide() const { return FALSE; }
    /* vt 0X268 */ virtual void changeDragonActor(fopAc_ac_c*) {}
    /* vt 0X26C */ virtual u8 getClothesChangeWaitTimer() const { return 0; }
    /* vt 0X270 */ virtual u8 getShieldChangeWaitTimer() const { return 0; }
    /* vt 0X274 */ virtual u8 getSwordChangeWaitTimer() const { return 0; }
    /* vt 0X278 */ virtual BOOL checkMetamorphose() const { return FALSE; }
    /* vt 0X27C */ virtual BOOL checkWolfDownAttackPullOut() const { return FALSE; }
    /* vt 0X280 */ virtual BOOL checkBootsOrArmorHeavy() const { return FALSE; }
    /* vt 0X284 */ virtual fpc_ProcID getBottleOpenAppearItem() const { return fpcM_ERROR_PROCESS_ID_e; }
    /* vt 0X288 */ virtual bool checkItemSwordEquip() const { return FALSE; }
    /* vt 0X28C */ virtual f32 getSinkShapeOffset() const { return 0.0f; }
    /* vt 0X290 */ virtual BOOL checkSinkDead() const { return FALSE; }
    /* vt 0X294 */ virtual BOOL checkHorseStart() { return FALSE; }
    /* vt 0X298 */ virtual Z2WolfHowlMgr* getWolfHowlMgrP() { return NULL; }
    /* vt 0X29C */ virtual BOOL checkWolfHowlSuccessAnime() const { return FALSE; }
    /* vt 0X2A0 */ virtual BOOL checkCopyRodTopUse() { return FALSE; }
    /* vt 0X2A4 */ virtual bool checkCopyRodEquip() const { return FALSE; }
    /* vt 0X2A8 */ virtual BOOL checkCutJumpMode() const { return FALSE; }

    f32 getSpeedF() const { return speedF; }

    bool getSumouCameraMode() const {
        bool sumouCameraMode = false;
        if (mSpecialMode != 0 && mSpecialMode < 0x26) {
            sumouCameraMode = true;
        }
        return sumouCameraMode;
    }

    bool getSumouMode() const { return getSumouCameraMode(); }
    void cancelOriginalDemo() {
        mDemo.setSystemDemoType();
        mDemo.setDemoMode(1);
    }

    u32 getDemoMode() const { return mDemo.getDemoMode(); }

    u8 getCutCount() const { return mComboCutCount; }

    BOOL checkStatusWindowDraw() const { return checkNoResetFlg2(FLG2_STATUS_WINDOW_DRAW); }
    bool checkCargoCarry() const { return mSpecialMode == SMODE_CARGO_CARRY; }
    BOOL getHeavyStateAndBoots() { return checkNoResetFlg0(FLG0_HVY_STATE); }
    BOOL checkEnemyAttentionLock() const { return checkResetFlg0(RFLG0_ENEMY_ATTN_LOCK); }
    BOOL getGrabUpStart() const { return checkResetFlg0(RFLG0_UNK_8000); }
    bool checkCanoeSlider() const { return mSpecialMode == 0x2D; }
    bool checkGoatStopGame() const { return mSpecialMode == 0x2A; }
    void onGoatStopGame() { mSpecialMode = 0x2A; }
    u8 getCutType() const { return mCutType; }
    u16 getSwordAtUpTime() const { return mSwordUpTimer; }
    s16 getDamageWaitTimer() const { return mDamageTimer; }
    BOOL checkWaterInMove() const { return checkNoResetFlg0(FLG0_UNDERWATER); }
    BOOL checkSceneChangeAreaStart() const { return checkNoResetFlg2(FLG2_SCN_CHG_START); }
    BOOL checkFrontRollCrash() const { return checkResetFlg0(RFLG0_FRONT_ROLL_CRASH); }
    BOOL checkWolfAttackReverse() const { return checkResetFlg1(RFLG1_WOLF_ATTACK_REVERSE); }
    BOOL checkFreezeDamage() const { return checkNoResetFlg1(FLG1_ICE_FREEZE); }
    BOOL checkWolfTagLockJumpReady() const { return checkResetFlg0(RFLG0_UNK_20000); }
    BOOL checkDamageImpact() const { return checkResetFlg1(RFLG1_DAMAGE_IMPACT); }
    BOOL getGrabUpEnd() const { return checkResetFlg0(RFLG0_GRAB_UP_END); }
    BOOL getGrabPutStart() const { return checkResetFlg0(RFLG0_GRAB_PUT_START); }
    BOOL checkSwimUp() const { return checkNoResetFlg0(FLG0_SWIM_UP); }
    BOOL checkHorseZelda() const { return checkNoResetFlg2(FLG2_HORSE_ZELDA); }
    BOOL checkSpecialHorseRide() { return checkNoResetFlg2(daPy_FLG2(FLG2_HORSE_ZELDA | FLG2_UNK_1000000 | FLG2_BOAR_SINGLE_BATTLE)); }
    BOOL checkBoardNoFootAngle() const { return checkResetFlg1(RFLG1_UNK_40); }
    BOOL checkGrabThrow() const { return checkResetFlg0(RFLG0_GRAB_THROW); }
    BOOL checkMidnaAtnPos() const { return checkNoResetFlg1(FLG1_MIDNA_ATN_POS); }
    BOOL checkMidnaHairAtnPos() const { return checkNoResetFlg1(FLG1_MIDNA_HAIR_ATN_POS); }
    BOOL checkAttentionLock() const { return checkResetFlg0(RFLG0_UNK_10000); }
    BOOL checkFishingRodUseStart() const { return checkResetFlg1(RFLG1_UNK_80); }
    BOOL checkFishingRodUseAccept() const { return checkResetFlg1(RFLG1_UNK_200); }

    void onBossRoomWait() { onEndResetFlg0(ERFLG0_BOSS_ROOM_WAIT); }
    void onBeeFollow() { onEndResetFlg0(ERFLG0_BEE_FOLLOW); }
    void onForceAutoJump() { onEndResetFlg0(ERFLG0_FORCE_AUTO_JUMP); }
    void onNotAutoJump() { onEndResetFlg0(ERFLG0_NOT_AUTO_JUMP); }
    void onNotHang() { onEndResetFlg0(ERFLG0_NOT_HANG); }
    void onLeafSe() { onEndResetFlg0(ERFLG0_UNK_200000); }
    void onWolfFchainPull() { onEndResetFlg0(ERFLG0_WOLF_FCHAIN_PULL); }
    void onFishingRodGetFish() { onEndResetFlg0(ERFLG0_FISHING_ROD_GET_FISH); }
    void onSingleBoarAvoid() { onEndResetFlg0(ERFLG0_SINGLE_BOAR_AVOID); }
    void onShieldBackBone() { onEndResetFlg1(ERFLG1_GANON_FINISH); }
    void onWolfEyeKeep() { onEndResetFlg1(ERFLG1_WOLF_EYE_KEEP); }
    void onPortalWarpMidnaAtnKeep() { onEndResetFlg2(ERFLG2_PORTAL_WARP_MIDNA_ATN_KEEP); }
    void onFogFade() { onNoResetFlg2(FLG2_UNK_4000); }
    void onDkCaught2() { onNoResetFlg0(FLG0_DK_CAUGHT); }
    void onFishingRodCastingEnd() { onNoResetFlg1(FLG1_UNK_8000); }
    void onFishingRodReelEnd() { onEndResetFlg0(ERFLG0_UNK_20000); }
    void onFishingHit() { onEndResetFlg0(ERFLG0_UNK_10000); }
    void onFishingKeep() { onEndResetFlg2(ERFLG2_UNK_1); }
    void onTraningCutHead() { onNoResetFlg3(FLG3_UNK_40); }
    void onTraningCutFast() { onNoResetFlg3(FLG3_UNK_80); }
    void onTraningCutLargeJump() { onNoResetFlg3(FLG3_UNK_200); }
    void onTraningCutLargeTurn() { onNoResetFlg3(FLG3_UNK_100); }
    void onTraningCutDown() { onNoResetFlg3(FLG3_UNK_10); }
    void onTraningShieldAttack() { onNoResetFlg3(FLG3_UNK_8); }

    BOOL checkStickArrowReset() const { return checkResetFlg0(RFLG0_UNK_1); }
    BOOL getCutAtFlg() const { return checkNoResetFlg0(FLG0_UNK_40); }
    BOOL checkBoarSingleBattleFirst() const { return checkNoResetFlg2(FLG2_BOAR_SINGLE_BATTLE); }
    BOOL checkBoarSingleBattleSecond() const { return checkNoResetFlg2(FLG2_UNK_1000000); }
    BOOL getFootOnGround() const { return checkResetFlg1(daPy_RFLG1(RFLG1_UNK_10 | RFLG1_UNK_20)); }

    void offWolfEnemyHangBite() { offNoResetFlg2(FLG2_WOLF_ENEMY_HANG_BITE); }
    bool onWolfEnemyHangBite(fopAc_ac_c* param_0) { return onWolfEnemyBiteAll(param_0, FLG2_WOLF_ENEMY_HANG_BITE); }

    void offHorseZelda() { offNoResetFlg2(FLG2_HORSE_ZELDA); }
    void onHorseZelda() { onNoResetFlg2(FLG2_HORSE_ZELDA); }

    BOOL checkItemSightBgHit() const { return checkResetFlg0(RFLG0_UNK_2000000); }

    void setCanoeSlider() { mSpecialMode = 0x2D; }
    
    void offCanoeSlider() {
        if (checkCanoeSlider()) {
            mSpecialMode = 0;
        }
    }
    void onMidnaTalkPolySpeed() { onNoResetFlg3(FLG3_MIDNA_TALK_POLY_SPEED); }
    void onWarpObjDemo() { onNoResetFlg3(FLG3_WARP_OBJ_DEMO); }

    void offCargoCarry() {
        if (checkCargoCarry()) {
            mSpecialMode = 0;
        }
    }

    bool onWolfEnemyCatch(fopAc_ac_c* i_actor) { return onWolfEnemyBiteAll(i_actor, FLG2_UNK_8); }

    void offGoronSideMove() {
        if (checkGoronSideMove()) {
            mSpecialMode = 0;
        }
    }

    void offGoronUpStopCancel() {
        offNoResetFlg0(FLG0_UNK_200000);
    }

    s16 getBodyAngleX() const { return mBodyAngle.x; }
    s16 getBodyAngleY() const { return mBodyAngle.y; }

    f32 getHeight() const { return field_0x598; }

    BOOL checkMidnaWarp() const { return 0; }

    u32 checkNoResetFlg0(daPy_FLG0 i_flag) const { return mNoResetFlg0 & i_flag; }
    u32 checkNoResetFlg1(daPy_FLG1 i_flag) const { return mNoResetFlg1 & i_flag; }
    u32 checkNoResetFlg2(daPy_FLG2 i_flag) const { return mNoResetFlg2 & i_flag; }
    u32 checkNoResetFlg3(daPy_FLG3 i_flag) const { return mNoResetFlg3 & i_flag; }

    void onNoResetFlg0(daPy_FLG0 i_flag) { mNoResetFlg0 |= i_flag; }
    void onNoResetFlg1(daPy_FLG1 i_flag) { mNoResetFlg1 |= i_flag; }
    void onNoResetFlg2(daPy_FLG2 i_flag) { mNoResetFlg2 |= i_flag; }
    void onNoResetFlg3(daPy_FLG3 i_flag) { mNoResetFlg3 |= i_flag; }

    void offNoResetFlg0(daPy_FLG0 i_flag) { mNoResetFlg0 &= ~i_flag; }
    void offNoResetFlg1(daPy_FLG1 i_flag) { mNoResetFlg1 &= ~i_flag; }
    void offNoResetFlg2(daPy_FLG2 i_flag) { mNoResetFlg2 &= ~i_flag; }
    void offNoResetFlg3(daPy_FLG3 i_flag) { mNoResetFlg3 &= ~i_flag; }

    void offResetFlg0(daPy_RFLG0 flag) { mResetFlg0 &= ~flag; }
    void offResetFlg1(daPy_RFLG1 flag) { mResetFlg1 &= ~flag; }
    void onResetFlg0(daPy_RFLG0 flag) { mResetFlg0 |= flag; }
    void onResetFlg1(daPy_RFLG1 flag) { mResetFlg1 |= flag; }

    void onEndResetFlg0(daPy_ERFLG0 i_flag) { mEndResetFlg0 |= i_flag; }
    void onEndResetFlg2(daPy_ERFLG2 i_flag) { mEndResetFlg2 |= i_flag; }
    void offEndResetFlg2(daPy_ERFLG2 i_flag) { mEndResetFlg2 &= ~i_flag; }

    u32 checkResetFlg0(daPy_RFLG0 i_flag) const { return mResetFlg0 & i_flag; }
    u32 checkResetFlg1(daPy_RFLG1 i_flag) const { return mResetFlg1 & i_flag; }

    u32 checkEndResetFlg0(daPy_ERFLG0 i_flag) const { return mEndResetFlg0 & i_flag; }
    u32 checkEndResetFlg1(daPy_ERFLG1 i_flag) const { return mEndResetFlg1 & i_flag; }
    u32 checkEndResetFlg2(daPy_ERFLG2 i_flag) const { return mEndResetFlg2 & i_flag; }

    void onEndResetFlg1(daPy_ERFLG1 i_flag) { mEndResetFlg1 |= i_flag; }

    cXyz* getLeftHandPosP() { return &mLeftHandPos; }
    cXyz* getRightHandPosP() { return &mRightHandPos; }
    cXyz* getHeadTopPosP() { return &mHeadTopPos; }

    const cXyz getSwordTopPos() const { return mSwordTopPos; }

    u32 checkWolf() const { return checkNoResetFlg1(FLG1_IS_WOLF); }
    u32 checkEquipHeavyBoots() const { return checkNoResetFlg0(FLG0_EQUIP_HVY_BOOTS); }
    u32 checkMagneBootsOn() const { return checkNoResetFlg0(FLG0_MAGNE_BOOTS_ON); }
    BOOL checkFastSwordCut() const { return checkNoResetFlg0(FLG0_UNK_80000); }
    u32 checkMidnaRide() const { return checkNoResetFlg0(FLG0_MIDNA_RIDE); }
    BOOL checkWolfGrowl() const { return checkResetFlg0(RFLG0_WOLF_GROWL); }
    BOOL checkWolfThreat() const { return checkWolfGrowl(); }
    BOOL checkWolfBark() const { return checkResetFlg0(RFLG0_WOLF_BARK); }
    u32 checkBoomerangAtnKeep() const { return checkNoResetFlg3(FLG3_BOOMERANG_ATN_KEEP); }
    u32 checkCopyRodAtnKeep() const { return checkNoResetFlg3(FLG3_COPY_ROD_ATN_KEEP); }

    void onPlayerNoDraw() { onNoResetFlg0(FLG0_PLAYER_NO_DRAW); }
    void offPlayerNoDraw() { offNoResetFlg0(FLG0_PLAYER_NO_DRAW); }
    void onPushPullKeep() { onNoResetFlg0(FLG0_PUSH_PULL_KEEP); }
    void offPushPullKeep() { offNoResetFlg0(FLG0_PUSH_PULL_KEEP); }
    void onMidnaRide() { onNoResetFlg0(FLG0_MIDNA_RIDE); }
    void offMidnaRide() { offNoResetFlg0(FLG0_MIDNA_RIDE); }
    void onMidnaRideShock() { onEndResetFlg1(ERFLG1_MIDNA_RIDE_SHOCK); }
    void onPlayerShadowNoDraw() { onNoResetFlg2(FLG2_PLAYER_SHADOW); }
    void offPlayerShadowNoDraw() { offNoResetFlg2(FLG2_PLAYER_SHADOW); }
    void onLightSwordGetEffect() { onEndResetFlg2(ERFLG2_UNK_10); }
    void onForceGameOver() { onNoResetFlg2(FLG2_FORCE_GAMEOVER); }
    void onForceWolfChange() { onEndResetFlg0(ERFLG0_UNK_2); }
    void onDoPutEmphasys() { onEndResetFlg1(ERFLG1_UNK_10000000); }
    void onDoExchangePutIn() { onEndResetFlg1(ERFLG1_UNK_4000000); }
    void onNsScream() { onEndResetFlg1(ERFLG1_UNK_1); }
    void onNsScreamAnm() { onEndResetFlg1(daPy_ERFLG1(ERFLG1_UNK_1 | ERFLG1_UNK_2)); }
    void onLargeDamageUpStop() { onEndResetFlg1(ERFLG1_LARGE_DAMAGE_UP_STOP); }
    void onTraningCutBack() { onNoResetFlg3(FL3_TRANING_CUT_BACK); }
    void onNeckSearchWide() { onEndResetFlg0(ERFLG0_UNK_400); }
    void offPressedDamage() { offNoResetFlg2(FLG2_PRESSED_DAMAGE); }
    void onForceSubjectCancel() { onEndResetFlg0(ERFLG0_FORCE_SUBJECT_CANCEL); }
    void onEnemyDead() { onEndResetFlg0(ERFLG0_ENEMY_DEAD); }
    void offDkCaught() { offNoResetFlg1(FLG1_DK_CAUGHT); }
    void offDkCaught2() { offNoResetFlg0(FLG0_DK_CAUGHT); }
    void onWaterFallFrontHit() { onEndResetFlg1(ERFLG1_UNK_800); }
    void onCanoeItemCancel() { onEndResetFlg1(ERFLG1_UNK_20000000); }
    void onSwordTriggerNon() { onEndResetFlg1(ERFLG1_UNK_40000000); }

    u32 checkBoarSingleBattle() const { return checkNoResetFlg2(daPy_FLG2(FLG2_UNK_1000000 | FLG2_BOAR_SINGLE_BATTLE)); }
    u32 checkWolfDashAutoJump() const { return checkNoResetFlg2(FLG2_WOLF_DASH_AUTO_JUMP); }

    void changeOriginalDemo() {
        mDemo.setOriginalDemoType();
        mDemo.setParam0(0);
    }

    void changeDemoMode(u32 i_demoMode, int i_param0, int i_param1, s16 i_param2) {
        mDemo.setDemoMode(i_demoMode);
        mDemo.setParam0(i_param0);
        mDemo.setParam1(i_param1);
        mDemo.setParam2(i_param2);
    }

    void changeDemoParam0(int i_param1) {
        mDemo.setParam0(i_param1);
    }

    void changeDemoParam1(int i_param1) {
        mDemo.setParam1(i_param1);
    }

    void changeDemoParam2(s16 i_param2) {
        mDemo.setParam2(i_param2);
    }

    void changeDemoPos0(const cXyz* i_posP) { mDemo.setPos0(i_posP); }

    void changeDemoMoveAngle(s16 i_angle) { mDemo.setMoveAngle(i_angle); }

    void setItemPos(cXyz* i_itemPos) { mItemPos = *i_itemPos; }

    cXyz* getViewerCurrentPosP() { return &field_0x5f8; }

    static bool checkPeepEndSceneChange() { return getLastSceneMode() == 7; }

    static bool checkWolfCargoCarrySceneChange() { return getLastSceneMode() == 10; }

    static int getLastSceneDamage() { return (dComIfGs_getLastSceneMode() >> 4) & 0x7F; }
    static u32 getLastSceneSwordAtUpTime() { return (dComIfGs_getLastSceneMode() >> 11) & 0xFF; }

    static BOOL checkNormalSwordEquip() { return dComIfGs_getSelectEquipSword() == fpcNm_ITEM_SWORD; }

    static u32 getLastSceneMode() {
        return dComIfGs_getLastSceneMode() & 0xF;
    }

    static bool checkWoodSwordEquip() {
        return dComIfGs_getSelectEquipSword() == fpcNm_ITEM_WOOD_STICK;
    }

    static bool checkLightMasterSwordEquip() {
        return dComIfGs_getSelectEquipSword() == fpcNm_ITEM_LIGHT_SWORD;
    }

    static bool checkCasualWearFlg() {
        return dComIfGs_getSelectEquipClothes() == fpcNm_ITEM_WEAR_CASUAL;
    }

    static u32 checkNowWolf() { return ((daPy_py_c*)dComIfGp_getLinkPlayer())->checkWolf(); }

    static bool checkZoraWearFlg() {
        return dComIfGs_getSelectEquipClothes() == fpcNm_ITEM_WEAR_ZORA;
    }

    static bool checkMagicArmorWearFlg() {
        return dComIfGs_getSelectEquipClothes() == fpcNm_ITEM_ARMOR;
    }

                                               /* dSv_event_flag_c::M_067 - Main Event - Midna riding / not riding (ON == riding) */
    static BOOL checkFirstMidnaDemo() { return dComIfGs_isEventBit(0xc10); }
    static int checkNowWolfPowerUp() { return checkNowWolfEyeUp(); }

    static daMidna_c* getMidnaActor() { return m_midnaActor; }
    static void setMidnaActor(fopAc_ac_c* actor) { m_midnaActor = (daMidna_c*)actor; }

    BOOL checkWolfEnemyCatchOwn(fopAc_ac_c* i_actorP) { return checkWolfEnemyBiteAllOwn(i_actorP); }
    BOOL checkWolfEnemyHangBiteOwn(const fopAc_ac_c* i_actorP) const { return checkWolfEnemyBiteAllOwn(i_actorP); }
    BOOL checkWolfEnemyLeftThrow() const { return checkNoResetFlg2(FLG2_WOLF_ENEMY_LEFT_THROW); }

    void onWolfLightDropGet() {
        onEndResetFlg0(ERFLG0_UNK_20000000);
        onNoResetFlg3(FLG3_UNK_200000);
    }

    BOOL checkCowGameLash() const { return checkResetFlg0(RFLG0_COW_GAME_LEASH); }
    BOOL checkClimbEndHang() { return checkResetFlg0(RFLG0_UNK_40000); }

    void onForceHorseGetOff() {
        onEndResetFlg2(ERFLG2_UNK_2);
    }

    s16 getFishingRodAngleY() const {
        return shape_angle.y + mBodyAngle.y;
    }

    void onFishingRelease() {
        onEndResetFlg0(ERFLG0_UNK_4000000);
    }

    static daMidna_c* m_midnaActor;

    void setGiantPuzzle() { mSpecialMode = SMODE_WOLF_PUZZLE; }
    void setGiantPuzzleEnd() { mSpecialMode = 0; }

    BOOL checkAutoJumpStart() const { return checkResetFlg0(RFLG0_UNK_100); }

    void onForceGrabRebound() {
        onEndResetFlg2(ERFLG2_UNK_8);
    }

    void setSumouReadyAcceptButton() { mSpecialMode = 2; }
    void setSumouForceStand() { mSpecialMode = 3; }
    void setSumouPushFrontStop() { mSpecialMode = 9; }
    void setSumouPunchWinEnd() { mSpecialMode = 0xB; }
    void setSumouPunchLoseEnd() { mSpecialMode = 0xC; }
    void setSumouMoveWinEnd() { mSpecialMode = 0xD; }
    void setSumouMoveLoseEnd() { mSpecialMode = 0xE; }
    void setSumouForcePunch() {
        if (mSpecialMode == 0x1F) {
            mSpecialMode = 0x21;
        } else {
            mSpecialMode = 0x1C;
        }
    }
    void setSumouForceTackle() { mSpecialMode = 0x1B; }
    void setSumouForceGraspCancel() { mSpecialMode = 0x24; }

    bool checkSumouPushFront() const { return mSpecialMode == 7; }
    bool checkSumouPushBack() const { return mSpecialMode == 8; }
    bool checkSumouTackleSuccess() const { return mSpecialMode == 0xF; }
    bool checkSumouTackleSuccessPunch() const { return mSpecialMode == 0x10; }
    bool checkSumouTackleMiss() const { return mSpecialMode == 0x11; }
    bool checkSumouTackleDraw() const { return mSpecialMode == 0x12; }
    bool checkSumouPunchSuccess() const { return mSpecialMode == 0x13; }
    bool checkSumouPunchMiss() const { return mSpecialMode == 0x14; }
    bool checkSumouPunchDraw() const { return mSpecialMode == 0x15; }
    bool checkSumouWait() const { return mSpecialMode == 0x16; }
    bool checkSumouLeftMove() const { return mSpecialMode == 0x17; }
    bool checkSumouRightMove() const { return mSpecialMode == 0x18; }
    bool checkSumouSlideLeft() const { return mSpecialMode == 0x19; }
    bool checkSumouSlideRight() const { return mSpecialMode == 0x1A; }
    bool checkSumouPunchStagger() const { return mSpecialMode == 0x1F; }
    bool checkSumouTackleStagger() const { return mSpecialMode == 0x20; }
    bool checkSumouGraspRelease() const { return mSpecialMode == 0x23; }

    void onHeavyState() { onNoResetFlg0(FLG0_UNK_40000000); }
    void onHeavyStateMidnaPanic() {
        onHeavyState();
        onEndResetFlg1(ERFLG1_UNK_40000);
    }

    BOOL checkInsectRelease() { return checkResetFlg1(RFLG1_UNK_100); }

    void onChainForcePull() { onEndResetFlg1(ERFLG1_UNK_1000000); }
    void onChainPullEnd() { onEndResetFlg1(ERFLG1_UNK_2000000); }

    void onWaterDrop() { onEndResetFlg1(ERFLG1_UNK_20000); }
    void forceKandelaarLightOff() { onNoResetFlg2(FLG2_UNK_10000000); }
};

int daPy_addCalcShort(s16* param_0, s16 param_1, s16 param_2, s16 param_3, s16 param_4);

inline daPy_py_c* daPy_getPlayerActorClass() {
    return (daPy_py_c*)dComIfGp_getPlayer(0);
}

inline daPy_py_c* daPy_getLinkPlayerActorClass() {
    return dComIfGp_getLinkPlayer();
}

#endif /* D_A_D_A_PLAYER_H */
