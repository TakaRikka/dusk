#ifndef D_A_OBJ_FLAG_H
#define D_A_OBJ_FLAG_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_set_bgobj.h"

static int createSolidHeap(fopAc_ac_c*);
static int nodeCallBack(J3DJoint*, int);

class FlagJoint_c {
public:
    ~FlagJoint_c();
    FlagJoint_c();

    csXyz mJoint1;
    csXyz mJoint2;
    csXyz mJoint3;
    s16 mRv;
};

/**
 * @ingroup actors-objects
 * @class daObjFlag_c
 * @brief Flag 1
 *
 * @details
 *
 */
class daObjFlag_c : public fopAc_ac_c {
private:
    /* 0x568 */ J3DModel* mpModel1;
    /* 0x56c */ J3DModel* mpModel2;
    /* 0x570 */ request_of_phase_process_class mPhase;
    /* 0x578 */ request_of_phase_process_class mPhase2;
    /* 0x580 */ FlagJoint_c mFlagJoints[4];
    /* 0x5d0 */ cXyz mPos;
    /* 0x5dc */ f32 field_0x5dc;
    /* 0x5e0 */ s16 field_0x5e0;
    /* 0x5e2 */ s16 field_0x5e2;
    /* 0x5e4 */ s16 field_0x5e4;
    /* 0x5e6 */ s16 field_0x5e6;
    /* 0x5e8 */ s16 field_0x5e8;
    /* 0x5ea */ u8 field_0x568[0x5ec - 0x5ea];

public:
    void create_init();
    void initBaseMtx();
    void getJointAngle(csXyz*, int);
    void calcJointAngle();
    void calcAngleSwingZ(FlagJoint_c*, f32);
    void calcAngleSwingX(FlagJoint_c*, f32);
    f32 getSwingY(f32);

    ~daObjFlag_c() {
        dComIfG_resDelete(&this->mPhase, "FlagObj");
        dComIfG_resDelete(&this->mPhase2, daSetBgObj_c::getArcName(this));
    }

    inline int create();
    inline int execute();
    inline int draw();
    inline int createHeap();

    struct M_attrs {
        /* 0x00 */ f32 field_0x00;
        /* 0x04 */ f32 field_0x04;
        /* 0x08 */ f32 field_0x08;
        /* 0x0c */ s16 field_0x0c;
        /* 0x0e */ s16 field_0x0e;
        /* 0x10 */ s16 fielx_0x10;
        /* 0x12 */ s16 fielx_0x12;
        /* 0x14 */ s16 field_0x14;
        /* 0x16 */ s16 field_0x16;
        /* 0x18 */ s16 field_0x18;
        /* 0x1a */ s16 field_0x1a;
        /* 0x1c */ s16 field_0x1c;
        /* 0x1e */ s16 field_0x1e;
        /* 0x20 */ s16 field_0x20;
        /* 0x22 */ s16 field_0x22;
        /* 0x24 */ u8 field_0x24;
        /* 0x25 */ u8 field_0x25;
        /* 0x26 */ u8 field_0x26;
        /* 0x28 */ f32 field_0x28;
        /* 0x2c */ f32 field_0x2c;
        /* 0x30 */ f32 field_0x30;
    };

    static M_attrs const M_attr;
    M_attrs const& attr() const { return M_attr; }
};

STATIC_ASSERT(sizeof(daObjFlag_c) == 0x5ec);


#endif /* D_A_OBJ_FLAG_H */
