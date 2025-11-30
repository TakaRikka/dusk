#ifndef D_A_OBJ_ORNAMENT_CLOTH_H
#define D_A_OBJ_ORNAMENT_CLOTH_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjOnCloth_c
 * @brief Ordon Village Flag
 *
 * @details
 *
*/

class daObjOnCloth_Attr_c {
public:
    /* 0x00 */ f32 gravity;
    /* 0x04 */ f32 offset;
    /* 0x08 */ f32 windRate;
    /* 0x0C */ f32 speedRate;
    /* 0x10 */ f32 revPowRate;
    /* 0x14 */ f32 playerReactionDist;
    /* 0x18 */ f32 playerReactionSpeed;
    /* 0x1C */ f32 playerReactionCoeff;
    /* 0x20 */ f32 reflectReducePowCoeff;
    /* 0x24 */ s16 rotationLimit[3];
    /* 0x2A */ s16 delayedVibrationFrame;
    /* 0x2C */ u8 jointDisplay;
};

class ClothJoint_c {
public:
    ~ClothJoint_c();
    ClothJoint_c();

    /* 0x00 */ cXyz field_0x0;
    /* 0x0C */ cXyz field_0xc;
    /* 0x18 */ cXyz field_0x18;
    /* 0x24 */ cXyz field_0x24;
    /* 0x30 */ csXyz field_0x30;
    /* 0x36 */ u8 field_0x36;
    /* 0x37 */ u8 field_0x37;
};

class daObjOnCloth_c : public fopAc_ac_c {
public:
    void create_init();
    void initBaseMtx();
    void setBaseMtx();
    void setNormalClothPos();
    void calcJointAngle();
    void checkPlayerMove();

    f32 getColorType() { return fopAcM_GetParam(this) & 0xFF; }
    inline void getJointAngle(csXyz& jointAngle, int param_2) { jointAngle = mClothJoints[param_2].field_0x30; }
    inline int createHeap();
    inline int draw();
    inline int execute();
    inline ~daObjOnCloth_c();
    inline cPhs__Step create();

    const daObjOnCloth_Attr_c& attr() const { return M_attr; }

    static daObjOnCloth_Attr_c const M_attr;

    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ mDoExt_btkAnm* mBtkAnm;
    /* 0x570 */ request_of_phase_process_class mPhase;
    /* 0x578 */ Mtx mMtx;
    /* 0x5A8 */ ClothJoint_c mClothJoints[3];
    /* 0x650 */ f32 field_0x650[2];
    /* 0x658 */ u8 field_0x658[0x688 - 0x658];
    /* 0x688 */ f32 field_0x688;
    /* 0x68C */ u8 field_0x68c[0x6c8 - 0x68c];
};

STATIC_ASSERT(sizeof(daObjOnCloth_c) == 0x6c8);

#endif /* D_A_OBJ_ORNAMENT_CLOTH_H */
