#ifndef D_A_NPC_COACH_H
#define D_A_NPC_COACH_H

#include "d/actor/d_a_npc.h"
#include "d/d_path.h"

/**
 * @ingroup actors-npcs
 * @class daNpcCoach_c
 * @brief Coach
 *
 * @details
 *
*/

class daNpcCoach_Attr_c {
public:
    /* 0x00 */ f32 gravity;                 // 重力 - Gravity
    /* 0x04 */ f32 max_fall_speed;          // 最大落下速度 - Maximum Fall Speed
    /* 0x08 */ f32 jump_coeff;              // 跳係数 - Jump Coefficient
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 max_speed;               // 最大速度 - Maximum Speed
    /* 0x20 */ f32 spring_constant;         // バネ係数 - Spring Constant
    /* 0x24 */ f32 damp_coeff[1];           // 減衰係数 - Damp Coefficient
    /* 0x28 */ f32 field_0x28[1];
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 vert_swing_width;        // 縦揺れ幅 - Vertical Swing Width
    /* 0x3C */ f32 shake_dist;              // 揺れ距離 - Shake Distance
    /* 0x40 */ f32 jump_dist;               // 跳ね距離 - Jump Distance
    /* 0x44 */ f32 wheel_bounce;            // 車輪跳ね - Wheel Bounce
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ Vec field_0x4c;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 field_0x5c;
    /* 0x60 */ s16 field_0x60;
    /* 0x62 */ s16 horse_rot_limit;         // 馬Rot限度 - Horse Rot Limit
    /* 0x64 */ s16 horiz_sway_range;        // 横揺れ幅 - Horizontal Sway Range
    /* 0x66 */ s16 field_0x66;
    /* 0x68 */ s16 field_0x68;
    /* 0x6A */ s8 field_0x6a;
    /* 0x6B */ u8 field_0x6b;
    /* 0x6C */ u8 field_0x6c;
    /* 0x6D */ u8 shake_test;               // 揺れ実験 - Shake Test
    /* 0x6E */ s16 field_0x6e;
    /* 0x70 */ int damage_reduction;        // ダメージ減量 - Damage Reduction
    /* 0x74 */ int damage_durability;       // ダメージ耐久度 - Damage Durability
    /* 0x78 */ u8 damage_extinguish_level;  // ダメージ消火度 - Damage Extinguish Level
    /* 0x79 */ u8 horse_camera;             // 馬カメラ - Horse Camera
    /* 0x7A */ u8 field_0x7a;
    /* 0x7B */ u8 field_0x7b;
    /* 0x7C */ f32 camera_dist;             // カメラ距離 - Camera Distance
    /* 0x80 */ s16 camera_elevation_angle;  // カメラ仰角 - Camera Elevation Angle
    /* 0x82 */ s16 camera_rotation_angle;   // カメラ回転角 - Camera Rotation Angle
    /* 0x84 */ s16 field_0x84;
    /* 0x86 */ u8 field_0x86;
    /* 0x87 */ u8 field_0x87;
    /* 0x88 */ u8 field_0x88;
    /* 0x89 */ u8 field_0x89;
    /* 0x8A */ s16 field_0x8a;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ f32 deceleration_speed;      // 減速速度 - Deceleration Speed
    /* 0x9C */ s32 deceleration_time;       // 減速時間 - Deceleration Time
};

class daNpcChReins_c {
public:
    /* 0x000 */ mDoExt_3DlineMat1_c field_0x0;
    /* 0x03C */ mDoExt_3DlineMat1_c field_0x3c;
    /* 0x078 */ cXyz field_0x78[2][5];
    /* 0x0F0 */ cXyz field_0xf0[2][5];
    /* 0x168 */ f32 field_0x168;
    /* 0x16C */ s8 field_0x16c;
    /* 0x170 */ cXyz field_0x170;
};

class daNpcChHorse_c {
public:
    /* 0x000 */ mDoExt_McaMorf2* mpModelMorf;
    /* 0x004 */ mDoExt_btpAnm* mpBtpAnm;
    /* 0x008 */ mDoExt_bckAnm* mpBckAnm;
    /* 0x00C */ dBgS_ObjAcch mBgc;
    /* 0x1F0 */ dCcD_Cyl field_0x1f0[4];
    /* 0x6D4 */ Z2CreatureRide mSound;
    /* 0x770 */ cXyz field_0x770;
    /* 0x77C */ f32 mAnmRate;
    /* 0x780 */ f32 mAimSpeed;
    /* 0x784 */ f32 field_0x784;
    /* 0x788 */ int field_0x788;
    /* 0x78C */ int field_0x78c;
    /* 0x790 */ int field_0x790;
    /* 0x794 */ u32 mShadowKey;
    /* 0x798 */ daNpcChReins_c mChReins;
};

class daNpcChWheel_c {
public:
    /* 0x00 */ cXyz field_0x0;
    /* 0x0C */ u32 field_0xc[2];
    /* 0x14 */ csXyz mWheelRot;
    /* 0x1A */ s16 field_0x1a;
    /* 0x1C */ s16 field_0x1c;
    /* 0x1E */ s16 field_0x1e;
    /* 0x20 */ s16 field_0x20;
};

