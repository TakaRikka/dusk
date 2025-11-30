#ifndef D_A_OBJ_NOUGU_H
#define D_A_OBJ_NOUGU_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObj_Nougu_c
 * @brief Farm Tools
 *
 * @details
 *
 */
class daObj_Nougu_c : public fopAc_ac_c {
public:
    /* 0x568 */ u8 field_0x568[0x56C - 0x568];
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dBgS_ObjAcch mAcch;
    /* 0x750 */ dCcD_Stts mCcStts;
    /* 0x78C */ dBgS_AcchCir mAcchCir;
    /* 0x7CC */ cBgS_GndChk mGndChk;
    /* 0x808 */ dCcD_Sph mCcSph[5];
    /* 0xE20 */ f32 mGroundH;
    /* 0xE24 */ u32 mShadowId;
    /* 0xE28 */ u8 mType;

    virtual ~daObj_Nougu_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    const char* getResName();
    int isDelete();
    void setEnvTevColor();
    void setRoomNo();
    void setMtx();

    int getType() { return 0; }

    static dCcD_SrcSph mCcDSph;
};

STATIC_ASSERT(sizeof(daObj_Nougu_c) == 0xe30);

struct daObj_Nougu_HIOParam {
    /* 0x0 */ f32 attention_offset;
    /* 0x4 */ f32 gravity;
    /* 0x8 */ f32 scale;
    /* 0xC */ f32 shadow_size;
};

class daObj_Nougu_Param_c {
public:
    virtual ~daObj_Nougu_Param_c() {}

    static const daObj_Nougu_HIOParam m;
};


#endif /* D_A_OBJ_NOUGU_H */
