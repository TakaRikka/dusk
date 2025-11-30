#ifndef D_A_OBJ_GRAVE_STONE_H
#define D_A_OBJ_GRAVE_STONE_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_particle_copoly.h"

/**
 * @ingroup actors-objects
 * @class daGraveStone_c
 * @brief Zora Gravestone
 *
 * @details
 *
 */
class daGraveStone_c : public dBgS_MoveBgActor {
public:
    class daObj_GrvStn_prtclMngr_c {
    public:
        /* 0x00 */ bool field_0x00;
        /* 0x04 */ cXyz mPos;
        /* 0x10 */ csXyz mAngle;
        /* 0x18 */ cXyz scale;
        /* 0x24 */ dPaPo_c field_0x24;
    };  // Size: 0x5C

    void setBaseMtx();
    void moveCalc();
    int setPrtcl();
    void setEnvTevColor();
    void setRoomNo();
    int Create();
    int CreateHeap();
    cPhs__Step create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

private:
    friend class daNpc_zrZ_c;

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dBgS_ObjAcch mAcch;
    /* 0x784 */ dCcD_Stts mColStatus;
    /* 0x7C0 */ dBgS_AcchCir mAcchCir;
    /* 0x800 */ dCcD_Cyl mColCyl;
    /* 0x93C */ cBgS_GndChk mGndChk;
    /* 0x978 */ daObj_GrvStn_prtclMngr_c mPrtclMngr[4];
    /* 0xAE8 */ s16 mTimer;

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl const mCcDCyl;
};

STATIC_ASSERT(sizeof(daGraveStone_c) == 0xAEC);

#endif /* D_A_OBJ_GRAVE_STONE_H */
