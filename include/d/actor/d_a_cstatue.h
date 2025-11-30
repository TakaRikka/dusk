#ifndef D_A_CSTATUE_H
#define D_A_CSTATUE_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_particle_copoly.h"
#include "f_op/f_op_actor.h"

enum daCstatue_FLG0 {
    daCstatue_FLG0_1 = 0x1,
    daCstatue_FLG0_2 = 0x2,
    daCstatue_FLG0_4 = 0x4,
    daCstatue_FLG0_8 = 0x8,
    daCstatue_FLG0_10 = 0x10,
    daCstatue_FLG0_20 = 0x20,
    daCstatue_FLG0_Demo = 0x40,
    daCstatue_FLG0_80 = 0x80,
    daCstatue_FLG0_100 = 0x100,
    daCstatue_FLG0_200 = 0x200,
    daCstatue_FLG0_400 = 0x400,
    daCstatue_FLG0_800 = 0x800,
    daCstatue_FLG0_1000 = 0x1000,
};

enum daCstatue_MoveMode {
    daCstatue_MoveMode_0,
    daCstatue_MoveMode_1,
    daCstatue_MoveMode_2,
    daCstatue_MoveMode_3,
};

enum daCstatue_DemoMode {
    daCstatue_DemoMode_0,
    daCstatue_DemoMode_1,
    daCstatue_DemoMode_2,
    daCstatue_DemoMode_3,
};

enum daCstatueWarpMode {
    daCstatueWarpMode_Active = 0x1,
    daCstatueWarpMode_Ground = 0x2,
};

enum daCstatueType {
    daCstatueType_Normal,
    daCstatueType_Small,
    daCstatueType_Normal2,
    daCstatueType_BossRight,
    daCstatueType_BossLeft,
    daCstatueType_N,
};

/**
 * @ingroup actors-unsorted
 * @class daCstatue_c
 * @brief Dominion Rod Statue
 *
 * @details
 *
 */
class daCstatue_c : public fopAc_ac_c {
public:
    void atHitCallback(fopAc_ac_c*);
    int createHeap();
    int create();
    ~daCstatue_c();
    void setRoomInfo();
    void setMatrix();
    void posMove();
    void setCollision();
    BOOL checkHammerReverse();
    void setDemo();
    void setAnime();
    int initBrk(u16);
    void initStopBrkBtk();
    void initStartBrkBtk();
    int execute();
    int draw();

    bool checkBossAtGroundHit() const { return mBossAtGndHit == 1; }
    bool checkNormalType() const { return mType == daCstatueType_Normal; }
    bool checkNotSmallType() const { return mType != daCstatueType_Small; }
    bool checkBossType() const {
        return mType == daCstatueType_BossLeft || mType == daCstatueType_BossRight;
    }
    cXyz& getBallPos() { return mBallPos; }
    void onWarpGround(const cXyz& pos) {
        mWarpTarget = pos;
        mWarpMode = daCstatueWarpMode_Active | daCstatueWarpMode_Ground;
    }
    void warpStart(const cXyz& pos) {
        mWarpTarget = pos;
        mWarpMode = daCstatueWarpMode_Active;
    }
    void onStateFlg0(daCstatue_FLG0 flg) { mStateFlg0 |= flg; }
    void offStateFlg0(daCstatue_FLG0 flg) { mStateFlg0 &= ~flg; }
    bool checkStateFlg0(daCstatue_FLG0 flg) const { return mStateFlg0 & flg; }
    void warpDelete() {
        onStateFlg0(daCstatue_FLG0_1);
        mWarpMode = daCstatueWarpMode_Active;
    }

    static u16 const m_bckIdxTable[daCstatueType_N][7];

private:
    /* 0x568 */ const char* mResName;
    /* 0x56C */ request_of_phase_process_class mPhaseReq;
    /* 0x574 */ J3DModel* mModel;
    /* 0x578 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x57C */ mDoExt_btkAnm mAnim1;
    /* 0x594 */ mDoExt_brkAnm mAnim2;
    /* 0x5AC */ dBgS_AcchCir mAcchCir[4];
    /* 0x6AC */ dBgS_StatueAcch mStatueAcch;
    /* 0x884 */ dCcD_Stts mStts;
    /* 0x8C0 */ dCcD_Cyl mCyl1;
    /* 0x9FC */ dCcD_Cyl* mCyl2;
    /* 0xA00 */ dCcD_Cps* mCps1;
    /* 0xA04 */ dCcD_Cps* mCps2;
    /* 0xA08 */ dCcD_Sph* mSph;
    /* 0xA0C */ mDoExt_invisibleModel mInvisible;
    /* 0xA14 */ Z2Creature mSound;
    /* 0xAA4 */ dPaPo_c mPaPo;
    /* 0xADC */ s8 mReverb;
    /* 0xADD */ u8 mType;
    /* 0xADE */ u8 mParam0;
    /* 0xADF */ u8 mCurrentAnim;
    /* 0xAE0 */ u8 mMoveMode;
    /* 0xAE1 */ u8 mWarpMode;
    /* 0xAE2 */ u8 mParam1;
    /* 0xAE3 */ u8 mDemoMode;
    /* 0xAE4 */ u8 mParam2;
    /* 0xAE5 */ u8 mBossAtGndHit;
    /* 0xAE6 */ u16 unused;
    /* 0xAE8 */ s16 mDemoTimer;
    /* 0xAEA */ s16 mTimer1;
    /* 0xAEC */ u32 mStateFlg0;
    /* 0xAF0 */ u32 mShadowKey;
    /* 0xAF4 */ cXyz mSomePos;
    /* 0xB00 */ f32 mControlDistanceOffset;
    /* 0xB04 */ f32 mTargetFrame;
    /* 0xB08 */ cXyz mLinkPos;
    /* 0xB14 */ cXyz mWarpTarget;
    /* 0xB20 */ cXyz mBallPos;
};

STATIC_ASSERT(sizeof(daCstatue_c) == 0xB2C);

#endif /* D_A_CSTATUE_H */
