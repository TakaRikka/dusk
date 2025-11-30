#ifndef D_A_OBJ_CARRY_H
#define D_A_OBJ_CARRY_H

#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_jnt_col.h"

struct daObjCarry_dt_t {
    /* 0x00 */ f32 m_throwInitSpeedY;
    /* 0x04 */ f32 m_throwInitSpeedXZ;
    /* 0x08 */ f32 m_wolfThrowInitSpeedY;
    /* 0x0C */ f32 m_wolfThrowInitSpeedXZ;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 m_gravity;
    /* 0x1C */ f32 m_Buoyancy;
    /* 0x20 */ f32 m_urnWindEffRatio;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 scale;
    /* 0x34 */ u8 m_carry_attn_dist;
    /* 0x35 */ u8 m_flags;
    /* 0x36 */ u8 m_colliderWeight;
    /* 0x37 */ u8 m_cyl_atp;
    /* 0x38 */ f32 m_cullsph_min_x;
    /* 0x3C */ f32 m_cullsph_min_y;
    /* 0x40 */ f32 m_cullsph_min_z;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 m_height;
    /* 0x50 */ f32 m_radius;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ u32 m_breakSound;
    /* 0x60 */ u32 m_putSound;
    /* 0x64 */ u32 m_putWaterSound;
    /* 0x68 */ u32 m_fallSound;
    /* 0x6C */ u32 m_fallWaterSound;
    /* 0x70 */ u32 field_0x70;
    /* 0x74 */ f32 field_0x74;
    /* 0x78 */ s16 m_rollAngle;
    /* 0x7A */ s16 m_urnRotateFactor;
    /* 0x7C */ s16 field_0x7c;
    /* 0x80 */ f32 m_slopeInfluence;
    /* 0x84 */ f32 m_friction;
    /* 0x88 */ f32 field_0x88;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ u32 m_heapSize;
};

/**
 * @ingroup actors-objects
 * @class daObjCarry_c
 * @brief Carryable Object
 *
 * @details This is a multi-use class for carryable objects.
 * It's used by: small and large blue pot, small and large red pot, box, cannon ball, barrel, skull,
 * deku nut, Sols, small and large Twilight pots.
 *
 */
class daObjCarry_c : public fopAc_ac_c {
public:
    enum {
        /* 0x0 */ TYPE_TSUBO,     /**< Small Blue Pot */
        /* 0x1 */ TYPE_OOTSUBO,   /**< Big Red Pot */
        /* 0x2 */ TYPE_KIBAKO,    /**< Box */
        /* 0x3 */ TYPE_IRON_BALL, /**< Cannon Ball */
        /* 0x4 */ TYPE_TARU,      /**< Barrel */
        /* 0x5 */ TYPE_DOKURO,    /**< Skull */
        /* 0x6 */ TYPE_BOKKURI,   /**< Deku Nut */
        /* 0x7 */ TYPE_TSUBO_2,   /**< Small Red Pot */
        /* 0x8 */ TYPE_BALL_S,    /**< Light Ball A */
        /* 0x9 */ TYPE_BALL_S_2,  /**< Light Ball B */
        /* 0xA */ TYPE_AOTSUBO,   /**< Big Blue Pot */
        /* 0xB */ TYPE_LV8_BALL,  /**< Light Ball? Probably unused */
        /* 0xC */ TYPE_TSUBO_S,   /**< Small pot - Twilight */
        /* 0xD */ TYPE_TSUBO_B,   /**< Big pot - Twilight */
    };

    enum {
        MODE_WAIT,
        MODE_WALK,
        MODE_CARRY,
        MODE_DROP,
        MODE_FLOAT,
        MODE_SINK,
        MODE_YOGAN,
        MODE_MAGNE,
        MODE_MAGNE_CARRY,
        MODE_BOOM_CARRY,
        MODE_GROWTH,
        MODE_DB_DROP,
        MODE_HOOK_CARRY,
        MODE_END,
        MODE_FIT,
        MODE_CONTROLED,
        MODE_RESET_LIGHTBALL,
    };

