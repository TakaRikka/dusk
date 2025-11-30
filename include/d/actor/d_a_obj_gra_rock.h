#ifndef D_A_OBJ_GRA_ROCK_H
#define D_A_OBJ_GRA_ROCK_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjGraRock_c
 * @brief Goron Entombing Rock
 *
 * @details
 *
 */
class daObjGraRock_c : public dBgS_MoveBgActor {
public:
    void setAttnPos();
    void setBaseMtx();
    void setPrtcl();
    void bombParticleSet();
    void setEnvTevColor();
    void setRoomNo();
    void col_set();
    int checkHitAt(cCcD_Obj*);
    int Create();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    static dCcD_SrcCyl const mCcDCyl;

private:
    /* 0x5A0 */ request_of_phase_process_class mPhases[5];
    /* 0x5C8 */ J3DModel* mModel;
    /* 0x5CC */ mDoExt_btpAnm mBtp;
    /* 0x5E4 */ dBgS_ObjAcch mAcch;
    /* 0x7BC */ dCcD_Stts mStts;
    /* 0x7F8 */ dBgS_AcchCir mAcchCir;
    /* 0x838 */ dCcD_Cyl mCyl;
    /* 0x974 */ cBgS_GndChk mGndChk;
    /* 0x9B0 */ s16 field_0x9b0;
    /* 0x9B2 */ s16 field_0x9b2;
    /* 0x9B4 */ s16 field_0x9b4;
    /* 0x9B6 */ s16 field_0x9b6;
    /* 0x9B8 */ s16 field_0x9b8;
    /* 0x9BA */ s16 field_0x9ba;
    /* 0x9BC */ u8 field_0x9bc;
    /* 0x9BD */ u8 field_0x9bd;
    /* 0x9BD */ s16 field_0x9be;
    /* 0x9C0 */ u32 mParticleIds[2];
    /* 0x9BD */ u8 field_0x9c8;
};

//STATIC_ASSERT(sizeof(daObjGraRock_c) == 0x9cc);


#endif /* D_A_OBJ_GRA_ROCK_H */
