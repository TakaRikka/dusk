#ifndef D_A_OBJ_CRVSTEEL_H
#define D_A_OBJ_CRVSTEEL_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjCRVSTEEL_c
 * @brief Caravan Steel
 *
 * @details
 *
 */
class daObjCRVSTEEL_c : public dBgS_MoveBgActor {
public:
    void OpenSet(f32, f32);
    void CloseSet(f32);
    void CloseExecute();
    void OpenExecute();
    void Action();
    void setBaseMtx();
    int CreateHeap();
    int create();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

private:
    /* 0x5A0 */ u32 mSmokeParticle1;
    /* 0x5A4 */ u32 mSmokeParticle2;
    /* 0x5A8 */ s16 mAction;
    /* 0x5AC */ f32 mMaxHeight;
    /* 0x5B0 */ f32 mMinHeight;
    /* 0x5B4 */ f32 field_0x5b4;
    /* 0x5B8 */ f32 field_0x5b8;
    /* 0x5BC */ f32 field_0x5bc;
    /* 0x5C0 */ J3DModel* mModel;
    /* 0x5C4 */ request_of_phase_process_class mPhase;
    /* 0x5CC */ dBgS_AcchCir mAcchCir;
    /* 0x60C */ dBgS_ObjAcch mAcch;
    /* 0x7E4 */ dCcD_Stts mStts;
    /* 0x820 */ u8 field_0x820[0x848 - 0x820];
};

STATIC_ASSERT(sizeof(daObjCRVSTEEL_c) == 0x848);


#endif /* D_A_OBJ_CRVSTEEL_H */
