#ifndef D_A_OBJ_ZRA_ROCK_H
#define D_A_OBJ_ZRA_ROCK_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjZraRock_c
 * @brief Zora Rock
 *
 * @details
 *
 */
class daObjZraRock_c : public dBgS_MoveBgActor {
public:
    void setAttnPos();
    void setBaseMtx();
    void setEnvTevColor();
    void setRoomNo();
    int Create();
    int CreateHeap();
    cPhs__Step create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    void setDemoStart() { mDemoStart = 1; }

private:
    friend class daNpc_zrZ_c;

    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModelXlu;
    /* 0x5AC */ J3DModel* mpModelOpa;
    /* 0x5B0 */ mDoExt_brkAnm mBrkAnm;
    /* 0x5C8 */ dBgS_ObjAcch mObjAcch;
    /* 0x7A0 */ dBgS_AcchCir mAcchCir;
    /* 0x7E0 */ cBgS_GndChk mGndChk;
    /* 0x81C */ bool mDemoStart;
};

STATIC_ASSERT(sizeof(daObjZraRock_c) == 0x820);

#endif /* D_A_OBJ_ZRA_ROCK_H */