class daNpcChHarness_c {
public:
    /* 0x000 */ J3DModel* mHarnessModel;
    /* 0x004 */ dKy_tevstr_c mTevStr;
    /* 0x38C */ dBgS_ObjAcch mBgc;
    /* 0x564 */ dCcD_Cyl field_0x564;
    /* 0x6A0 */ dBgW* field_0x6a0;
    /* 0x6A4 */ Mtx mMtx;
    /* 0x6D4 */ cXyz field_0x6d4;
    /* 0x6E0 */ cXyz field_0x6e0;
    /* 0x6EC */ cXyz field_0x6ec;
    /* 0x6F8 */ u32 mShadowKey;
    /* 0x6FC */ f32 field_0x6fc;
    /* 0x700 */ csXyz field_0x700;
    /* 0x706 */ csXyz field_0x706;
    /* 0x70C */ daNpcChWheel_c field_0x70c;
    /* 0x730 */ daNpcChWheel_c field_0x730;
};

class daNpcChCoach_c {
public:
    /* 0x000 */ J3DModel* mCoachModel;
    /* 0x004 */ dKy_tevstr_c mTevStr;
    /* 0x38C */ dBgS_ObjAcch mBgc;
    /* 0x564 */ dBgW* field_0x564;
    /* 0x568 */ Mtx field_0x568;
    /* 0x598 */ Mtx field_0x598;
    /* 0x5C8 */ cXyz field_0x5c8;
    /* 0x5D4 */ cXyz field_0x5d4;
    /* 0x5E0 */ cXyz field_0x5e0;
    /* 0x5EC */ cXyz field_0x5ec;
    /* 0x5F8 */ JPABaseEmitter* field_0x5f8[10];
    /* 0x620 */ Z2SoundObjCoach mSound;
    /* 0x644 */ dCcD_Sph field_0x644;
    /* 0x77C */ int field_0x77c;
    /* 0x780 */ u32 mShadowKey;
    /* 0x784 */ f32 field_0x784;
    /* 0x788 */ f32 field_0x788;
    /* 0x78C */ f32 mCoachTrans;
    /* 0x790 */ f32 field_0x790;
    /* 0x794 */ f32 field_0x794;
    /* 0x798 */ s16 mCoachRot;
    /* 0x79A */ csXyz field_0x79a;
    /* 0x7A0 */ csXyz field_0x7a0;
    /* 0x7A8 */ daNpcChWheel_c field_0x7a8;
    /* 0x7CC */ daNpcChWheel_c field_0x7cc;
};

class daNpcChYelia_c {
public:
    /* 0x000 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x004 */ dKy_tevstr_c mTevStr;
    /* 0x38C */ Z2Creature mSound;
    /* 0x41C */ dCcD_Cyl field_0x41c[2];
    /* 0x694 */ cXyz field_0x694;
};

class daNpcChPath_c {
public:
    inline BOOL setPath(int, int, cXyz&, bool);
    BOOL isClose() { return dPath_ChkClose(mpPath); }
    bool setNextPoint() {
        mPntIndex++;

        if (mPntIndex >= mpPath->m_num) {
            if (isClose()) {
                mPntIndex = 0;
            } else {
                mPntIndex--;
                return FALSE;
            }
        }

        return TRUE;
    }
    BOOL setPrevPoint() {
        mPntIndex--;

        if (mPntIndex < 0) {
            if (isClose()) {
                mPntIndex = mpPath->m_num - 1;
            } else {
                mPntIndex++;
                return FALSE;
            }
        }

        return TRUE;
    }
    void getTargetPoint(cXyz& param_1) {
        if (mpPath != NULL) {
            param_1 = dPath_GetPnt(mpPath, mPntIndex)->m_position;
        }
    }
    bool isPath() { return mpPath != NULL; }
    int getCurrentId() { return mCurrentID; }
    s8 getArg0() { return mpPath->m_points[mPntIndex].mArg0; }
    s8 getArg1() { return mpPath->m_points[mPntIndex].mArg1; }
    s8 getArg2() { return mpPath->m_points[mPntIndex].mArg2; }
    s8 getArg3() { return mpPath->m_points[mPntIndex].mArg3; }
    inline s8 checkNearAttackPoint();
    inline bool checkPoint(cXyz*, f32);
    inline bool setNextTarget();

    /* 0x00 */ dPath* mpPath;
    /* 0x04 */ cXyz* field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ int mCurrentID;
    /* 0x10 */ int mPntIndex;
};

