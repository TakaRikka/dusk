#ifndef D_A_OBJ_KABUTO_H
#define D_A_OBJ_KABUTO_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_player.h"
#include "d/d_insect.h"

/**
 * @ingroup actors-objects
 * @class daObjKABUTO_c
 * @brief Insect - Beetle
 *
 * @details
 *
 */
class daObjKABUTO_c : public dInsect_c {
public:
    enum Action {
        /* 0 */ ACT_WAIT,
        /* 1 */ ACT_MOVE,
        /* 2 */ ACT_WALK,
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
    void Z_BufferChk();
    void Action();
    void ShopAction();
    void Insect_Release();
    void ParticleSet();
    int Execute();
    void ObjHit();
    int Delete();
    void setBaseMtx();
    bool CreateChk();
    cPhs__Step create();
    inline int Draw();

private:
    /* 0x590 */ dCcD_Stts mCcStts;
    /* 0x5CC */ dCcD_Sph mCcSph;
    /* 0x704 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x710 */ u8 mAction;
    /* 0x711 */ u8 mMode;
    /* 0x712 */ s16 mTimers[3];
    /* 0x718 */ f32 mTargetSpeedXZ;
    /* 0x71C */ f32 mTargetSpeedY;
    /* 0x720 */ s16 mTargetAngleY;
    /* 0x722 */ u8 field_0x722[8];
    /* 0x72A */ csXyz mWallAngle;
    /* 0x730 */ u8 field_0x730[2];
    /* 0x732 */ s16 mDownAngleY;
    /* 0x734 */ u8 field_0x734;
    /* 0x735 */ bool field_0x735;
    /* 0x736 */ u8 mLocation;
    /* 0x737 */ bool mBoomerangHit;
    /* 0x738 */ dBgS_AcchCir mAcchCir;
    /* 0x778 */ f32 mScreenZ;
    /* 0x77C */ u32 mBufferZ;
    /* 0x780 */ f32 mParticleScale;
    /* 0x784 */ f32 mParticleTargetScale;
    /* 0x788 */ u8 field_0x788[4];
    /* 0x78C */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x790 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x794 */ u8 field_0x794[4];
    /* 0x798 */ dBgS_ObjAcch mAcch;
    /* 0x970 */ Z2Creature mCreatureSound;
    /* 0xA00 */ u8 field_0xa00[4];
    /* 0xA04 */ mDoExt_McaMorfSO* mpMorf;
    /* 0xA08 */ request_of_phase_process_class mPhaseReq;
    /* 0xA10 */ bool mHIOInit;
 };

STATIC_ASSERT(sizeof(daObjKABUTO_c) == 0xA14);

class daObj_KabHIO_c {
public:
    daObj_KabHIO_c();
    virtual ~daObj_KabHIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
};

STATIC_ASSERT(sizeof(daObj_KabHIO_c) == 0x14);

#endif /* D_A_OBJ_KABUTO_H */
