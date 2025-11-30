#ifndef D_A_OBJ_TREESH_H
#define D_A_OBJ_TREESH_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daTreeSh_c
 * @brief Conifer Tree
 *
 * @details
 *
 */
class daTreeSh_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int create1st();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ csXyz mRot[2];
    /* 0x5B8 */ f32 field_0x5b8[2];
    /* 0x5C0 */ s16 field_0x5c0[2];
    /* 0x5C4 */ s16 field_0x5c4[2];
    /* 0x5C8 */ s16 mShakeTimer[2];
};

STATIC_ASSERT(sizeof(daTreeSh_c) == 0x5cc);

class daTreeSh_HIO_c : public mDoHIO_entry_c {
public:
    daTreeSh_HIO_c();
    virtual ~daTreeSh_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s16 shake_strength;
    /* 0x6 */ s16 shake_speed;
    /* 0x8 */ s16 field_0x8;
    /* 0xA */ csXyz field_0xa;
};


#endif /* D_A_OBJ_TREESH_H */
