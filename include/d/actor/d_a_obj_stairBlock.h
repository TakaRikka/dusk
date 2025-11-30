#ifndef D_A_OBJ_STAIRBLOCK_H
#define D_A_OBJ_STAIRBLOCK_H

#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daStairBlock_c
 * @brief Stair Block
 *
 * @details
 *
 */
class daStairBlock_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    virtual int CreateHeap();
    int create();
    static int createHeapCallBack(fopAc_ac_c*);
    virtual int Execute();
    virtual int Draw();
    virtual int Delete();

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcSph mCcDSph;

private:
    /* 0x056C */ request_of_phase_process_class mPhaseReq;
    /* 0x0574 */ J3DModel* mpModel[10];
    /* 0x059C */ mDoExt_bckAnm mBckAnm[10];
    /* 0x06B4 */ u8 mHit[10];
    /* 0x06C0 */ dCcD_Stts mStts;
    /* 0x06FC */ dCcD_Sph mSpheres[10];
};

STATIC_ASSERT(sizeof(daStairBlock_c) == 0x132c);

class daStairBlock_HIO_c : public mDoHIO_entry_c {
public:
    daStairBlock_HIO_c();
    virtual ~daStairBlock_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 collision_radius[10];
}; // Size: 0x2C

#endif /* D_A_OBJ_STAIRBLOCK_H */
