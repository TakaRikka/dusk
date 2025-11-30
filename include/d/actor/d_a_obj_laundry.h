#ifndef D_A_OBJ_LAUNDRY_H
#define D_A_OBJ_LAUNDRY_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjLdy_c
 * @brief Laundry
 *
 * @details
 *
 */
class LaundJoint_c {
public:
    cXyz mPos1;
    cXyz mPos2;
    cXyz mPos3;
    cXyz mPos4;
    csXyz mAngle;
};

struct daObjLdy_Attr_c {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    u16 field_0x30;
    u8 field_0x32;
    u8 field_0x33;
};

class daObjLdy_c : public fopAc_ac_c {
public:
    void create_init();
    void initBaseMtx();
    void setBaseMtx();
    void getJointAngle(csXyz*, int);
    void setNormalClothPos();
    void calcJointAngle();
    bool divorceParent();

    inline ~daObjLdy_c();
    inline int create();
    inline int createHeap();
    inline int getObjType();
    inline int daObjLdy_Draw();
    inline int daObjLdy_Execute();

private:
    static const daObjLdy_Attr_c mAttr;

    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x570 */ request_of_phase_process_class mPhase;
    /* 0x578 */ Mtx mMtx;
    /* 0x5A8 */ dCcD_Stts mStts;
    /* 0x5E4 */ dCcD_Cyl mCyl;
    /* 0x720 */ LaundJoint_c mJoints[3];
};

STATIC_ASSERT(sizeof(daObjLdy_c) == 0x7c8);

#endif /* D_A_OBJ_LAUNDRY_H */