class daNpcCoach_c : public fopAc_ac_c {
public:
    void hitFireArrow(cXyz);
    void deleteFireArrow(unsigned int);
    bool deleteFireArrowFromList(unsigned int);
    void setPosAngle(cXyz&, csXyz&);
    int ctrlJointHorse(J3DJoint*, J3DModel*);
    void initCoachPosition(Vec&, SVec&);
    void changeAtherPath(s8, cXyz&, csXyz&);
    int createHeap();
    int execute();
    void checkCoachDamage();
    inline void setCoachBlazing(u8);
    void reinsExecute();
    void reinsInit();
    void calcCoachMotion();
    inline void calcRearWheelRotate();
    void calcHarnessMotion();
    void calcFrontWheelRotate();
    void calcHorseMotion();
    inline void calcHorseAnm();
    void calcHorsePath();
    int draw();
    ~daNpcCoach_c();
    cPhs__Step create();
    void create_init();
    inline void initCollision();
    void initBaseMtx();
    void setBaseMtx();
    void initCoachBlazing();
    daNpcCoach_c();

    bool checkCoachBlazing() { return mCoachBlazing; }
    Mtx* getCoachMtx() { return &mCoachMtx; }
    csXyz* getFrontWheelRot(int param_1) {
        return (param_1 == 2) ? &mChHarness.field_0x70c.mWheelRot
                              : &mChHarness.field_0x730.mWheelRot;
    }
    csXyz* getRearWheelRot(int param_1) {
        return (param_1 == 1) ? &mChCoach.field_0x7a8.mWheelRot
                              : &mChCoach.field_0x7cc.mWheelRot;
    }
    s16 getCoachRotate() { return mChCoach.mCoachRot; }
    f32 getCoachTrans() { return mChCoach.mCoachTrans; }
    int getRailID() { return mChPath.getCurrentId(); }

    inline void setDriverMtx();
    void calcDriverMotion() { setDriverMtx(); }
    inline void calcYeliaMotion();
    void setYeliaMtx() {
        mDoMtx_stack_c::copy(mChCoach.mCoachModel->getAnmMtx(6));
        mDoMtx_stack_c::multVecZero(&mChYelia.field_0x694);
        mChYelia.mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    inline void setCoachCollision();
    inline s16 getGroundSlope(dBgS_ObjAcch*, s16);
    inline void calcSpringF(f32*, f32, f32*);
    inline void calcSpringS(s16*, s16, f32*);
    inline void setCoachMtx();
    inline void setWheelSmoke(daNpcChWheel_c*, dBgS_ObjAcch*, dKy_tevstr_c*, csXyz*);
    inline void setHarnessCollision();
    inline f32 calcMaxSpeed();
    inline void setHorseCollision();
    inline void setHorseMtx();
    inline void setHorseAnm(int);
    inline void eyeWink();
    inline bool checkKargoAttack();
    inline BOOL setExpressionAnm(int, bool);
    inline void resetOverAngle();
    inline void reinsDraw();
    u8 getPathID() { return fopAcM_GetParam(this) & 0xFF; }
    s16 getMessageNo() { return home.angle.x; }
    inline void setHarnessMtx();
    void setAimSpeed(f32 i_aimSpeed) { mChHorse.mAimSpeed = i_aimSpeed; }

    const daNpcCoach_Attr_c& attr() const { return M_attr; }

    static daNpcCoach_Attr_c const M_attr;
    static u16 const ParticleName[10];
private:
    /* 0x0568 */ daNpcChHorse_c mChHorse;
    /* 0x0E7C */ daNpcChHarness_c mChHarness;
    /* 0x15D0 */ daNpcChCoach_c mChCoach;
    /* 0x1DC0 */ int field_0x1dc0;
    /* 0x1DC4 */ bool mCoachBlazing;
    /* 0x1DC5 */ u8 field_0x1dc5;
    /* 0x1DC6 */ u8 field_0x1dc6;
    /* 0x1DC7 */ u8 field_0x1dc7;
    /* 0x1DC8 */ daNpcChYelia_c mChYelia;
    /* 0x2468 */ daNpcChPath_c mChPath;
    /* 0x247C */ fpc_ProcID field_0x247c[5];
    /* 0x2490 */ Mtx mCoachMtx;
    /* 0x24C0 */ int field_0x24c0;
    /* 0x24C4 */ int mTotalDmgRecv;
    /* 0x24C8 */ f32 field_0x24c8;
    /* 0x24CC */ dBgS_AcchCir mAcchCir;
    /* 0x250C */ dCcD_Stts mStts;
    /* 0x2548 */ request_of_phase_process_class mPhase;
    /* 0x2550 */ int field_0x2550;
    /* 0x2554 */ fpc_ProcID field_0x2554;
    /* 0x2558 */ u8 field_0x2558;
    /* 0x2559 */ u8 field_0x2559;
    /* 0x255C */ cXyz field_0x255c;
    /* 0x2568 */ int mPathID;
    /* 0x256C */ u8 field_0x256c[0x2570 - 0x256c];
};

STATIC_ASSERT(sizeof(daNpcCoach_c) == 0x2570);

struct daNpcCoach_Hio_c {
public:
    void default_set() {
        field_0xc = daNpcCoach_c::M_attr;
    }

    /* 0x0 */ u8 field_0x0[0x8 - 0x0];
    /* 0x8 */ int field_0x8;
    /* 0xC */ daNpcCoach_Attr_c field_0xc;
};

#endif /* D_A_NPC_COACH_H */
