#ifndef D_A_OBJ_BED_H
#define D_A_OBJ_BED_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "f_op/f_op_actor.h"
#include "dolphin/types.h"

class dBgW;

/**
 * @ingroup actors-objects
 * @class daObj_Bed_c
 * @brief Bed
 *
 * @details
 *
 */
class daObj_Bed_c : public fopAc_ac_c {
public:
    /* 0x568 */ u8 field_0x568[4];
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dBgS_ObjAcch mAcch;
    /* 0x750 */ dBgS_AcchCir mAcchCir;
    /* 0x790 */ cBgS_GndChk mGndChk;
    /* 0x7CC */ Mtx mMtx;
    /* 0x7FC */ dBgW* mpCollider;
    /* 0x800 */ cM3dGCyl field_0x800;
    /* 0x818 */ cM3dGLin field_0x818;
    /* 0x834 */ f32 mGroundH;
    /* 0x838 */ int mShadow;
    /* 0x83C */ u8 mType;
    /* 0x840 */ /* vtable */

    virtual ~daObj_Bed_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 getType();
    char* getResName();
    int isDelete();
    void setEnvTevColor();
    void setRoomNo();
    void setMtx();
};

STATIC_ASSERT(sizeof(daObj_Bed_c) == 0x844);

class daObj_Bed_Param_c {
public:
    virtual ~daObj_Bed_Param_c() {}

    struct params {
        /* 0x0 */ f32 field_0x0;
        /* 0x4 */ f32 field_0x4;
        /* 0x8 */ f32 field_0x8;
        /* 0xC */ f32 field_0xc;
    };

    static daObj_Bed_Param_c::params const m;
};

#endif /* D_A_OBJ_BED_H */
