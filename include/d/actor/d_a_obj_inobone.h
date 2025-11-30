#ifndef D_A_OBJ_INOBONE_H
#define D_A_OBJ_INOBONE_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjIBone_c
 * @brief Boar Bone
 *
 * @details
 *
 */
class daObjIBone_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    void setBreakEffect();
    int draw();
    int _delete();

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ dCcD_Stts mStts;
    /* 0x5B0 */ dCcD_Cyl mCyl;
    /* 0x6EC */ dCcD_Cyl mCyl2;
    /* 0x828 */ u8 field_0x828;
    /* 0x82C */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(daObjIBone_c) == 0x84c);

#endif /* D_A_OBJ_INOBONE_H */
