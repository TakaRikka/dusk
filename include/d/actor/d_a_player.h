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
    void setSightImage(ResTIMG* i_img);

    u8 getDrawFlg() { return mDrawFlag; }
    void onDrawFlg() { mDrawFlag = true; }
    void offDrawFlg() { mDrawFlag = false; }
    void setPos(const cXyz* i_pos) { mPos = *i_pos; }
    cXyz* getPosP() { return &mPos; }

    /* 0x04 */ u8 mDrawFlag;
    /* 0x08 */ cXyz mPos;
    /* 0x14 */ Mtx mProjMtx;
    /* 0x44 */ ResTIMG* mpImg;
    /* 0x48 */ u8* mpData;
};

class daPy_boomerangMove_c {
public:
    void initOffset(const cXyz* i_pos);
    void initOffset(const cXyz* i_pos, const dCcD_GObjInf*) { initOffset(i_pos); }
    int posMove(cXyz* o_pos, s16* o_rotY, fopAc_ac_c* i_objActor, s16 i_rotStep);
    void bgCheckAfterOffset(const cXyz* i_pos);

    static void initDropAngleY() { m_dropAngleY = 0x4000; }
    static void offEventKeepFlg() { m_eventKeepFlg = 0; }
    static void onEventKeepFlg() { m_eventKeepFlg = 1; }

    static s16 m_dropAngleY;
    static s16 m_eventKeepFlg;

private:
    /* 0x0 */ u8 field_0x0;
    /* 0x2 */ s16 field_0x2;
    /* 0x4 */ f32 m_offsetY;
    /* 0x8 */ f32 m_offsetXZ;
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

    daPy_anmHeap_c(u32 i_bufferSize = 0);
    ~daPy_anmHeap_c();
    void initData();
    void* mallocBuffer();
    void createHeap(daAlinkHEAP_TYPE i_heapType);
    void* loadData(u16 i_resId);
    void* loadDataIdx(u16 i_resId);
    void* loadDataPriIdx(u16 i_resId);
    void* loadDataDemoRID(u16 i_resID, u16 i_arcNo);
    JKRHeap* setAnimeHeap();

    u16 getIdx() const { return mIdx; }
    void resetIdx() { mIdx = 0xFFFF; }
    void resetPriIdx() { mPriIdx = 0xFFFF; }
    void resetArcNo() { mArcNo = 0xFFFF; }
    bool checkNoSetArcNo() const { return mArcNo == 0xFFFF; }
    void setBufferSize(u32 i_size) { mBufferSize = i_size; }
    void setBuffer(u8* i_buffer) { mBuffer = i_buffer; }
    u32 getBufferSize() { return mBufferSize; }
    u8* getBuffer() { return mBuffer; }
    bool checkNoSetIdx() const { return mIdx == 0xFFFF; }
    bool checkNoSetPriIdx() const { return mPriIdx == 0xFFFF; }
    u16 getArcNo() const { return mArcNo; }
    void setIdx(u16 i_idx) { mIdx = i_idx; }

    /* 0x00 */ u16 mIdx;
    /* 0x02 */ u16 mPriIdx;
    /* 0x04 */ u16 mArcNo;
    /* 0x08 */ u32 mBufferSize;
    /* 0x0C */ u8* mBuffer;
    /* 0x10 */ JKRSolidHeap* mAnimeHeap;
};  // Size = 0x14

class daPy_actorKeep_c {
public:
    daPy_actorKeep_c() { clearData(); }

    void setActor();
    void setData(fopAc_ac_c* i_actor);
    void clearData();

    fpc_ProcID getID() const { return mID; }
    void setID(fpc_ProcID i_id) { mID = i_id; }
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
    void setFrameCtrl(u8 i_attribute, s16 i_start, s16 i_end, f32 i_rate, f32 i_frame);

