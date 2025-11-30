#ifndef D_A_OBJ_ARI_H
#define D_A_OBJ_ARI_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_player.h"
#include "d/d_insect.h"

/**
 * @ingroup actors-objects
 * @class daObjARI_c
 * @brief Insect - Ant
 *
 * @details
 *
 */
class daObjARI_c : public dInsect_c {
public:
    void InitCcSph();
    void SetCcSph();
    int ctrlJoint(J3DJoint*, J3DModel*);
    int CreateHeap();
    void WallWalk();
    void MoveAction();
    void Action();
    void Insect_Release();
    void checkGround();
    void B_MoveAction();
    void FallAction();
    void ParticleSet();
    void BoomChk();
    void ObjHit();
    int Execute();
    void Z_BufferChk();
    int Delete();
    void setBaseMtx();
    /*  inline  */ inline int Draw();
    bool CreateChk();
    cPhs__Step create();

private:
    /* 0x590 */ dBgS_GndChk mGndChk;
    /* 0x5E4 */ u8 field_0x5e4[4];
    /* 0x5E8 */ u8 mAction;
    /* 0x5E9 */ u8 mMode;
    /* 0x5EA */ s16 mTimer;
    /* 0x5EC */ f32 field_0x5ec;
    /* 0x5F0 */ u8 field_0x5f0[4];
    /* 0x5F4 */ s16 mTargetAngleY;
    /* 0x5F6 */ u8 field_0x5f6[6];
    /* 0x5FC */ bool field_0x5fc;
    /* 0x5FE */ csXyz mWallAlignAngle;
    /* 0x604 */ csXyz mWallAngle;
    /* 0x60A */ u8 field_0x60a[2];
    /* 0x60C */ s16 mLegAngleY;
    /* 0x60E */ s16 mDownAngleY;
    /* 0x610 */ u8 field_0x610[4];
    /* 0x614 */ cXyz mTargetPos;
    /* 0x620 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x624 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x628 */ f32 mScreenZ;
    /* 0x62C */ u32 mBufferZ;
    /* 0x630 */ u8 mSex;  // used instead of the variable from dInsect_c
    /* 0x631 */ u8 mLocation;
    /* 0x634 */ f32 mParticleScale;
    /* 0x638 */ f32 mParticleTargetScale;
    /* 0x63C */ dCcD_Stts mCcStts;
    /* 0x678 */ dCcD_Sph mCcSph;
    /* 0x7B0 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x7BC */ bool mBoomerangHit;
    /* 0x7C0 */ Z2Creature mCreatureSound;
    /* 0x850 */ dBgS_AcchCir mAcchCir;
    /* 0x890 */ dBgS_ObjAcch mAcch;
    /* 0xA68 */ mDoExt_McaMorfSO* mpMorf;
    /* 0xA6C */ request_of_phase_process_class mPhase;
    /* 0xA74 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(daObjARI_c) == 0xA78);

class daObj_AriHIO_c {
public:
    daObj_AriHIO_c();
    virtual ~daObj_AriHIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mScaleFemale;
    /* 0xC */ f32 mScaleMale;
};

STATIC_ASSERT(sizeof(daObj_AriHIO_c) == 0x10);

#endif /* D_A_OBJ_ARI_H */
