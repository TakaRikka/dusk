#ifndef D_A_OBJ_CHO_H
#define D_A_OBJ_CHO_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_player.h"
#include "d/d_insect.h"

/**
 * @ingroup actors-objects
 * @class daObjCHO_c
 * @brief Insect - Butterfly
 *
 * @details
 *
 */
class daObjCHO_c : public dInsect_c {
public:
    enum Action {
        /* 0 */ ACT_WAIT,
        /* 1 */ ACT_MOVE,
    };

    void InitCcSph();
    void SetCcSph();
    int CreateHeap();
    void SpeedSet();
    void WallCheck();
    void SearchLink();
    void WaitAction();
    void MoveAction();
    void Action();
    void ShopAction();
    void checkGroundPos();
    void Insect_Release();
    void Z_BufferChk();
    void ParticleSet();
    void BoomChk();
    int Execute();
    void ObjHit();
    int Delete();
    void setBaseMtx();
    bool CreateChk();
    cPhs__Step create();
    inline int Draw();

private:
    /* 0x590 */ dBgS_AcchCir mAcchCir;
    /* 0x5D0 */ dBgS_ObjAcch mAcch;
    /* 0x7A8 */ dCcD_Stts mCcStts;
    /* 0x7E4 */ dCcD_Sph mCcSph;
    /* 0x91C */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x928 */ dBgS_GndChk mGndChk;
    /* 0x97C */ f32 mGroundPos;
    /* 0x980 */ u8 mAction;
    /* 0x981 */ u8 mMode;
    /* 0x982 */ s16 mTimers[3];
    /* 0x988 */ f32 mTargetSpeedXZ;
    /* 0x98C */ f32 mTargetSpeedY;
    /* 0x990 */ s16 mTargetAngleY;
    /* 0x994 */ f32 mParticleScale;
    /* 0x998 */ f32 mScreenZ;
    /* 0x99C */ u32 mBufferZ;
    /* 0x9A0 */ u8 field_0x9a0[0xC];
    /* 0x9AC */ bool field_0x9ac;
    /* 0x9B0 */ f32 mPlaySpeed;
    /* 0x9B4 */ cXyz mTargetPos;
    /* 0x9C0 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x9C4 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x9C8 */ u8 mLocation;
    /* 0x9C9 */ bool mBoomerangHit;
    /* 0x9CC */ Z2Creature mCreatureSound;
    /* 0xA5C */ u8 field_0xa5c[4];
    /* 0xA60 */ mDoExt_McaMorfSO* mpMorf;
    /* 0xA64 */ request_of_phase_process_class mPhaseReq;
    /* 0xA6C */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(daObjCHO_c) == 0xA70);

class daObj_ChoHIO_c {
public:
    daObj_ChoHIO_c();
    virtual ~daObj_ChoHIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mScaleFemale;
    /* 0xC */ f32 mScaleMale;
};

STATIC_ASSERT(sizeof(daObj_ChoHIO_c) == 0x10);

#endif /* D_A_OBJ_CHO_H */