    static void clrSaveFlag();
    static void setSaveFlag();
    static bool chkSaveFlag();
    static const cXyz& getPos(int);
    static void savePos(int, cXyz);
    static void onSttsFlag(int, u8);
    static void offSttsFlag(int, u8);
    static u8 chkSttsFlag(int, u8);
    static void setRoomNo(int, s8);
    static s8 getRoomNo(int);
    const daObjCarry_dt_t& data();
    char* getArcName();
    char* getBmdName();
    BOOL checkFlag(u8);
    void initBaseMtx();
    void setBaseMtx();
    int preInit();
    daObjCarry_c();
    BOOL checkBreakWolfAttack();
    BOOL checkCarryBoomerang();
    BOOL checkCarryHookshot();
    BOOL checkCarryWolf();
    BOOL checkCarryOneHand();
    int Create();
    int CreateInit_tsubo();
    int CreateInit_ootubo();
    int CreateInit_kibako();
    int CreateInit_ironball();
    int CreateInit_taru();
    int CreateInit_dokuro();
    int CreateInit_bokkuri();
    int CreateInit_LightBall();
    int CreateInit_Lv8Ball();
    int CreateHeap();
    int create();
    int checkCreate_LightBallA();
    int checkCreate_LightBallB();
    int checkCreate_Lv8Ball();
    void resetIconPosForLightBallA();
    void setIconPosForLightBallAAtR00();
    void setIconPosForLightBallBAtR00();
    void resetIconPosForLightBallB();
    int execute();
    BOOL checkCulling();
    int draw();
    void debugDraw();
    int _delete();
    void crr_pos();
    int check_sink(f32*);
    f32 calc_gravity();
    BOOL checkRollAngle();
    void mode_proc_call();
    void mode_init_wait();
    int mode_proc_wait();
    void mode_init_walk(u8);
    int mode_proc_walk();
    void mode_init_carry();
    int mode_proc_carry();
    void mode_init_drop(u8);
    int mode_proc_drop();
    void mode_init_float();
    int mode_proc_float();
    void mode_init_sink();
    int mode_proc_sink();
    void mode_init_yogan();
    int mode_proc_yogan();
    int mode_proc_magne();
    int mode_proc_magneCarry();
    void mode_init_boomCarry();
    int mode_proc_boomCarry();
    void mode_init_growth();
    int mode_proc_growth();
    int mode_proc_end();
    void mode_init_dbDrop(u8);
    int mode_proc_dbDrop();
    void mode_init_hookCarry();
    int mode_proc_hookCarry();
    void mode_init_fit();
    int mode_proc_fit();
    int mode_proc_controled();
    void mode_init_resetLightBall();
    int mode_proc_resetLightBall();
    BOOL chkSinkAll();
    BOOL chkWaterLineIn();
    BOOL chkSinkObj();
    void bg_check();
    bool check_bg_damage_proc_base();
    bool bg_damage_proc_kotubo();
    bool bg_damage_proc_ootubo();
    bool bg_damage_proc_kibako();
    bool bg_damage_proc_ironball();
    bool bg_damage_proc_taru();
    bool bg_damage_proc_dokuro();
    bool bg_damage_proc_bokkuri();
    bool bg_damage_proc_LightBall();
    bool bg_damage_proc_Lv8Ball();
    void obj_break(bool, bool, bool);
    bool check_cc_damage_proc_base(bool);
    bool cc_damage_proc_kotubo();
    bool cc_damage_proc_ootubo();
    bool cc_damage_proc_kibako();
    bool cc_damage_proc_ironball();
    bool cc_damage_proc_taru();
    bool cc_damage_proc_dokuro();
    bool cc_damage_proc_bokkuri();
    bool cc_damage_proc_LightBall();
    bool cc_damage_proc_Lv8Ball();
    void eff_break_tuboBmd(u16, cXyz);
    void eff_break_kibakoBmd(cXyz);
    void eff_break_tsubo();
    void eff_break_ootubo();
    void eff_break_kibako();
    void eff_break_ironball();
    void eff_break_taru();
    void eff_break_dokuro();
    void eff_break_bokkuri();
    void eff_break_kotubo2();
    void eff_break_LightBall();
    void eff_break_ootubo2();
    void eff_break_Lv8Ball();
    void eff_break_tsuboL8();
    void eff_break_ootuboL8();
    void se_break(cBgS_PolyInfo*);
    void se_put(cBgS_PolyInfo*);
    void se_put_water();
    void se_fall_water();
    void calc_rot_axis_base(u8);
    void calc_rot_axis_tsubo();
    void calc_rot_axis_ootubo();
    void calc_rot_axis_kibako();
    void calc_rot_axis_ironball();
    void calc_rot_axis_taru();
    void calc_rot_axis_dokuro();
    void calc_rot_axis_bokkuri();
    void calc_rot_axis_kotubo2();
    void calc_rot_axis_LightBall();
    void calc_rot_axis_Lv8Ball();
    void set_wind_power();
    void exec_proc_tsubo();
    void exec_proc_ootubo();
    void exec_proc_kibako();
    void exec_proc_ironball();
    void exec_proc_taru();
    void exec_proc_dokuro();
    void exec_proc_bokkuri();
    void exec_proc_LightBall();
    void exec_proc_Lv8Ball();
    void bg_damage_proc_call();
    void cc_damage_proc_call();
    void eff_break_call();
    void calc_rot_call();
    void obj_execute_proc_call();
    void setTgHitCallBack();
    void setCoHitCallBack();
    int CreateInitCall();

