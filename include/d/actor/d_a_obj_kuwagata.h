#ifndef D_A_OBJ_KUWAGATA_H
#define D_A_OBJ_KUWAGATA_H

#include "SSystem/SComponent/c_phase.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_insect.h"

/**
 * @ingroup actors-objects
 * @class daObjKUW_c
 * @brief Insect - Stag Beetle
 *
 * @details
 *
 */
class daObjKUW_c : public dInsect_c {
public:
    enum Action {
        /* 0 */ ACTION_WAIT,
        /* 1 */ ACTION_MOVE,
        /* 2 */ ACTION_WALK,
    };

    void InitCcSph();
    void SetCcSph();
    int CreateHeap();
    void ShopWaitAction();
    void WaitAction();
    BOOL WallCheck();
    void SpeedSet();
    void WallWalk();
    void WalkAction();
    void MoveAction();
    void Action();
    void ShopAction();
    void Insect_Release();
    void Z_BufferChk();
    void ParticleSet();
    void BoomChk();
    void ObjHit();
    int Execute();
    int Delete();
    void setBaseMtx();
    bool CreateChk();
    int create();
    inline int Draw();

private:
    /* 0x590 */ dCcD_Stts mStts;
    /* 0x5CC */ dCcD_Sph mSph;
    /* 0x704 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x710 */ dBgS_AcchCir mAcchCir;
    /* 0x750 */ dBgS_ObjAcch mAcch;
    /* 0x928 */ dBgS_GndChk mGndChk;
    /* 0x97C */ int field_0x97c;
    /* 0x980 */ u8 mAction;
    /* 0x981 */ u8 mSubAction;
    /* 0x982 */ s16 mTimers[3];
    /* 0x988 */ f32 mSpeedFTarget;
    /* 0x98C */ f32 mSpeedYTarget;
    /* 0x990 */ s16 mAngleTarget;
    /* 0x992 */ u8 field_0x992[0x99a - 0x992];
    /* 0x99A */ csXyz field_0x99a;
    /* 0x9A0 */ s16 field_0x9a0;
    /* 0x9A2 */ s16 field_0x9a2;
    /* 0x9A4 */ u8 field_0x9a4[0x9a8 - 0x9a4];
    /* 0x9A8 */ f32 mParticleScale;
    /* 0x9AC */ f32 mParticleScaleTarget;
    /* 0x9B0 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x9B4 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x9B8 */ f32 field_0x9b8;
    /* 0x9BC */ u32 field_0x9bc;
    /* 0x9C0 */ u8 field_0x9c0;
    /* 0x9C1 */ u8 field_0x9c1;
    /* 0x9C2 */ u8 field_0x9c2;
    /* 0x9C4 */ Z2Creature mSound;
    /* 0xA54 */ int field_0xa54;
    /* 0xA58 */ mDoExt_McaMorfSO* mpMorf;
    /* 0xA5C */ request_of_phase_process_class mPhase;
    /* 0xA64 */ bool mIsHIOOwner;
};

STATIC_ASSERT(sizeof(daObjKUW_c) == 0xa68);

#endif /* D_A_OBJ_KUWAGATA_H */
