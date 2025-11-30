#ifndef D_A_OBJ_HATA_H
#define D_A_OBJ_HATA_H

#include "SSystem/SComponent/c_phase.h"
#include "dolphin/types.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjHata_c
 * @brief Flag ???
 *
 * @details
 *
*/

class daObjHata_c : public fopAc_ac_c, request_of_phase_process_class {
public:
    daObjHata_c();
    virtual ~daObjHata_c();
    int createHeap();
    cPhs__Step create();
    int Delete();
    int draw();
    int execute();
    void setModelMtx();
    void init();
    void moveSwing();

    void getJntAngle(csXyz* angle, int idx) { *angle = jnt_angle[idx]; }
    
    /* 0x574 */ csXyz jnt_angle[3];
    /* 0x588 */ J3DModel* model;
    /* 0x58C */ f32 field_0x58c[3];
    /* 0x598 */ f32 field_0x598[3];
    /* 0x5A4 */ f32 field_0x5a4[3];
    /* 0x5B0 */ f32 field_0x5b0[3];
    /* 0x5BC */ s8 field_0x5bc[3];
    /* 0x5BF */ s8 field_0x5bf[3];
};

STATIC_ASSERT(sizeof(daObjHata_c) == 0x5C4);

#endif /* D_A_OBJ_HATA_H */