    u16 getEndFlg() const { return mEndFlg; }
    u16 getNowSetFlg() const { return mNowSetFlg; }
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
    /* 0x056A */ u8 mMode;
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
    /* 0x0594 */ f32 mMaxSpeed;
    /* 0x0598 */ f32 mHeight;
    /* 0x059C */ csXyz mBodyAngle;
    /* 0x05A4 */ cXyz mHeadTopPos;
    /* 0x05B0 */ cXyz mItemPos;
    /* 0x05BC */ cXyz mSwordTopPos;
    /* 0x05C8 */ cXyz mLeftHandPos;
    /* 0x05D4 */ cXyz mRightHandPos;
    /* 0x05E0 */ cXyz mLeftFootPos;
    /* 0x05EC */ cXyz mRightFootPos;
    /* 0x05F8 */ cXyz mViewerCurrentPos;
    /* 0x0604 */ daPy_demo_c mDemo;

public:
    enum daPy_FLG0 {
        FLG0_HEAVY_STATE = 0x40000000,
        FLG0_EQUIP_HVY_BOOTS = 0x2000000,
        FLG0_PLAYER_NO_DRAW = 0x8000000,
        FLG0_UNK_10000000 = 0x10000000,
        FLG0_UNK_1000000 = 0x1000000,
        FLG0_WATER_IN_MOVE = 0x800000,
        FLG0_UNK_400000 = 0x400000,
        FLG0_GORON_UP_STOP_CANCEL = 0x200000,
        FLG0_UNK_100000 = 0x100000,
        FLG0_FAST_SWORD_CUT = 0x80000,
        FLG0_UNK_40000 = 0x40000,
        FLG0_DK_CAUGHT2 = 0x20000,
        FLG0_DEMO_STREAM_ACCEPT = 0x10000,
        FLG0_UNK_8000 = 0x8000,
        FLG0_UNK_4000 = 0x4000,
        FLG0_UNK_2000 = 0x2000,
        FLG0_MAGNE_BOOTS_ON = 0x1000,
        FLG0_PUSH_PULL_KEEP = 0x800,
        FLG0_UNK_400 = 0x400,
        FLG0_SWIM_UP = 0x100,
        FLG0_UNK_80 = 0x80,
        FLG0_CUT_AT_FLG = 0x40,
        FLG0_UNK_20 = 0x20,
        FLG0_UNK_10 = 0x10,
        FLG0_UNK_8 = 0x8,
        FLG0_MIDNA_RIDE = 4,
        FLG0_UNK_2 = 2,
    };

    enum daPy_FLG1 {
        FLG1_UNK_80000000 = 0x80000000,
        FLG1_FREEZE_DAMAGE = 0x40000000,
        FLG1_UNK_20000000 = 0x20000000,
        FLG1_UNK_10000000 = 0x10000000,
        FLG1_UNK_8000000 = 0x8000000,
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
        FLG1_FISHING_ROD_CASTING_END = 0x8000,
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
    };

    enum daPy_FLG2 {
        FLG2_HORSE_ZELDA = 0x40000000,
        FLG2_FISHING_CAST_WAIT = 0x20000000,
        FLG2_KANDELAAR_LIGHT_OFF = 0x10000000,
        FLG2_UNK_4080000 = 0x4080000,
        FLG2_UNK_4000000 = 0x4000000,
        FLG2_UNK_2080000 = 0x2080000,
        FLG2_UNK_2000000 = 0x2000000,
        FLG2_UNK_8000000 = 0x8000000,
        FLG2_BOAR_SINGLE_BATTLE_2ND = 0x1000000,
        FLG2_BOAR_SINGLE_BATTLE_1ST = 0x800000,
        FLG2_STATUS_WINDOW_DRAW = 0x400000,
        FLG2_UNK_280000 = 0x280000,
        FLG2_UNK_200000 = 0x200000,
        FLG2_UNK_100000 = 0x100000,
        FLG2_UNK_80000 = 0x80000,
        FLG2_FORCE_GAMEOVER = 0x40000,
        FLG2_UNK_20000 = 0x20000,
        FLG2_WOLF_DASH_AUTO_JUMP = 0x10000,
        FLG2_SCENE_CHANGE_START = 0x8000,
        FLG2_FOG_FADE = 0x4000,
        FLG2_UNK_2000 = 0x2000,
        FLG2_PRESSED_DAMAGE = 0x1000,
        FLG2_UNK_800 = 0x800,
        FLG2_PLAYER_SHADOW_NO_DRAW = 0x400,
        FLG2_UNK_200 = 0x200,
        FLG2_UNK_100 = 0x100,
        FLG2_UNK_80 = 0x80,
        FLG2_WOLF_ENEMY_HANG_BITE = 0x40,
        FLG2_WOLF_ENEMY_LEFT_THROW = 0x20,
        FLG2_UNK_10 = 0x10,
        FLG2_UNK_8 = 8,
        FLG2_UNK_4 = 4,
        FLG2_COMBO_RESERB = 2,
        FLG2_UNK_1 = 1,
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
        FLG3_TRANING_CUT_LARGE_JUMP = 0x200,
        FLG3_TRANING_CUT_LARGE_TURN = 0x100,
        FLG3_TRANING_CUT_FAST = 0x80,
        FLG3_TRANING_CUT_HEAD = 0x40,
        FLG3_TRANING_CUT_BACK = 0x20,
        FLG3_TRANING_CUT_DOWN = 0x10,
        FLG3_TRANING_SHIELD_ATTACK = 8,
        FLG3_UNK_4 = 4,
        FLG3_UNK_1 = 1,
    };

