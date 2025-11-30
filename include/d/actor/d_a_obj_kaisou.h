#ifndef D_A_OBJ_KAISOU_H
#define D_A_OBJ_KAISOU_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjKaisou_c
 * @brief Seaweed
 *
 * @details
 *
*/

class daObjKaisou_c : public fopAc_ac_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    void initCcCylinder();
    void setCcCylinder();
    void ObjHit();
    int Execute();
    int Delete();
    void setBaseMtx();
    cPhs__Step create();
    inline int CreateHeap();
    inline int Draw();

private:
    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ u8 field_0x569;
    /* 0x56A */ u8 field_0x56a[0x56e - 0x56a];
    /* 0x56E */ s16 mJoint1ZRot;
    /* 0x570 */ s16 mJoint2ZRot;
    /* 0x572 */ s16 mJoint3ZRot;
    /* 0x574 */ s16 field_0x574;
    /* 0x576 */ s16 field_0x576;
    /* 0x578 */ s16 field_0x578;
    /* 0x57A */ s16 field_0x57a;
    /* 0x57C */ s16 field_0x57c;
    /* 0x57E */ s16 field_0x57e;
    /* 0x580 */ s16 field_0x580;
    /* 0x582 */ s16 field_0x582;
    /* 0x584 */ s16 mJointYRot;
    /* 0x588 */ Z2SoundObjSimple mSound;
    /* 0x5A8 */ s16 mInvulnerabilityTimer;
    /* 0x5AA */ u8 field_0x5aa[0x5b0 - 0x5aa];
    /* 0x5B0 */ J3DModel* mpModel;
    /* 0x5B4 */ request_of_phase_process_class mPhase;
    /* 0x5BC */ dBgS_AcchCir mAcchCir;
    /* 0x5FC */ dCcD_Stts mStts;
    /* 0x638 */ dCcD_Cyl mCyl;
    /* 0x774 */ dCcU_AtInfo mAtInfo;
    /* 0x798 */ u8 field_0x798[0x79c - 0x798];
};

STATIC_ASSERT(sizeof(daObjKaisou_c) == 0x79c);


#endif /* D_A_OBJ_KAISOU_H */
