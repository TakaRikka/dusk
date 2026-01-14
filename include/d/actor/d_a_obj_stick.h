#ifndef D_A_OBJ_STICK_H
#define D_A_OBJ_STICK_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

struct daObj_Stick_HIOParam {
    f32 attention_offset;
    f32 gravity;
    f32 scale;
    f32 real_shadow_size;
};

class daObj_Stick_Param_c {
public:
    virtual ~daObj_Stick_Param_c() {}

    static const daObj_Stick_HIOParam m;
};

#if DEBUG
class daObj_Stick_HIO_c : public mDoHIO_entry_c {
public:
    daObj_Stick_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daObj_Stick_HIOParam m;
};

#define OBJ_STICK_HIO_CLASS daObj_Stick_HIO_c
#else
#define OBJ_STICK_HIO_CLASS daObj_Stick_Param_c
#endif

/**
 * @ingroup actors-objects
 * @class daObj_Stick_c
 * @brief Stick
 *
 * @details
 *
 */
class daObj_Stick_c : public fopAc_ac_c {
private:
    /* 0x568 */ OBJ_STICK_HIO_CLASS* mpHIO;
    /* 0x56c */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dBgS_ObjAcch mAcch;
    /* 0x750 */ dCcD_Stts mStts;
    /* 0x78c */ dBgS_AcchCir mAcchCir;
    /* 0x7cc */ cBgS_GndChk mGndChk;
    /* 0x808 */ dCcD_Sph mSph;
    /* 0x940 */ f32 mGroundHeight;
    /* 0x944 */ u32 mShadowKey;
    /* 0x948 */ u8 mType;
public:
    virtual ~daObj_Stick_c();
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

    u32 getType() { return 0; }

    static dCcD_SrcSph mCcDSph;
};

STATIC_ASSERT(sizeof(daObj_Stick_c) == 0x950);

#endif /* D_A_OBJ_STICK_H */
