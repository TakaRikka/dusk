#ifndef D_A_E_PZ_H
#define D_A_E_PZ_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class daE_PZ_c
 * @brief Phantom Zant
 * 
 * @details 
 * 
 */
class daE_PZ_c : public fopEn_enemy_c {
public:
    enum Action_e {
        ACTION_SEARCH_POINT_e,
        ACTION_OPENING_DEMO_e,
        ACTION_WAIT_e,
        ACTION_ATTACK_e,
        ACTION_DAMAGE_e,
        ACTION_DEAD_e,
    };

    void onBombArrowHit() { mBombArrowHit = true; }

    int draw();
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    void mEntrySUB(bool);
    bool mPzScaleSet(bool);
    void mEnemyScaleSet();
    u8 getCutType();
    void mSetFirePos();
    void damage_check();
    void executeSearchPoint();
    void executeOpeningDemo();
    void mPointerSet();
    void executeWait();
    void executeAttack();
    void executeDamage();
    void executeDead();
    void executeSummonsBullet();
    void executeBullet();
    void action();
    void action_fire();
    void fire_mtx_set();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();
    daE_PZ_c();

    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5B8 */ mDoExt_McaMorfSO* mpBallModelMorf;
    /* 0x5BC */ J3DModel* mpPortalModel;
    /* 0x5C0 */ mDoExt_btkAnm* mpPortalBtk;
    /* 0x5C4 */ mDoExt_brkAnm* mpPortalBrk[2];
    /* 0x5CC */ mDoExt_brkAnm* mpBallBrk;
    /* 0x5D0 */ mDoExt_btkAnm* mpPzBtk;
    /* 0x5D4 */ mDoExt_brkAnm* mpPzBrk;
    /* 0x5D8 */ int field_0x5d8;
    /* 0x5DC */ Z2CreatureEnemy field_0x5dc;
    /* 0x680 */ int mActionMode;
    /* 0x684 */ int mMoveMode;
    /* 0x688 */ u32 mShadowKey;
    /* 0x68C */ int mAnm;
    /* 0x690 */ cXyz mPointPos[10];
    /* 0x708 */ cXyz field_0x708[10];
    /* 0x780 */ u8 field_0x780[10];
    /* 0x78A */ u8 mPointNum;
    /* 0x78B */ u8 mCurrentPoint;
    /* 0x78C */ u8 field_0x78c;
    /* 0x790 */ cXyz field_0x790;
    /* 0x79C */ cXyz mPzScale;
    /* 0x7A8 */ cXyz field_0x7a8;
    /* 0x7B4 */ u8 field_0x7B4[0x7BC - 0x7B4];
    /* 0x7BC */ f32 field_0x7bc;
    /* 0x7C0 */ f32 field_0x7c0;
    /* 0x7C4 */ f32 field_0x7c4;
    /* 0x7C8 */ f32 field_0x7c8;
    /* 0x7CC */ f32 field_0x7cc;
    /* 0x7D0 */ u8 field_0x7d0;
    /* 0x7D1 */ u8 field_0x7d1;
    /* 0x7D2 */ u8 field_0x7d2;
    /* 0x7D3 */ u8 field_0x7d3;
    /* 0x7D4 */ u8 arg0;
    /* 0x7D5 */ u8 arg1;
    /* 0x7D6 */ u8 bitSw;
    /* 0x7D7 */ u8 field_0x7d7;
    /* 0x7D8 */ u8 field_0x7d8;
    /* 0x7DC */ fpc_ProcID field_0x7dc[25];
    /* 0x840 */ u8 field_0x840;
    /* 0x841 */ u8 field_0x841;
    /* 0x842 */ u8 field_0x842;
    /* 0x843 */ u8 field_0x843;
    /* 0x844 */ u8 field_0x844;
    /* 0x845 */ u8 mBombArrowHit;
    /* 0x846 */ u8 field_0x846;
    /* 0x847 */ u8 field_0x847;
    /* 0x848 */ u8 field_0x848;
    /* 0x84C */ int field_0x84c;
    /* 0x850 */ dBgS_AcchCir mAcchCir;
    /* 0x890 */ dBgS_ObjAcch mAcch;
    /* 0xA68 */ u32 field_0xa68[3];
    /* 0xA74 */ u32 field_0xa74[2];
    /* 0xA7C */ u8 field_0xA7C[0xAB4 - 0xA7C];
    /* 0xAB4 */ u32 field_0xab4[2];
    /* 0xABC */ cXyz mDemoCameraEye;
    /* 0xAC8 */ cXyz mDemoCameraCenter;
    /* 0xAD4 */ dCcD_Stts mColliderStts;
    /* 0xB10 */ dCcD_Cyl mCylCollider;
    /* 0xC4C */ dCcD_Cyl mWarpCylCollider;
    /* 0xD88 */ dCcD_Sph mBulletAtCollider;
    /* 0xEC0 */ dCcU_AtInfo mAtInfo;
    /* 0xEE4 */ u8 mInitHIO;
    /* 0xEE5 */ u8 field_0xee5[0xEF8 - 0xEE5];
};

STATIC_ASSERT(sizeof(daE_PZ_c) == 0xEF8);

#endif /* D_A_E_PZ_H */
