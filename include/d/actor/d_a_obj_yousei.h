#ifndef D_A_OBJ_YOUSEI_H
#define D_A_OBJ_YOUSEI_H

#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_player.h"

/**
 * @ingroup actors-objects
 * @class daObjYOUSEI_c
 * @brief Fairy
 *
 * @details
 *
 */
class daObjYOUSEI_c : public fopEn_enemy_c {
public:
    void InitCcSph();
    void SetCcSph();
    void SpeedSet();
    void MoveAction();
    void BinAction();
    void LinkChk();
    void WaitAction();
    void LinkAction();
    bool LinkSearch();
    bool WayBgCheck(f32, f32, s16);
    void CareAction();
    void CheckGround();
    bool CheckWater();
    void Action();
    void ObjHit();
    int Execute();
    int Delete();
    void setBaseMtx();
    int create();

    inline int CreateHeap();
    inline int Draw();

    /* 0x5AC */ u8 field_0x5ac;
    /* 0x5AD */ u8 field_0x5ad;
    /* 0x5AE */ u8 field_0x5ae;
    /* 0x5AF */ u8 field_0x5af;
    /* 0x5B0 */ cXyz field_0x5b0;
    /* 0x5BC */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x5C8 */ u8 mPrm;
    /* 0x5CA */ s16 mAction;
    /* 0x5CC */ s16 mMode;
    /* 0x5CE */ s16 mTimers[4];
    /* 0x5D6 */ s16 field_0x5d6;
    /* 0x5D8 */ u8 field_0x5D8[0x5DA - 0x5D8];
    /* 0x5DA */ s16 mAngleTarget;
    /* 0x5DC */ s16 field_0x5dc;
    /* 0x5E0 */ f32 mSpeedVTarget;
    /* 0x5E4 */ f32 mSpeedFTarget;
    /* 0x5E8 */ f32 field_0x5e8;
    /* 0x5EC */ f32 field_0x5ec;
    /* 0x5F0 */ f32 mAnmSpeed;
    /* 0x5F4 */ u8 field_0x5F4[0x5F8 - 0x5F4];
    /* 0x5F8 */ cXyz field_0x5f8;
    /* 0x604 */ u32 field_0x604;
    /* 0x608 */ u32 field_0x608;
    /* 0x60C */ u32 field_0x60c;
    /* 0x610 */ u32 field_0x610;
    /* 0x614 */ f32 mDistToPlayer;
    /* 0x618 */ s16 mAngleToPlayer;
    /* 0x61A */ s16 field_0x61a;
    /* 0x61C */ Z2Creature mSound;
    /* 0x6AC */ u8 field_0x6AC[0x6B0 - 0x6AC];
    /* 0x6B0 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x6B4 */ u8 field_0x6B4[0x6BC - 0x6B4];
    /* 0x6BC */ dBgS_AcchCir mAcchCir;
    /* 0x6FC */ dBgS_ObjAcch mAcch;
    /* 0x8D4 */ dCcD_Stts mCcStts;
    /* 0x910 */ dCcD_Sph mCcSph;
    /* 0xA48 */ u8 field_0xa48[0xA4C - 0xA48];
};

STATIC_ASSERT(sizeof(daObjYOUSEI_c) == 0xa4c);


#endif /* D_A_OBJ_YOUSEI_H */