    s32 getType() { return mType; }
    u8 getSwbit() { return fopAcM_GetParamBit(this, 6, 8); }
    u32 getSwbit2() { return fopAcM_GetParamBit(this, 14, 8); }
    s8 getRoomNo() { return fopAcM_GetParamBit(this, 0, 6); }

    u32 checkOnMoveBg() { return ~((field_0xd18 >> 0xc) & 1); }
    u32 checkCrashRoll() { return field_0xd18 >> 6 & 1; }
    u8 getItemNo() { return mItemNo; }
    u8 getItemBit() { return mItemNo >> 8; }
    u8 getItemType() { return field_0xd18 & 1; }
    u8 getSaveID() { return getItemNo(); }
    u8 getTrboxBit() { return (mItemNo >> 8) & 0x3F; }
    u32 getSetType() { return (field_0xd18 >> 13) & 0x7; }
    bool prm_chk_type_ironball() { return getType() == TYPE_IRON_BALL; }
    bool prm_chk_type_lightball() { return getType() == TYPE_BALL_S || getType() == TYPE_BALL_S_2; }
    void startCtrl() { mCtrl = 1; }
    void endCtrl() { mCtrl = 0; }
    void setDrop() { field_0xcf2 = 3; }
    void offDraw() { mDraw = 1; }
    void onDraw() { mDraw = 0; }
    bool isDraw() { return mDraw == false; }
    void onCarryHookOK() { mCarryHookOK = 1; }
    void offCarryHookOK() { mCarryHookOK = 0; }
    BOOL checkRecover() { return mRecover; }
    void reset() { mReset = 1; }

    void setPower(fopAc_ac_c* cannon_actor, f32 my_0xde0, f32 my_0xde4, s16 my_0xde8) {
        mpCannonActor = cannon_actor;
        field_0xde0 = my_0xde0;
        field_0xde4 = my_0xde4;
        field_0xde8 = my_0xde8;
        field_0xdea = 1;
    }

    int checkCannon() {
        return mCannon;
    }

    static void make_prm(u32* o_params, csXyz* o_paramsEx, u8 param_2, u8 i_itemNo, u8 i_itemBit, u8 i_itemType, u8 param_6) {
        o_paramsEx->x = (i_itemBit << 8) | (i_itemNo & 0xFF);
        o_paramsEx->z = (param_6 << 13) | (param_2 << 1) | i_itemType;
    }

    static void make_prm_bokkuri(u32* o_params, csXyz* o_paramsEx, u8 i_itemNo, u8 i_itemBit, u8 i_itemType, u8 param_5) {
        make_prm(o_params, o_paramsEx, 6, i_itemNo, i_itemBit, i_itemType, param_5);
    }