    enum daPy_ERFLG0 {
        ERFLG0_FORCE_SUBJECT_CANCEL = 0x80000000,
        ERFLG0_UNK_40000000 = 0x40000000,
        ERFLG0_UNK_20000000 = 0x20000000,
        ERFLG0_WOLF_FCHAIN_PULL = 0x10000000,
        ERFLG0_UNK_8000000 = 0x8000000,
        ERFLG0_FISHING_RELEASE = 0x4000000,
        ERFLG0_BOSS_ROOM_WAIT = 0x2000000,
        ERFLG0_ENEMY_DEAD = 0x1000000,
        ERFLG0_UNK_800000 = 0x800000,
        ERFLG0_BEE_FOLLOW = 0x400000,
        ERFLG0_LEAF_SE = 0x200000,
        ERFLG0_UNK_100000 = 0x100000,
        ERFLG0_SINGLE_BOAR_AVOID = 0x80000,
        ERFLG0_FISHING_ROD_GET_FISH = 0x40000,
        ERFLG0_FISHING_ROD_REEL_END = 0x20000,
        ERFLG0_FISHING_HIT = 0x10000,
        ERFLG0_UNK_4000 = 0x4000,
        ERFLG0_UNK_2000 = 0x2000,
        ERFLG0_UNK_1000 = 0x1000,
        ERFLG0_NECK_SEARCH_WIDE = 0x400,
        ERFLG0_FORCE_AUTO_JUMP = 0x200,
        ERFLG0_UNK_100 = 0x100,
        ERFLG0_UNK_80 = 0x80,
        ERFLG0_UNK_40 = 0x40,
        ERFLG0_NOT_AUTO_JUMP = 0x20,
        ERFLG0_NOT_HANG = 0x10,
        ERFLG0_UNK_8 = 8,
        ERFLG0_UNK_4 = 4,
        ERFLG0_FORCE_WOLF_CHANGE = 2,
        ERFLG0_UNK_1 = 1,
    };

    enum daPy_ERFLG1 {
        ERFLG1_SHIELD_BACKBONE = 0x80000000,
        ERFLG1_SWORD_TRIGGER_NON = 0x40000000,
        ERFLG1_CANOE_ITEM_CANCEL = 0x20000000,
        ERFLG1_DO_PUT_EMPHASYS = 0x10000000,
        ERFLG1_LARGE_DAMAGE_UP_STOP = 0x8000000,
        ERFLG1_DO_EXCHANGE_PUT_IN = 0x4000000,
        ERFLG1_CHAIN_PULL_END = 0x2000000,
        ERFLG1_CHAIN_FORCE_PULL = 0x1000000,
        ERFLG1_UNK_400000 = 0x400000,
        ERFLG1_UNK_200000 = 0x200000,
        ERFLG1_UNK_100000 = 0x100000,
        ERFLG1_UNK_80000 = 0x80000,
        ERFLG1_UNK_40000 = 0x40000,
        ERFLG1_WATER_DROP = 0x20000,
        ERFLG1_UNK_10000 = 0x10000,
        ERFLG1_UNK_8000 = 0x8000,
        ERFLG1_UNK_4000 = 0x4000,
        ERFLG1_UNK_2000 = 0x2000,
        ERFLG1_UNK_1000 = 0x1000,
        ERFLG1_WATERFALL_FRONT_HIT = 0x800,
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
        ERFLG1_NS_SCREAM = 1,
    };

    enum daPy_ERFLG2 {
        ERFLG2_UNK_100 = 0x100,
        ERFLG2_PORTAL_WARP_MIDNA_ATN_KEEP = 0x80,
        ERFLG2_UNK_40 = 0x40,
        ERFLG2_UNK_20 = 0x20,
        ERFLG2_LIGHT_SWORD_GET_EFFECT = 0x10,
        ERFLG2_FORCE_GRAB_REBOUND = 8,
        ERFLG2_FORCE_HORSE_GETOFF = 2,
        ERFLG2_FISHING_KEEP = 1,
    };

    enum daPy_RFLG0 {
        RFLG0_WOLF_BARK = 0x80000000,
        RFLG0_UNK_40000000 = 0x40000000,
        RFLG0_UNK_20000000 = 0x20000000,
        RFLG0_UNK_10000000 = 0x10000000,
        RFLG0_UNK_8000000 = 0x8000000,
        RFLG0_UNK_4000000 = 0x4000000,
        RFLG0_ITEM_SIGHT_BG_HIT = 0x2000000,
        RFLG0_GRAB_PUT_START = 0x400000,
        RFLG0_WOLF_GROWL = 0x200000,
        RFLG0_UNK_80000 = 0x80000,
        RFLG0_CLIMB_END_HANG = 0x40000,
        RFLG0_WOLF_TAG_LOCK_JUMP_READY = 0x20000,
        RFLG0_ATTENTION_LOCK = 0x10000,
        RFLG0_GRAB_UP_START = 0x8000,
        RFLG0_UNK_4000 = 0x4000,
        RFLG0_FRONT_ROLL_CRASH = 0x2000,
        RFLG0_ENEMY_ATTN_LOCK = 0x1000,
        RFLG0_UNK_400 = 0x400,
        RFLG0_COW_GAME_LEASH = 0x200,
        RFLG0_AUTO_JUMP_START = 0x100,
        RFLG0_UNK_80 = 0x80,
        RFLG0_UNK_40 = 0x40,
        RFLG0_GRAB_UP_END = 0x20,
        RFLG0_UNK_10 = 0x10,
        RFLG0_UNK_8 = 0x8,
        RFLG0_GRAB_THROW = 0x4,
        RFLG0_UNK_2 = 0x2,
        RFLG0_STICK_ARROW_RESET = 0x1,
    };

