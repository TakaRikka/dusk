#ifndef D_A_E_HM_H
#define D_A_E_HM_H

#include "f_op/f_op_actor_mng.h"

#include <d/d_cc_uty.h>

/**
 * @ingroup actors-enemies
 * @class daE_HM_c
 * @brief Torch Slug
 *
 * @details
 *
 */
class daE_HM_c : public fopEn_enemy_c {
public:
    int CreateHeap();
    void initCcCylinder();
    void setCcCylinder();
    int ctrlJoint(J3DJoint*, J3DModel*);
    void Particle_Set(u32&, u16);
    void SetAnm(int, int, f32, f32);
    s16 W_TargetAngle(cXyz, cXyz);
    void W_DeathSpSet();
    void W_DeathAction();
    void W_DeathMotion();
    int W_MoveCheckWall();
    int W_WallCheck();
    void WallExecute();
    void WallCreateExecute();
    void CreateExecute();
    void Tyakuchi_Set();
    void UpDeathAction();
    void UpMoveAction();
    void UpWaitAction();
    void UpExecute();
    int WallCheck();
    int MoveCheck();
    void ShippuAction();
    void ShieldMotion();
    void ShieldAction();
    void DeathSpSet();
    void DeathMotion();
    void DeathCutDown();
    void DeathAction();
    void AttackAfter();
    void AttackMotion();
    void AttackAction();
    void FreeMove();
    void LinkSearch();
    void WaitAction();
    void MemberClear();
    void DownExecute();
    void At_Check();
    void Obj_Damage();
    void ActionMode();
    void Yazirushi();
    void setStabPos();
    int Execute();
    int Draw();
    int Delete();
    void setBaseMtx();
    void CheckRoof();
    void CheckWall();
    void CreateStyle();
    int Create();
private:
    /* 0x5AC */ u8 field_0x5ac;
    /* 0x5AD */ u8 field_0x5ad;
    /* 0x5AE */ u8 field_0x5ae;
    /* 0x5AF */ u8 field_0x5af;
    /* 0x5B0 */ f32 field_0x5b0;
    /* 0x5B4 */ s16 field_0x5b4;
    /* 0x5B8 */ s32 field_0x5b8;
    /* 0x5BC */ s32 field_0x5bc;
    /* 0x5C0 */ s16 field_0x5c0;
    /* 0x5C2 */ s16 field_0x5c2;
    /* 0x5C4 */ fpc_ProcID field_0x5c4;
    /* 0x5C8 */ u8 field_0x5c8[0x5cc - 0x5c8];
    /* 0x5CC */ csXyz field_0x5cc;
    /* 0x5D2 */ s16 field_0x5d2;
    /* 0x5D4 */ s16 field_0x5d4;
    /* 0x5D6 */ s16 field_0x5d6;
    /* 0x5D8 */ u8 field_0x5d8[0x5da - 0x5d8];
    /* 0x5DA */ u8 field_0x5da;
    /* 0x5DC */ cXyz field_0x5dc;
    /* 0x5E8 */ cXyz field_0x5e8;
    /* 0x5F4 */ f32 field_0x5f4;
    /* 0x5F8 */ f32 field_0x5f8;
    /* 0x5FC */ u32 field_0x5fc;
    /* 0x600 */ u32 field_0x600;
    /* 0x604 */ s32 field_0x604;
    /* 0x608 */ u32 field_0x608;
    /* 0x60C */ u8 field_0x60c;
    /* 0x60D */ u8 field_0x60d;
    /* 0x610 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x614 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x618 */ mDoExt_McaMorfSO* mAnm_p;
    /* 0x61C */ s32 field_0x61c;
    /* 0x620 */ u8 field_0x620;
    /* 0x624 */ request_of_phase_process_class mPhase;
    /* 0x62C */ u32 field_0x62c;
    /* 0x630 */ Z2CreatureEnemy mSound;
    /* 0x6D4 */ dBgS_AcchCir mAcchCir;
    /* 0x714 */ dBgS_ObjAcch mAcch;
    /* 0x8EC */ dCcD_Stts mStts;
    /* 0x928 */ dCcD_Sph mSph;
    /* 0xA60 */ dCcU_AtInfo mAtInfo;
    /* 0xA84 */ u8 field_0xa84;
};

STATIC_ASSERT(sizeof(daE_HM_c) == 0xa88);


#endif /* D_A_E_HM_H */
