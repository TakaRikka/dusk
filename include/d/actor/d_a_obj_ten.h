#ifndef D_A_OBJ_TEN_H
#define D_A_OBJ_TEN_H

#include "d/actor/d_a_player.h"
#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_cc_d.h"
#include "d/d_insect.h"

/**
 * @ingroup actors-objects
 * @class daObjTEN_c
 * @brief Insect - Ladybug
 *
 * @details
 *
 */
class daObjTEN_c : public dInsect_c {
public:
    enum Action {
        ACTION_WAIT,
        ACTION_MOVE,
        ACTION_WALK,
    };

    void InitCcSph();
    void SetCcSph();
    void WaitAction();
    BOOL WallCheck();
    void SpeedSet();
    void WallWalk();
    void WalkAction();
    void MoveAction();
    void Action();
    void ShopAction();
    void checkGroundPos();
    virtual void Insect_Release();
    void ParticleSet();
    void BoomChk();
    int Execute();
    void ObjHit();
    void Z_BufferChk();
    int Delete();
    void setBaseMtx();
    bool CreateChk();
    int create();

    /* 0x590 */ dBgS_GndChk mGndChk;
    /* 0x5E4 */ f32 field_0x5e4;
    /* 0x5E8 */ u8 mAction;
    /* 0x5E9 */ u8 field_0x5e9;
    /* 0x5EA */ s16 field_0x5ea[3];
    /* 0x5F0 */ f32 field_0x5f0;
    /* 0x5F4 */ f32 field_0x5f4;
    /* 0x5F8 */ s16 field_0x5f8;
    /* 0x5EC */ u8 field_0x5fa[0x602 - 0x5fa];
    /* 0x602 */ csXyz field_0x602;
    /* 0x608 */ s16 field_0x608;
    /* 0x60A */ s16 field_0x60a;
    /* 0x60C */ u8 field_0x60c;
    /* 0x60D */ u8 field_0x60d;
    /* 0x610 */ mDoExt_brkAnm* mBrk;
    /* 0x614 */ mDoExt_btkAnm* mBtk;
    /* 0x618 */ f32 field_0x618;
    /* 0x61C */ u32 field_0x61c;
    /* 0x620 */ f32 mParticleScale;
    /* 0x624 */ u8 field_0x624;
    /* 0x628 */ dCcD_Stts mStts;
    /* 0x664 */ dCcD_Sph mSph;
    /* 0x79C */ daPy_boomerangMove_c mBoomerang;
    /* 0x7A8 */ u8 field_0x7a8;
    /* 0x7AC */ dBgS_ObjAcch mAcch;
    /* 0x984 */ dBgS_AcchCir mAcchCir;
    /* 0x9C4 */ Z2Creature mCreature;
    /* 0xA54 */ int field_0xa54;
    /* 0xA58 */ mDoExt_McaMorfSO* mMorf;
    /* 0xA5C */ request_of_phase_process_class mPhase;
    /* 0xA64 */ u8 field_0xa64;
};

STATIC_ASSERT(sizeof(daObjTEN_c) == 0xa68);

class daObj_TenHIO_c {
public:
    daObj_TenHIO_c();
    virtual ~daObj_TenHIO_c() {}

    s8 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
};


#endif /* D_A_OBJ_TEN_H */
