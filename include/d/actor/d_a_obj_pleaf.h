#ifndef D_A_OBJ_PLEAF_H
#define D_A_OBJ_PLEAF_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObj_Pleaf_c
 * @brief Ordon Pumpkin Vine
 *
 * @details
 *
 */
class daObj_Pleaf_c : public fopAc_ac_c {
private:
    /* 0x568*/ u8 field_0x568[0x56C - 0x568];
    /* 0x56C */ request_of_phase_process_class mPhaseReq;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dBgS_ObjAcch mObjAcch;
    /* 0x750 */ dCcD_Stts mStts;
    /* 0x78C */ dBgS_AcchCir mAcchCir;
    /* 0x7CC */ cBgS_GndChk mGndChk;
    /* 0x808 */ dCcD_Sph mSph;
    /* 0x940 */ f32 mGroundDist;
    /* 0x944 */ u32 mShadowKey;
    /* 0x948 */ u8 mType;

public:
    virtual ~daObj_Pleaf_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    char* getResName();
    int isDelete();
    void setEnvTevColor();
    void setRoomNo();
    void setMtx();

    u32 getType() { return 0; }
};

STATIC_ASSERT(sizeof(daObj_Pleaf_c) == 0x950);

class daObj_Pleaf_Param_c {
public:
    virtual ~daObj_Pleaf_Param_c() {}

    struct params {
        f32 field_0x0;
        f32 field_0x4;
        f32 field_0x8;
        f32 field_0xc;
    };

    static daObj_Pleaf_Param_c::params const m;
};

#endif /* D_A_OBJ_PLEAF_H */
