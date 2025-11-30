#ifndef D_A_OBJ_TOMBO_H
#define D_A_OBJ_TOMBO_H

#include "JSystem/JHostIO/JORReflexible.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_insect.h"

/**
 * @ingroup actors-objects
 * @class daObjTOMBO_c
 * @brief Insect - Dragonfly (Tonbo)
 *
 * @details
 *
 */
class daObjTOMBO_c : public dInsect_c {
public:
    enum Action {
        ACTION_WAIT,
        ACTION_MOVE,
        ACTION_LINK,
    };

    void InitCcSph();
    void SetCcSph();
    int CreateHeap();
    void WaitAction();
    void SpeedSet();
    bool CheckWater();
    void CheckGround();
    void CheckWall();
    void SearchLink();
    void LinkAction();
    void MoveAction();
    void Action();
    void ShopAction();
    void Insect_Release();
    void ParticleSet();
    void BoomChk();
    int Execute();
    void ObjHit();
    void Z_BufferChk();
    int Delete();
    void setBaseMtx();
    bool CreateChk();
    int create();

    inline int Draw();

private:
    /* 0x590 */ dCcD_Stts mStts;
    /* 0x5CC */ dCcD_Sph mSph;
    /* 0x704 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x710 */ bool mIsHitByBoomerang;
    /* 0x711 */ u8 mAction;
    /* 0x712 */ u8 field_0x712;
    /* 0x714 */ s16 field_0x714[2];
    /* 0x718 */ s16 field_0x718;
    /* 0x71C */ f32 field_0x71c;
    /* 0x720 */ f32 field_0x720;
    /* 0x724 */ s16 field_0x724;
    /* 0x726 */ u8 field_0x726[0x73c - 0x726];
    /* 0x73C */ cXyz field_0x73c;
    /* 0x748 */ mDoExt_brkAnm* mBrk;
    /* 0x74C */ mDoExt_btkAnm* mBtk;
    /* 0x750 */ f32 field_0x750;
    /* 0x754 */ u32 field_0x754;
    /* 0x758 */ f32 mParticleScale;
    /* 0x75C */ u8 field_0x75c;
    /* 0x760 */ Z2Creature mCreature;
    /* 0x7F0 */ dBgS_AcchCir mAcchCir;
    /* 0x830 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x834 */ dBgS_ObjAcch mAcch;
    /* 0xA0C */ request_of_phase_process_class mPhase;
    /* 0xA14 */ bool mIsHIOOwner;
};

STATIC_ASSERT(sizeof(daObjTOMBO_c) == 0xa18);

#endif /* D_A_OBJ_TOMBO_H */