    static const daObjCarry_dt_t mData[];
    static cXyz mPos[5];
    static u8 mSttsFlag[5];
    static s8 mRoomNo[5];
    static bool mSaveFlag;

public:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ dBgS_ObjAcch mAcch;
    /* 0x74C */ dBgS_AcchCir mAcchCir;
    /* 0x78C */ dCcD_Stts mStts;
    /* 0x7C8 */ dCcD_Cyl mCyl;
    /* 0x904 */ dCcD_Sph mLightAtSph;
    /* 0xA3C */ dCcD_Cps mAtCps;
    /* 0xB80 */ dCcD_Cyl mTgCyl;
    /* 0xCBC */ u8 field_0xCBC[0xCEC - 0xCBC];
    /* 0xCEC */ f32 field_0xcec;
    /* 0xCF0 */ u8 mType;
    /* 0xCF1 */ u8 mMode;
    /* 0xCF2 */ u8 field_0xcf2;
    /* 0xCF3 */ u8 field_0xcf3;
    /* 0xCF4 */ cXyz mRotAxis;
    /* 0xD00 */ s16 mRotation;
    /* 0xD02 */ u8 field_0xD02[0xD04 - 0xD02];
    /* 0xD04 */ s16 field_0xd04;
    /* 0xD08 */ cXyz field_0xd08;
    /* 0xD14 */ u8 field_0xd14;
    /* 0xD15 */ bool mInitParams;
    /* 0xD16 */ u16 mItemNo;
    /* 0xD18 */ u16 field_0xd18;
    /* 0xD1A */ u8 field_0xD1A[0xD1C - 0xD1A];
    /* 0xD1C */ cXyz field_0xd1c;
    /* 0xD28 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0xD34 */ u8 field_0xD34[0xD3C - 0xD34];
    /* 0xD3C */ Quaternion field_0xd3c;
    /* 0xD4C */ Quaternion field_0xd4c;
    /* 0xD5C */ f32 field_0xd5c;
    /* 0xD60 */ f32 field_0xd60;
    /* 0xD64 */ s16 field_0xd64;
    /* 0xD66 */ s16 field_0xd66;
    /* 0xD68 */ s16 field_0xd68;
    /* 0xD6A */ s16 field_0xd6a;
    /* 0xD6C */ f32 field_0xd6c;
    /* 0xD70 */ f32 field_0xd70;
    /* 0xD74 */ u8 field_0xd74;
    /* 0xD75 */ u8 field_0xd75;
    /* 0xD76 */ u8 field_0xd76;
    /* 0xD77 */ u8 field_0xd77;
    /* 0xD78 */ u8 field_0xD78[0xD79 - 0xD78];
    /* 0xD79 */ u8 field_0xd79;
    /* 0xD7A */ u8 field_0xd7a;
    /* 0xD7B */ s8 field_0xd7b;
    /* 0xD7C */ csXyz field_0xd7c;
    /* 0xD82 */ u8 field_0xD82[0xD84 - 0xD82];
    /* 0xD84 */ f32 field_0xd84;
    /* 0xD88 */ Z2SoundObjSimple mSound;
    /* 0xDA8 */ u8 field_0xda8;
    /* 0xDA9 */ u8 field_0xda9;
    /* 0xDAA */ u8 field_0xdaa;
    /* 0xDAB */ u8 field_0xdab;
    /* 0xDAC */ u8 field_0xdac;
    /* 0xDAD */ u8 field_0xdad;
    /* 0xDAE */ u8 field_0xdae;
    /* 0xDAF */ u8 field_0xdaf;
    /* 0xDB0 */ u8 field_0xdb0;
    /* 0xDB1 */ u8 field_0xdb1;
    /* 0xDB2 */ u8 mOnMoveBG;
    /* 0xDB3 */ u8 field_0xdb3;
    /* 0xDB4 */ u8 field_0xdb4;
    /* 0xDB5 */ bool mCanCrashRoll;
    /* 0xDB6 */ u8 mDeleteTimer;
    /* 0xDB7 */ bool mDraw;
    /* 0xDB8 */ u8 mCtrl;
    /* 0xDB9 */ u8 field_0xdb9;
    /* 0xDBA */ bool mReset;
    /* 0xDBB */ u8 mCarryHookOK;
    /* 0xDBC */ u8 mRecover;
    /* 0xDBD */ u8 field_0xDBD[0xDC4 - 0xDBD];
    /* 0xDC4 */ int field_0xdc4;
    /* 0xDC8 */ f32 field_0xdc8;
    /* 0xDCC */ f32 field_0xdcc;
    /* 0xDD0 */ fopAc_ac_c* mpCannonActor;
    /* 0xDD4 */ u32 field_0xdd4;
    /* 0xDD8 */ u8 field_0xDD8[0xDDC - 0xDD8];
    /* 0xDDC */ f32 field_0xddc;
    /* 0xDE0 */ f32 field_0xde0;
    /* 0xDE4 */ f32 field_0xde4;
    /* 0xDE8 */ s16 field_0xde8;
    /* 0xDEA */ u8 field_0xdea;
    /* 0xDEB */ bool mCannon;
    /* 0xDEC */ cXyz field_0xdec;
    /* 0xDF8 */ DALKMIST_INFLUENCE mDalkmistInf;
    /* 0xE0C */ u8 field_0xe0c;
    /* 0xE0D */ u8 field_0xe0d;
    /* 0xE0E */ u8 field_0xE0E[0xE10 - 0xE0E];
    /* 0xE10 */ dJntCol_c mJntCol;
    /* 0xE20 */ f32 field_0xe20;
    /* 0xE24 */ s8 field_0xe24;
    /* 0xE25 */ u8 field_0xe25;
    /* 0xE26 */ u8 field_0xe26;
    /* 0xE27 */ u8 field_0xe27;

private:
    u16 getType_private() { return field_0xd18 >> 1 & 0x1f; }
};

STATIC_ASSERT(sizeof(daObjCarry_c) == 0xE28);

#endif /* D_A_OBJ_CARRY_H */