    enum daPy_RFLG1 {
        RFLG1_DAMAGE_IMPACT = 0x400,
        RFLG0_FISHINGROD_USE_ACCEPT = 0x200,
        RFLG1_INSECT_RELEASE = 0x100,
        RFLG0_FISHINGROD_USE_START = 0x80,
        RFLG1_BOARD_NO_FOOT_ANGLE = 0x40,
        RFLG1_UNK_30 = 0x30,
        RFLG1_UNK_20 = 0x20,
        RFLG1_UNK_10 = 0x10,
        RFLG1_UNK_8 = 0x8,
        RFLG1_UNK_4 = 0x4,
        RFLG1_UNK_2 = 0x2,
        RFLG1_WOLF_ATTACK_REVERSE = 0x1,
    };

    enum daPy_MODE {
        /* 0x01 */ SMODE_SUMO_READY = 1,
        /* 0x25 */ SMODE_SUMO_LOSE = 37,
        /* 0x27 */ SMODE_WOLF_PUZZLE = 39,
        /* 0x2A */ SMODE_GOAT_STOP = 42,
        /* 0x2B */ SMODE_GORON_THROW,
        /* 0x2C */ SMODE_CARGO_CARRY,
    };

    enum daPy_CUT_TYPE {
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
        /* 0x0B */ CUT_TYPE_DASH_LEFT_BOARD,
        /* 0x0C */ CUT_TYPE_DASH_RIGHT_BOARD,
        /* 0x0D */ CUT_TYPE_DASH_CHARGE_BOARD,
        /* 0x0E */ CUT_TYPE_UNK_E,
        /* 0x0F */ CUT_TYPE_UNK_F,
        /* 0x10 */ CUT_TYPE_DASH_LEFT_SIDESTEP,
        /* 0x11 */ CUT_TYPE_DASH_RIGHT_SIDESTEP,
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
        /* 0x22 */ CUT_TYPE_HORSE_LEFT_A,
        /* 0x23 */ CUT_TYPE_HORSE_LEFT_B,
        /* 0x24 */ CUT_TYPE_UNK_24,
        /* 0x25 */ CUT_TYPE_DASH_LEFT,
        /* 0x26 */ CUT_TYPE_DASH_RIGHT,
        /* 0x27 */ CUT_TYPE_DOWN,
        /* 0x28 */ CUT_TYPE_UNK_28,
        /* 0x29 */ CUT_TYPE_GUARD_ATTACK,
        /* 0x2A */ CUT_TYPE_HORSE_RIGHT_A,
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
        /* 0x38 */ CUT_TYPE_DASH_CHARGE,
        /* 0x39 */ CUT_TYPE_WOLF_JUMP,
    };

    static u32 setParamData(int i_roomNo, int i_mode, int, int);
    static BOOL checkFishingRodItem(int i_itemNo);
    static BOOL checkBombItem(int i_itemNo);
    static BOOL checkBottleItem(int i_itemNo);
    static BOOL checkDrinkBottleItem(int i_itemNo);
    static BOOL checkOilBottleItem(int i_itemNo);
    static BOOL checkOpenBottleItem(int i_itemNo);
    static BOOL checkBowItem(int i_itemNo);
    static BOOL checkHookshotItem(int i_itemNo);
    static BOOL checkTradeItem(int i_itemNo);
    static BOOL checkDungeonWarpItem(int i_itemNo);
    static BOOL checkMasterSwordEquip();
    static BOOL checkWoodShieldEquip();
    static f32 getAttentionOffsetY();
    static int checkNowWolfEyeUp();
    static void forceRestartRoom(int i_dmgAmount, u32 i_mode, int);
    static BOOL setFmChainPos(fopAc_ac_c*, cXyz*, int);
    static void cancelFmChainGrab();
    static void setLookPos(cXyz* i_pos);
    static void setPlayerSe(u32 i_soundID);
    static bool linkGrabSubjectNoDraw(fopAc_ac_c* i_actor);
    static bool wolfGrabSubjectNoDraw(fopAc_ac_c* i_actor);
    static bool checkRoomRestartStart();
    static u32 checkCarryStartLightBallA();
    static u32 checkCarryStartLightBallB();
    f32 getSpinnerRideSpeed() const;
    bool checkSpinnerReflectEffect();
    static bool checkBoomerangCharge();
    static bool checkBoomerangChargeTime();
    static daBoomerang_c* getThrowBoomerangActor();
    static void cancelBoomerangLockActor(fopAc_ac_c* i_actor);
    static void setPlayerDamage(int i_dmgAmount, BOOL i_setDmgTimer);
    static BOOL checkAcceptDungeonWarp();
    static void setMidnaMotionNum(int i_motionNum);
    static void setMidnaFaceNum(int i_faceNum);

