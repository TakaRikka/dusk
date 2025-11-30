#ifndef D_A_OBJ_KKANBAN_H
#define D_A_OBJ_KKANBAN_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjKKanban_c
 * @brief Kakariko Sign
 *
 * @details
 *
 */
class daObjKKanban_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    void setTG();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    Mtx& getMoveBgMtx() { return mBgMtx; }

    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dCcD_Stts mStts;
    /* 0x5E8 */ dCcD_Tri mTri;
    /* 0x744 */ u16 mJointIdx;
    /* 0x746 */ s16 mRotX;
    /* 0x748 */ s16 mRotZ;
    /* 0x74A */ s16 field_0x74a;
    /* 0x74C */ f32 field_0x74c;
    /* 0x750 */ s32 field_0x750;
};

STATIC_ASSERT(sizeof(daObjKKanban_c) == 0x754);

#endif /* D_A_OBJ_KKANBAN_H */