    static BOOL checkShieldGet() { return dComIfGs_getSelectEquipShield() != fpcNm_ITEM_NONE; }
    static BOOL checkSwordGet() { return dComIfGs_getSelectEquipSword() != fpcNm_ITEM_NONE; }

    cXyz getHeadTopPos() const { return mHeadTopPos; }
    u32 checkThrowDamage() const { return checkNoResetFlg1(FLG1_THROW_DAMAGE); }
    bool checkGoronSideMove() const { return mMode == 0x2B; }
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
    BOOL getDkCaught2() const { return checkNoResetFlg0(FLG0_DK_CAUGHT2); }
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
    /* vt 0X120 */ virtual void setOutPower(f32, s16, int) {}
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
    /* vt 0X154 */ virtual void setPlayerPosAndAngle(cXyz const*, s16, int) {}
    /* vt 0X158 */ virtual void setPlayerPosAndAngle(cXyz const*, csXyz const*) {}
    /* vt 0X15C */ virtual void setPlayerPosAndAngle(f32 (*)[4]) {}
    /* vt 0X160 */ virtual bool setThrowDamage(s16, f32, f32, int, int, int) { return FALSE; }
    /* vt 0X164 */ virtual bool checkSetNpcTks(cXyz*, int, int) { return FALSE; }
    /* vt 0X168 */ virtual int setRollJump(f32, f32, s16) { return FALSE; }
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
    /* vt 0X1D4 */ virtual void onPressedDamage(cXyz const&, s16) {}
    /* vt 0X1D8 */ virtual bool checkPriActorOwn(fopAc_ac_c const*) const { return FALSE; }
    /* vt 0X1DC */ virtual bool onWolfEnemyBiteAll(fopAc_ac_c*, daPy_FLG2) { return FALSE; }
    /* vt 0X1E0 */ virtual bool checkWolfEnemyBiteAllOwn(fopAc_ac_c const*) const { return FALSE; }
    /* vt 0X1E4 */ virtual void setWolfEnemyHangBiteAngle(s16) {}
    /* vt 0X1E8 */ virtual void setKandelaarMtx(f32 (*)[4], int, int) {}
    /* vt 0X1EC */ virtual bool getStickAngleFromPlayerShape(s16*) const { return FALSE; }
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
    /* vt 0X230 */ virtual void setSumouPushBackDirection(s16) {}
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
        if (mMode != 0 && mMode < 0x26) {
            sumouCameraMode = true;
        }
        return sumouCameraMode;
    }

    bool getSumouMode() const { return getSumouCameraMode(); }
    void cancelOriginalDemo() {
        mDemo.setSystemDemoType();
        mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_1_e);
    }

    u32 getDemoMode() const { return mDemo.getDemoMode(); }

    u8 getCutCount() const { return mComboCutCount; }

    BOOL checkStatusWindowDraw() const { return checkNoResetFlg2(FLG2_STATUS_WINDOW_DRAW); }
    bool checkCargoCarry() const { return mMode == SMODE_CARGO_CARRY; }
    BOOL getHeavyStateAndBoots() { return checkNoResetFlg0(daPy_FLG0(FLG0_HEAVY_STATE | FLG0_EQUIP_HVY_BOOTS | FLG0_DK_CAUGHT2)); }
    BOOL checkEnemyAttentionLock() const { return checkResetFlg0(RFLG0_ENEMY_ATTN_LOCK); }
    BOOL getGrabUpStart() const { return checkResetFlg0(RFLG0_GRAB_UP_START); }
    bool checkCanoeSlider() const { return mMode == 0x2D; }
    bool checkGoatStopGame() const { return mMode == 0x2A; }
    void onGoatStopGame() { mMode = 0x2A; }
    u8 getCutType() const { return mCutType; }
    u16 getSwordAtUpTime() const { return mSwordUpTimer; }
    s16 getDamageWaitTimer() const { return mDamageTimer; }
    BOOL checkWaterInMove() const { return checkNoResetFlg0(FLG0_WATER_IN_MOVE); }
    BOOL checkSceneChangeAreaStart() const { return checkNoResetFlg2(FLG2_SCENE_CHANGE_START); }
    BOOL checkFrontRollCrash() const { return checkResetFlg0(RFLG0_FRONT_ROLL_CRASH); }
    BOOL checkWolfAttackReverse() const { return checkResetFlg1(RFLG1_WOLF_ATTACK_REVERSE); }
    BOOL checkFreezeDamage() const { return checkNoResetFlg1(FLG1_FREEZE_DAMAGE); }
    BOOL checkWolfTagLockJumpReady() const { return checkResetFlg0(RFLG0_WOLF_TAG_LOCK_JUMP_READY); }
    BOOL checkDamageImpact() const { return checkResetFlg1(RFLG1_DAMAGE_IMPACT); }
    BOOL getGrabUpEnd() const { return checkResetFlg0(RFLG0_GRAB_UP_END); }
    BOOL getGrabPutStart() const { return checkResetFlg0(RFLG0_GRAB_PUT_START); }
    BOOL checkSwimUp() const { return checkNoResetFlg0(FLG0_SWIM_UP); }
    BOOL checkHorseZelda() const { return checkNoResetFlg2(FLG2_HORSE_ZELDA); }
    BOOL checkSpecialHorseRide() { return checkNoResetFlg2(daPy_FLG2(FLG2_HORSE_ZELDA | FLG2_BOAR_SINGLE_BATTLE_2ND | FLG2_BOAR_SINGLE_BATTLE_1ST)); }
    BOOL checkBoardNoFootAngle() const { return checkResetFlg1(RFLG1_BOARD_NO_FOOT_ANGLE); }
    BOOL checkGrabThrow() const { return checkResetFlg0(RFLG0_GRAB_THROW); }
    BOOL checkMidnaAtnPos() const { return checkNoResetFlg1(FLG1_MIDNA_ATN_POS); }
    BOOL checkMidnaHairAtnPos() const { return checkNoResetFlg1(FLG1_MIDNA_HAIR_ATN_POS); }
    BOOL checkAttentionLock() const { return checkResetFlg0(RFLG0_ATTENTION_LOCK); }
    BOOL checkFishingRodUseStart() const { return checkResetFlg1(RFLG0_FISHINGROD_USE_START); }
    BOOL checkFishingRodUseAccept() const { return checkResetFlg1(RFLG0_FISHINGROD_USE_ACCEPT); }

    void onBossRoomWait() { onEndResetFlg0(ERFLG0_BOSS_ROOM_WAIT); }
    void onBeeFollow() { onEndResetFlg0(ERFLG0_BEE_FOLLOW); }
    void onForceAutoJump() { onEndResetFlg0(ERFLG0_FORCE_AUTO_JUMP); }
    void onNotAutoJump() { onEndResetFlg0(ERFLG0_NOT_AUTO_JUMP); }
    void onNotHang() { onEndResetFlg0(ERFLG0_NOT_HANG); }
    void onLeafSe() { onEndResetFlg0(ERFLG0_LEAF_SE); }
    void onWolfFchainPull() { onEndResetFlg0(ERFLG0_WOLF_FCHAIN_PULL); }
    void onFishingRodGetFish() { onEndResetFlg0(ERFLG0_FISHING_ROD_GET_FISH); }
    void onSingleBoarAvoid() { onEndResetFlg0(ERFLG0_SINGLE_BOAR_AVOID); }
    void onShieldBackBone() { onEndResetFlg1(ERFLG1_SHIELD_BACKBONE); }
    void onWolfEyeKeep() { onEndResetFlg1(ERFLG1_WOLF_EYE_KEEP); }
    void onPortalWarpMidnaAtnKeep() { onEndResetFlg2(ERFLG2_PORTAL_WARP_MIDNA_ATN_KEEP); }
    void onFogFade() { onNoResetFlg2(FLG2_FOG_FADE); }
    void onDkCaught2() { onNoResetFlg0(FLG0_DK_CAUGHT2); }
    void onFishingRodCastingEnd() { onNoResetFlg1(FLG1_FISHING_ROD_CASTING_END); }
    void onFishingRodReelEnd() { onEndResetFlg0(ERFLG0_FISHING_ROD_REEL_END); }
    void onFishingHit() { onEndResetFlg0(ERFLG0_FISHING_HIT); }
    void onFishingKeep() { onEndResetFlg2(ERFLG2_FISHING_KEEP); }
    void onTraningCutHead() { onNoResetFlg3(FLG3_TRANING_CUT_HEAD); }
    void onTraningCutFast() { onNoResetFlg3(FLG3_TRANING_CUT_FAST); }
    void onTraningCutLargeJump() { onNoResetFlg3(FLG3_TRANING_CUT_LARGE_JUMP); }
    void onTraningCutLargeTurn() { onNoResetFlg3(FLG3_TRANING_CUT_LARGE_TURN); }
    void onTraningCutDown() { onNoResetFlg3(FLG3_TRANING_CUT_DOWN); }
    void onTraningShieldAttack() { onNoResetFlg3(FLG3_TRANING_SHIELD_ATTACK); }
    void onDemoStreamAccept() { onNoResetFlg0(FLG0_DEMO_STREAM_ACCEPT); }
    void offDemoStreamAccept() { offNoResetFlg0(FLG0_DEMO_STREAM_ACCEPT); }

    BOOL checkStickArrowReset() const { return checkResetFlg0(RFLG0_STICK_ARROW_RESET); }
    BOOL getCutAtFlg() const { return checkNoResetFlg0(FLG0_CUT_AT_FLG); }
    BOOL checkBoarSingleBattleFirst() const { return checkNoResetFlg2(FLG2_BOAR_SINGLE_BATTLE_1ST); }
    BOOL checkBoarSingleBattleSecond() const { return checkNoResetFlg2(FLG2_BOAR_SINGLE_BATTLE_2ND); }
    BOOL getFootOnGround() const { return checkResetFlg1(daPy_RFLG1(RFLG1_UNK_10 | RFLG1_UNK_20)); }

    void offWolfEnemyHangBite() { offNoResetFlg2(FLG2_WOLF_ENEMY_HANG_BITE); }
    bool onWolfEnemyHangBite(fopAc_ac_c* i_enemy) { return onWolfEnemyBiteAll(i_enemy, FLG2_WOLF_ENEMY_HANG_BITE); }

    void offHorseZelda() { offNoResetFlg2(FLG2_HORSE_ZELDA); }
    void onHorseZelda() { onNoResetFlg2(FLG2_HORSE_ZELDA); }

    BOOL checkItemSightBgHit() const { return checkResetFlg0(RFLG0_ITEM_SIGHT_BG_HIT); }

    void setCanoeSlider() { mMode = 0x2D; }
    
    void offCanoeSlider() {
        if (checkCanoeSlider()) {
            mMode = 0;
        }
    }
    void onMidnaTalkPolySpeed() { onNoResetFlg3(FLG3_MIDNA_TALK_POLY_SPEED); }
    void onWarpObjDemo() { onNoResetFlg3(FLG3_WARP_OBJ_DEMO); }

    void offCargoCarry() {
        if (checkCargoCarry()) {
            mMode = 0;
        }
    }

    bool onWolfEnemyCatch(fopAc_ac_c* i_enemy) { return onWolfEnemyBiteAll(i_enemy, FLG2_UNK_8); }

    void offGoronSideMove() {
        if (checkGoronSideMove()) {
            mMode = 0;
        }
    }

    void offGoronUpStopCancel() {
        offNoResetFlg0(FLG0_GORON_UP_STOP_CANCEL);
    }

    s16 getBodyAngleX() const { return mBodyAngle.x; }
    s16 getBodyAngleY() const { return mBodyAngle.y; }

    f32 getHeight() const { return mHeight; }

    BOOL checkMidnaWarp() const { return FALSE; }

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

    void offResetFlg0(daPy_RFLG0 i_flag) { mResetFlg0 &= ~i_flag; }
    void offResetFlg1(daPy_RFLG1 i_flag) { mResetFlg1 &= ~i_flag; }
    void onResetFlg0(daPy_RFLG0 i_flag) { mResetFlg0 |= i_flag; }
    void onResetFlg1(daPy_RFLG1 i_flag) { mResetFlg1 |= i_flag; }

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
    BOOL checkFastSwordCut() const { return checkNoResetFlg0(FLG0_FAST_SWORD_CUT); }
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
    void onPlayerShadowNoDraw() { onNoResetFlg2(FLG2_PLAYER_SHADOW_NO_DRAW); }
    void offPlayerShadowNoDraw() { offNoResetFlg2(FLG2_PLAYER_SHADOW_NO_DRAW); }
    void onLightSwordGetEffect() { onEndResetFlg2(ERFLG2_LIGHT_SWORD_GET_EFFECT); }
    void onForceGameOver() { onNoResetFlg2(FLG2_FORCE_GAMEOVER); }
    void onForceWolfChange() { onEndResetFlg0(ERFLG0_FORCE_WOLF_CHANGE); }
    void onDoPutEmphasys() { onEndResetFlg1(ERFLG1_DO_PUT_EMPHASYS); }
    void onDoExchangePutIn() { onEndResetFlg1(ERFLG1_DO_EXCHANGE_PUT_IN); }
    void onNsScream() { onEndResetFlg1(ERFLG1_NS_SCREAM); }
    void onNsScreamAnm() { onEndResetFlg1(daPy_ERFLG1(ERFLG1_NS_SCREAM | ERFLG1_UNK_2)); }
    void onLargeDamageUpStop() { onEndResetFlg1(ERFLG1_LARGE_DAMAGE_UP_STOP); }
    void onTraningCutBack() { onNoResetFlg3(FLG3_TRANING_CUT_BACK); }
    void onNeckSearchWide() { onEndResetFlg0(ERFLG0_NECK_SEARCH_WIDE); }
    void offPressedDamage() { offNoResetFlg2(FLG2_PRESSED_DAMAGE); }
    void onForceSubjectCancel() { onEndResetFlg0(ERFLG0_FORCE_SUBJECT_CANCEL); }
    void onEnemyDead() { onEndResetFlg0(ERFLG0_ENEMY_DEAD); }
    void offDkCaught() { offNoResetFlg1(FLG1_DK_CAUGHT); }
    void offDkCaught2() { offNoResetFlg0(FLG0_DK_CAUGHT2); }
    void onWaterFallFrontHit() { onEndResetFlg1(ERFLG1_WATERFALL_FRONT_HIT); }
    void onCanoeItemCancel() { onEndResetFlg1(ERFLG1_CANOE_ITEM_CANCEL); }
    void onSwordTriggerNon() { onEndResetFlg1(ERFLG1_SWORD_TRIGGER_NON); }

    u32 checkBoarSingleBattle() const { return checkNoResetFlg2(daPy_FLG2(FLG2_BOAR_SINGLE_BATTLE_2ND | FLG2_BOAR_SINGLE_BATTLE_1ST)); }
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

    void changeDemoParam0(int i_param0) {
        mDemo.setParam0(i_param0);
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

    cXyz* getViewerCurrentPosP() { return &mViewerCurrentPos; }

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

    static bool checkCarvingWoodShieldEquip() {
        return dComIfGs_getSelectEquipShield() == fpcNm_ITEM_WOOD_SHIELD;
    }

    static bool checkShopWoodShieldEquip() {
        return dComIfGs_getSelectEquipShield() == fpcNm_ITEM_SHIELD;
    }

    static BOOL checkPowerGloveGet() { return false; }

    static BOOL checkFirstMidnaDemo() { return dComIfGs_isEventBit(dSv_event_flag_c::M_067); }
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
    BOOL checkClimbEndHang() { return checkResetFlg0(RFLG0_CLIMB_END_HANG); }

    void onForceHorseGetOff() {
        onEndResetFlg2(ERFLG2_FORCE_HORSE_GETOFF);
    }

    s16 getFishingRodAngleY() const {
        return shape_angle.y + mBodyAngle.y;
    }

    void onFishingRelease() {
        onEndResetFlg0(ERFLG0_FISHING_RELEASE);
    }

    static daMidna_c* m_midnaActor;

    void setGiantPuzzle() { mMode = SMODE_WOLF_PUZZLE; }
    void setGiantPuzzleEnd() { mMode = 0; }

    BOOL checkAutoJumpStart() const { return checkResetFlg0(RFLG0_AUTO_JUMP_START); }

    void onForceGrabRebound() {
        onEndResetFlg2(ERFLG2_FORCE_GRAB_REBOUND);
    }

    void setSumouReadyAcceptButton() { mMode = 2; }
    void setSumouForceStand() { mMode = 3; }
    void setSumouPushFrontStop() { mMode = 9; }
    void setSumouPunchWinEnd() { mMode = 0xB; }
    void setSumouPunchLoseEnd() { mMode = 0xC; }
    void setSumouMoveWinEnd() { mMode = 0xD; }
    void setSumouMoveLoseEnd() { mMode = 0xE; }
    void setSumouForcePunch() {
        if (mMode == 0x1F) {
            mMode = 0x21;
        } else {
            mMode = 0x1C;
        }
    }
    void setSumouForceTackle() { mMode = 0x1B; }
    void setSumouForceGraspCancel() { mMode = 0x24; }

    bool checkSumouPushFront() const { return mMode == 7; }
    bool checkSumouPushBack() const { return mMode == 8; }
    bool checkSumouTackleSuccess() const { return mMode == 0xF; }
    bool checkSumouTackleSuccessPunch() const { return mMode == 0x10; }
    bool checkSumouTackleMiss() const { return mMode == 0x11; }
    bool checkSumouTackleDraw() const { return mMode == 0x12; }
    bool checkSumouPunchSuccess() const { return mMode == 0x13; }
    bool checkSumouPunchMiss() const { return mMode == 0x14; }
    bool checkSumouPunchDraw() const { return mMode == 0x15; }
    bool checkSumouWait() const { return mMode == 0x16; }
    bool checkSumouLeftMove() const { return mMode == 0x17; }
    bool checkSumouRightMove() const { return mMode == 0x18; }
    bool checkSumouSlideLeft() const { return mMode == 0x19; }
    bool checkSumouSlideRight() const { return mMode == 0x1A; }
    bool checkSumouPunchStagger() const { return mMode == 0x1F; }
    bool checkSumouTackleStagger() const { return mMode == 0x20; }
    bool checkSumouGraspRelease() const { return mMode == 0x23; }

    void onHeavyState() { onNoResetFlg0(FLG0_HEAVY_STATE); }
    void onHeavyStateMidnaPanic() {
        onHeavyState();
        onEndResetFlg1(ERFLG1_UNK_40000);
    }

    BOOL checkInsectRelease() { return checkResetFlg1(RFLG1_INSECT_RELEASE); }

    void onChainForcePull() { onEndResetFlg1(ERFLG1_CHAIN_FORCE_PULL); }
    void onChainPullEnd() { onEndResetFlg1(ERFLG1_CHAIN_PULL_END); }

    void onWaterDrop() { onEndResetFlg1(ERFLG1_WATER_DROP); }
    void forceKandelaarLightOff() { onNoResetFlg2(FLG2_KANDELAAR_LIGHT_OFF); }
};

void daPy_aramBufferCheck(void* i_buffer, u32 i_size);
int daPy_addCalcShort(s16* i_value, s16 i_target, s16 i_scale, s16 i_maxStep, s16 i_minStep);

inline daPy_py_c* daPy_getPlayerActorClass() {
    return (daPy_py_c*)dComIfGp_getPlayer(0);
}

inline daPy_py_c* daPy_getLinkPlayerActorClass() {
    return dComIfGp_getLinkPlayer();
}

#endif /* D_A_D_A_PLAYER_H */
